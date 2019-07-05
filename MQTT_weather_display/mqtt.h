void sendStatus() {
    if (mqtt.connected()) {
        StaticJsonDocument < JSON_OBJECT_SIZE(4) > json;
        char message[140];

        json["status"] = MQTT_STATUS_ALIVE;
        json["ip"] = WiFi.localIP().toString();
        json["signal"] = wifiManager.getRSSIasQuality(WiFi.RSSI());
        json["fw_ver"] = FW_VERSION;

        uint32_t len = serializeJson(json, message, sizeof(message));

#if defined(DEBUG)
        Serial.println("[MQTT] Sending status data:");
        serializeJsonPretty(json, Serial);
        Serial.println();
#endif

        mqtt.publish(MQTT_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, message, len);
    }
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
#if defined(DEBUG)
    Serial.print("[MQTT] Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");

    for (uint16_t i = 0; i < len; i++) {
        Serial.print(static_cast<char>(payload[i]));
    }

    Serial.println();
#endif

    if (strcmp(topic, MQTT_UPGRADE_TOPIC) == 0 && len >= 4) {
#if defined(HTTP_OTA)

        if (len > sizeof(http_ota_url)) {
            mqtt.publish(MQTT_UPGRADE_STATUS_TOPIC, MQTT_QOS, false, "URL too long");

        } else {
            memset(http_ota_url, 0, sizeof(http_ota_url));
            memcpy(http_ota_url, payload, len);
            do_http_update = true;
        }

#endif

    } else if (strcmp(topic, MQTT_RESTART_TOPIC) == 0) {
        wifiManager.reboot();

    } else if (strcmp(topic, MQTT_NTP_TOPIC) == 0) {
#if defined(NTP_SUPPORT)
#if defined(DEBUG)
        Serial.println("[TIME] Getting new NTP time");
#endif
        NTP.getTime();
#endif

    } else if (strcmp(topic, MQTT_TOPIC_IN_TEMP) == 0 && len > 1) {
        in_temp = atof(payload);
        display_event = IN_TEMP;

    } else if (strcmp(topic, MQTT_TOPIC_OUT_TEMP) == 0 && len > 1) {
        out_temp = atof(payload);
        display_event = OUT_TEMP;

    } else if (strcmp(topic, MQTT_TOPIC_OUT_TEMP_FEEL) == 0 && len > 1) {
        out_temp_feel = atof(payload);
        display_event = OUT_TEMP_FEEL;

    } else if (strcmp(topic, MQTT_TOPIC_FORECAST) == 0 && len > 0) {
        forecast_icon = atoi(payload);
        display_event = FORECAST;

    } else if (strcmp(topic, MQTT_TOPIC_POWER) == 0 && len > 0) {
        power_value = atoi(payload);
        display_event = POWER;

    } else if (strcmp(topic, MQTT_TOPIC_NIGHT_MODE) == 0 && len == 1) {
        if (payload[0] == '0') {
            if (night_mode) {
                night_mode = false;
                digitalWrite(LED_PIN, HIGH);
                display_event = ALL;
            }

        } else if (payload[0] == '1') {
            if (!night_mode) {
                night_mode = true;
                analogWrite(LED_PIN, 30);
                display_event = ALL;
            }
        }
    }
}

void connectToMqtt() {
#if defined(DEBUG)
    Serial.print("[MQTT] Connecting to: ");
    Serial.print(mqtt_server);
    Serial.print(":");
    Serial.println(mqtt_port);
#endif

    mqtt.connect();
}

void onMqttConnect(bool sessionPresent) {
#if defined(DEBUG)
    Serial.println("[MQTT] Connected");
#endif

    mqtt.subscribe(MQTT_RESTART_TOPIC, MQTT_QOS);

#if defined(HTTP_OTA)
    mqtt.subscribe(MQTT_UPGRADE_TOPIC, MQTT_QOS);
#endif

#if defined(NTP_SUPPORT)
    mqtt.subscribe(MQTT_NTP_TOPIC, MQTT_QOS);
#endif

    mqtt.subscribe(MQTT_TOPIC_IN_TEMP, MQTT_QOS);
    mqtt.subscribe(MQTT_TOPIC_OUT_TEMP, MQTT_QOS);
    mqtt.subscribe(MQTT_TOPIC_OUT_TEMP_FEEL, MQTT_QOS);
    mqtt.subscribe(MQTT_TOPIC_FORECAST, MQTT_QOS);
    mqtt.subscribe(MQTT_TOPIC_POWER, MQTT_QOS);
    mqtt.subscribe(MQTT_TOPIC_NIGHT_MODE, MQTT_QOS);

    sendStatus();
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    if (!ota_in_progess) {
#if defined(DEBUG)

        if ((int8_t)reason != 0) {
            Serial.print("[MQTT] Disconnected, rc=");
            Serial.println((int8_t)reason);
        }

#endif

        if (WiFi.isConnected()) {
            mqttReconnectTimer.once(2, connectToMqtt);
        }
    }
}

void mqttSetup() {
#if defined(DEBUG)
    Serial.println("[MQTT] Setup");
#endif

    IPAddress ip;

    if (ip.fromString(mqtt_server)) {  // check if server is IP address or hostname
        mqtt.setServer(ip, mqtt_port);

    } else {
        mqtt.setServer(mqtt_server, mqtt_port);
    }

    snprintf(will, sizeof(will), "{\"status\": %i}", MQTT_STATUS_DEAD);

    mqtt.setCredentials(mqtt_user, mqtt_password);
    mqtt.setWill(MQTT_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, will, strlen(will));
    mqtt.setKeepAlive((MQTT_STATUS_INTERVAL / 1000) + 2);  // converts ms->s + 2 sec extra, in case we have a delay
    mqtt.onConnect(onMqttConnect);
    mqtt.onDisconnect(onMqttDisconnect);
    mqtt.onMessage(onMqttMessage);
}

void mqttLoop() {
    static uint32_t last_status = 0;

    if (!ota_in_progess) {
        if ((millis() - last_status) >= MQTT_STATUS_INTERVAL) {
            last_status = millis();

            sendStatus();
        }
    }
}
