long lastReconnectAttempt = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  StaticJsonBuffer<100> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(payload);

  if (!data.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  float temp_tmp = data.get<float>("temp");
  unsigned int icon_tmp = data.get<unsigned int>("icon");
  unsigned int power_tmp = data.get<unsigned int>("power");

  if(strcmp(topic, MQTT_TOPIC_IN_TEMP) == 0 && temp_tmp != NULL) {
    in_temp = temp_tmp;
    drawInTemp();
  }
  else if(strcmp(topic, MQTT_TOPIC_OUT_TEMP) == 0 && temp_tmp != NULL) {
    out_temp = temp_tmp;
    drawOutTemp();
  }
  else if(strcmp(topic, MQTT_TOPIC_OUT_TEMP_FEEL) == 0 && temp_tmp != NULL) {
    out_temp_feel = temp_tmp;
    drawOutTempFeel();
  }
  else if(strcmp(topic, MQTT_TOPIC_FORECAST) == 0 && icon_tmp != NULL) {
    forecast_icon = icon_tmp;
    drawForecast();
  }
  else if(strcmp(topic, MQTT_TOPIC_POWER) == 0 && power_tmp != NULL) {
    power_value = power_tmp;
    drawPower();
  }
}

void setupMqtt(){
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(mqttCallback);
}

bool mqttReconnect() {
  Serial.println("MQTT");
  drawIconText(ICON_WIFI, "Pripojovani MQTT", MQTT_SERVER);
  if (client.connect(DEVICE_NAME, MQTT_USER, MQTT_PASSWORD)) {
    client.subscribe(MQTT_TOPIC);
  }
  draw_all = true;
  return client.connected();
}



