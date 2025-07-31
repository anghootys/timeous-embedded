#include <ArduinoJson.h>
#include <AsyncJson.h>
#include "access_point_wifi.h"
#include "core.h"
#include "http_server.h"
#include "logger.h"
#include "storage.h"
#include "switch_service.h"

#define SWITCH_DIO_PINOUT 15
#define CHECK_TIMER_INTERVAL_MS (5000)

LoggerFacade main_logger(Logger::setup("MAIN"));

volatile unsigned long last_check_millis = 0;

Time rtc_time;
volatile bool flag_scheduled_timer_checked = false;

Storage storage;
volatile bool flag_write_scheduler_timer;
char *data_scheduler_time = nullptr;
SchedulerTimeMemScheme scheduled_timer{ 0 };

MOSFETSwitch switch_service(SWITCH_DIO_PINOUT);
volatile bool flag_test_switch = false;

AccessPoint ap;
HTTPServer http_server;

void register_http_server_handlers();

void middleware_cb(AsyncWebServerRequest *request, const std::function<void(AsyncWebServerRequest *)> &handler);

void read_scheduler_timer();

void
setup() {
    Logger::instance().initialize();

    main_logger.info("application starting.");

    main_logger.info("initializing Time.");
    rtc_time.initialize();
    Logger::instance().set_time_provider([]() { return rtc_time.get_full_time(); });
    main_logger.success("Time initialized successfully.");

    main_logger.info("initializing Storage service.");
    storage.initialize();
    main_logger.success("Storage service initialized successfully.");

    main_logger.info("reading scheduler timer value into ram.");
    read_scheduler_timer();
    main_logger.success("scheduler timer value read into ram successfully.");

    main_logger.info("initializing Switch service.");
    switch_service.initialize();
    main_logger.info("Switch service initialized successfully.");

    main_logger.info("initializing AccessPoint service.");
    ap.initialize();
    main_logger.success("AccessPoint service initialized successfully.");

    main_logger.info("initializing HTTPServer service.");
    http_server.initialize();
    main_logger.success("HTTPServer service initialized successfully.");

    http_server.register_middleware(middleware_cb);
    register_http_server_handlers();

    http_server.get_server()->begin();
}

void
loop() {
    auto m = millis();

    if (flag_scheduled_timer_checked) {
        if (rtc_time.is_timer_fired(2)) {
            main_logger.info("timer has been fired");
            last_check_millis = m;
            flag_scheduled_timer_checked = false;
            rtc_time.clear_timer(2);
        } else {
            delay(1000);
        }
    }

    if (!flag_scheduled_timer_checked && m - last_check_millis > CHECK_TIMER_INTERVAL_MS) {
        last_check_millis = m;

        if (rtc_time.is_scheduler_timer_period_came(scheduled_timer)) {
            main_logger.info("scheduled timer period came");
            flag_scheduled_timer_checked = true;
            rtc_time.setup_1H_timer();

            switch_service.test_switch();
        }
    }

    if (flag_test_switch) {
        flag_test_switch = false;
        switch_service.test_switch();
    }

    if (flag_write_scheduler_timer) {
        flag_write_scheduler_timer = false;

        storage.reset_cursor();
        auto scheduler_mem_data = Time::convert_time_string_to_scheduler_mem_data(data_scheduler_time);
        storage.write(scheduler_mem_data, sizeof(SchedulerTimeMemScheme));
        free(scheduler_mem_data);

        read_scheduler_timer();

        SchedulerTimeMemScheme ns = { 0 };
        sscanf((char *) data_scheduler_time, "%d:%d", (int *) &ns.hour, (int *) &ns.minute);

        if (scheduled_timer.hour == ns.hour && scheduled_timer.minute == ns.minute) {
            main_logger.success("scheduler timer written into flash memory successfully.");
            main_logger.success("scheduler timer:");
            main_logger.success((String(scheduled_timer.hour) + String(":") + String(scheduled_timer.minute)).c_str());
        } else {
            main_logger.error(
                    "the data has been read from flash memory is not same as the data that has been written.");
        }

        free(data_scheduler_time);
        data_scheduler_time = nullptr;
    }
}

void
register_http_server_handlers() {
    // for testing connection established between server and frontend application
    http_server.register_handler("/ping", HTTP_OPTIONS, [](AsyncWebServerRequest *request) { request->send(204); });


    // testing switch functionality
    http_server.register_handler("/test_switch", HTTP_POST, [](AsyncWebServerRequest *request) {
        main_logger.info("testing switch functionality.");

        flag_test_switch = true;

        request->send(204);
    });

    // get current RTC time
    http_server.register_handler("/time", HTTP_GET, [](AsyncWebServerRequest *request) {
        main_logger.info("sending RTCTime value.");

        char *time = rtc_time.get_full_time();

        size_t buf_len = (strlen(time) + 128) * sizeof(char);
        char *buf = alloc<char>(buf_len);

        snprintf(buf, buf_len, R"({"time": "%s"})", time);

        request->send(200, "application/json", buf);

        free(buf);
        free(time);
    });

    // return scheduler timer value
    http_server.register_handler("/scheduler_timer", HTTP_GET, [](AsyncWebServerRequest *request) {
        size_t buf_len = 128 * sizeof(char);
        char *buf = alloc<char>(buf_len);

        snprintf(buf, buf_len, R"({"timer": "%02d:%02d"})", scheduled_timer.hour, scheduled_timer.minute);

        request->send(200, "application/json", buf);

        free(buf);
    });

    auto *scheduler_api_json_web_handler
            = new AsyncCallbackJsonWebHandler("/schedule", [](AsyncWebServerRequest *request, JsonVariant &json) {
                  auto data = json.as<JsonObject>();

                  if (data.isNull() || !data["time"].is<const char *>()) {
                      main_logger.error("field 'time' not found in the body.");
                      request->send(401);
                  } else {
                      main_logger.info("writing scheduler timer into storage.");

                      data_scheduler_time = alloc<char>(16);
                      memcpy((void *) data_scheduler_time, (const char *) data["time"], 16);

                      flag_write_scheduler_timer = true;

                      request->send(200);
                  }
              });

    // for setting timer
    http_server.get_server()->addHandler(scheduler_api_json_web_handler);
}

void
middleware_cb(AsyncWebServerRequest *request, const std::function<void(AsyncWebServerRequest *)> &handler) {
    size_t buf_len = (strlen(request->url().c_str()) + 256) * sizeof(char);

    char *buf = alloc<char>(buf_len);
    snprintf(buf, buf_len, "Req <- %s '%s'.", request->methodToString(), request->url().c_str());
    main_logger.info(buf);

    if (request->hasHeader("X-TIME")) {
        rtc_time.update_time(&request->header("X-TIME"));
    }

    handler(request);

    free(buf);
}

void
read_scheduler_timer() {
    storage.reset_cursor();
    auto written_data = storage.read_all();

    auto ms = Time::convert_scheduler_mem_data_to_timer_scheme(written_data);

    scheduled_timer.hour = ms.hour;
    scheduled_timer.minute = ms.minute;

    free(written_data);
}
