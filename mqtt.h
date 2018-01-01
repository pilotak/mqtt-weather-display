long lastReconnectAttempt = 0;

WiFiClient espClient;
PubSubClient client(espClient, mqttServer);

void mqttCallback(const MQTT::Publish& pub) {
  bool data_ok = true;
  Serial.print("Message arrived [");
  Serial.print(pub.topic());
  Serial.print("] ");
  Serial.println(pub.payload_string());

  StaticJsonBuffer<100> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(pub.payload());

  if (!data.success()) {
    Serial.println("parseObject() failed");
    data_ok = false;
  }

  float temp_tmp = data.get<float>("temp");
  int icon_tmp = data.get<int>("icon");
  int power_tmp = data.get<int>("power");
  int mode_tmp = data.get<int>("night");

  if(data_ok && pub.topic() == MQTT_TOPIC_IN_TEMP && temp_tmp != NULL) {
    in_temp = temp_tmp;
    if(!night_mode) drawInTemp();
  }
  else if(data_ok && pub.topic() == MQTT_TOPIC_OUT_TEMP && temp_tmp != NULL) {
    out_temp = temp_tmp;
    if(!night_mode) drawOutTemp();
  }
  else if(data_ok && pub.topic() == MQTT_TOPIC_OUT_TEMP_FEEL && temp_tmp != NULL) {
    out_temp_feel = temp_tmp;
    if(!night_mode) drawOutTempFeel();
  }
  else if(data_ok && pub.topic() == MQTT_TOPIC_FORECAST) {
    forecast_icon = icon_tmp;
    if(!night_mode) drawForecast();
  }
  else if(data_ok && pub.topic() == MQTT_TOPIC_POWER && power_tmp != NULL) {
    power_value = power_tmp;
    if(!night_mode) drawPower();
  }
  else if(data_ok && pub.topic() == MQTT_TOPIC_NIGHT_MODE) {
    night_mode = (bool)mode_tmp;
  }
  else if(pub.topic() == MQTT_TOPIC_NTP_SYNC) {
    Serial.println("NTP sync command");
    getNtpTime();
  }
}

void setupMqtt(){
  client.set_callback(mqttCallback);
}

bool mqttReconnect() {
  Serial.println("MQTT");
  drawIconText(ICON_WIFI, "Pripojovani MQTT", MQTT_SERVER);
  if (client.connect(MQTT::Connect(DEVICE_NAME).set_auth(MQTT_USER, MQTT_PASSWORD))) {
    client.subscribe(MQTT_TOPIC);
  }
  draw_all = true;
  return client.connected();
}



