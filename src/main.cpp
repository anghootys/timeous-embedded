#include "access_point_wifi.h"
#include "core.h"
#include "http_server.h"
#include "logger.h"


LoggerFacade main_logger(Logger::setup("MAIN"));

Time rtc_time;

AccessPoint ap;
HTTPServer http_server;

void register_http_server_handlers();

void middleware_cb(AsyncWebServerRequest *request, const std::function<void(AsyncWebServerRequest *)> &handler);


void
setup() {
    Logger::instance().initialize();

    main_logger.info("application starting.");

    main_logger.info("initializing Time.");
    rtc_time.initialize();
    Logger::instance().set_time_provider([]() { return rtc_time.get_full_time(); });
    main_logger.success("Time initialized successfully.");

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
}

void
register_http_server_handlers() {
    // for testing connection established between server and frontend application
    http_server.register_handler("/ping", HTTP_OPTIONS, [](AsyncWebServerRequest *request) { request->send(204); });

    // for setting timer
    http_server.register_handler("/timer", HTTP_PUT, [](AsyncWebServerRequest *request) {
        main_logger.info("updating timer value.");
        request->send(204);
    });

    // testing switch functionality
    http_server.register_handler("/test_switch", HTTP_POST, [](AsyncWebServerRequest *request) {
        main_logger.info("testing switch functionality.");
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
