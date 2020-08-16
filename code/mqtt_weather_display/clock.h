#if defined(HAS_RTC)
static time_t getRtcTime() {
#if defined(NTP_SUPPORT)
    static uint8_t counter = 1;

    if (counter >= NTP_TIME_RESYNC) {
        counter = 0;
        NTP.begin(NTP_SERVER_NAME);  // once synced, RTC will be used as default time provider
    }

    counter++;
#endif

    return RTC.get();
}
#endif

#if defined(NTP_SUPPORT)
void processNtpEvent(NTPSyncEvent_t ntpEvent) {
    if (ntpEvent == timeSyncd) {
        time_t last_time = NTP.getLastNTPSync();

        char buffer[12];
        size_t len = snprintf(buffer, sizeof(buffer), "%lu", last_time);

        if (last_time > 1546300800) {  // 1.1.2019
#if defined(HAS_RTC)
            RTC.set(last_time);

            if (RTC.get() == last_time) {
#if defined(DEBUG)
                Serial.println("[TIME] RTC OK, will use it as default sync provider");
#endif
                NTP.stop();
                setSyncInterval(TIME_SYNC_INTERVAL);
                setSyncProvider(getRtcTime);
            }

#endif
#if defined(DEBUG)
            Serial.print("[TIME] Got new NTP time: ");
            Serial.println(NTP.getTimeDateString(myTZ.toLocal(last_time, &tcr)));
#endif
        }
    }
}
#endif

#if defined(DEBUG) && defined(HAS_RTC)
void printDigits(int digits) {
    if (digits < 10) {
        Serial.print('0');
    }

    Serial.print(digits);
}
#endif

void setupTime() {
#if defined(HAS_RTC)
    Wire.begin(SDA_PIN, SCL_PIN);
    setSyncProvider(getRtcTime);
    setSyncInterval(TIME_SYNC_INTERVAL);

    if (timeStatus() != timeSet) {
#if defined(DEBUG)
        Serial.println("[TIME] Unable to sync with the RTC");
#endif

#if !defined(NTP_SUPPORT)

        while (1) {}

#endif

    } else {
        time_t current_time = myTZ.toLocal(now(), &tcr);
        drawTime();

#if defined(DEBUG)
        Serial.print("[TIME] System time synchronized - ");
        Serial.print(hour(current_time));
        Serial.print(":");
        printDigits(minute(current_time));
        Serial.print(":");
        printDigits(second(current_time));
        Serial.print(" ");
        printDigits(day());
        Serial.print("/");
        printDigits(month(current_time));
        Serial.print("/");
        Serial.print(year(current_time));
        Serial.println();
#endif
    }

#endif

#if defined(NTP_SUPPORT)
    NTP.onNTPSyncEvent([](NTPSyncEvent_t event) {
        ntpEvent = event;
        trigger_ntp_event = true;
    });

    NTP.setInterval(TIME_SYNC_INTERVAL);
    NTP.begin(NTP_SERVER_NAME);  // overrides setSyncProvider
#endif
}

void timeLoop() {
#if defined(NTP_SUPPORT)

    if (trigger_ntp_event) {
        trigger_ntp_event = false;
        processNtpEvent(ntpEvent);
    }

#endif
}
