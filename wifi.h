void setupWifi(){
  tft.fillScreen(background_colour);
  drawIconText(ICON_WIFI, "Pripojovani k", WIFI_SSID);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  byte part = 0;
  for (int i = 0; i < 25; i++){
    if ( WiFi.status() != WL_CONNECTED ) {
      delay(200);
      animateWifi(part);
      part++;
      if(part > 3) part = 0;
      delay(100);
    }
  }

  ArduinoOTA.setHostname(DEVICE_NAME);
  ArduinoOTA.onStart([]() {
    drawIconText(ICON_OTA, "", "Aktualizace FW");
  });
  ArduinoOTA.onEnd([]() {
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    drawOTABar((progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    drawIconText(ICON_REFRESH, "Aktualizace FW se", "NEZDARILA");
    delay(1000);
  });
  ArduinoOTA.begin();
  
  draw_all = true;
}

