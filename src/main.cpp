#include <Arduino.h>

#include <Ticker.h>
#include "RTClib.h"

RTC_DS3231 rtc;

Ticker serialISRTicker;
volatile bool f_checkSerial = false;

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

void IRAM_ATTR
isr_checkSerial() {
    f_checkSerial = true;
}

[[noreturn]] void
setup() {
    Serial.begin(115200);

    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (true)
            delay(10);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
    }

    serialISRTicker.attach_ms(200, isr_checkSerial);
}

void
loop() {
    static unsigned long lastPrint = 0;

    if (millis() - lastPrint >= 3000) {
        lastPrint = millis();
        // Get the current time from the RTC
        DateTime now = rtc.now();

        // Getting each time field in individual variables
        // And adding a leading zero when needed;
        String yearStr = String(now.year(), DEC);
        String monthStr = (now.month() < 10 ? "0" : "") + String(now.month(), DEC);
        String dayStr = (now.day() < 10 ? "0" : "") + String(now.day(), DEC);
        String hourStr = (now.hour() < 10 ? "0" : "") + String(now.hour(), DEC);
        String minuteStr = (now.minute() < 10 ? "0" : "") + String(now.minute(), DEC);
        String secondStr = (now.second() < 10 ? "0" : "") + String(now.second(), DEC);
        String dayOfWeek = daysOfTheWeek[now.dayOfTheWeek()];

        // Complete time string
        String formattedTime = dayOfWeek + ", " + yearStr + "-" + monthStr + "-" + dayStr + " " + hourStr + ":"
                               + minuteStr + ":" + secondStr;

        // Print the complete formatted time
        Serial.println(formattedTime);

        // Getting temperature
        Serial.print(rtc.getTemperature());
        Serial.println("ºC");

        Serial.println();
    }

    if (f_checkSerial) {
        f_checkSerial = false;

        if (Serial.available()) {
            String time = Serial.readStringUntil('\n');
            Serial.printf("Updating time to %s %s\n", __DATE__, time.c_str());
            rtc.adjust(DateTime(__DATE__, time.c_str()));
        }
    }
}
