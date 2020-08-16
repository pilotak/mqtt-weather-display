// wifi.h
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>

WiFiManager wifiManager;
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

class IntParameter : public WiFiManagerParameter {
  public:
    IntParameter(const char *id, const char *placeholder, int32_t value, const uint8_t length = 10, const char *custom = NULL)
        : WiFiManagerParameter("") {
        const char * type_number = "type=\"number\" ";
        uint16_t custom_len = 0;

        if (custom) {
            custom_len = strlen(custom);
        }

        uint16_t custom_buffer_len = custom_len + strlen(type_number) + 1;

        custom_buffer = new char[custom_buffer_len];

        if (custom_buffer) {
            int offset = snprintf(custom_buffer, custom_buffer_len, type_number);

            if (custom_len > 0 && offset > 0) {
                snprintf(custom_buffer + offset, custom_buffer_len - offset, custom);
            }

            init(id, placeholder, String(value).c_str(), length, custom_buffer, WFM_LABEL_BEFORE);
        }
    }

    ~IntParameter() {
        if (custom_buffer) {
            delete[] custom_buffer;
        }
    }

    int32_t getValue() {
        return String(WiFiManagerParameter::getValue()).toInt();
    }

    void setValue(int32_t value, uint8_t max_size) {
        char *buffer = new char[max_size];

        if (buffer) {
            itoa(value, buffer, 10);
            WiFiManagerParameter::setValue(buffer, max_size);
            delete[] buffer;
        }
    }

    char *custom_buffer;
};

// mqtt.h
#include <GAsyncMqttClient.h>
#include <Ticker.h>

GAsyncMqttClient mqtt;
Ticker mqttReconnectTimer;

char mqtt_server[40];
uint16_t mqtt_port = DEFAULT_MQTT_PORT;
char mqtt_user[16] = {0};
char mqtt_password[32] = {0};
char will[40];

void connectToMqtt();

WiFiManagerParameter custom_mqtt_server("mqtt_server", "MQTT server", mqtt_server, sizeof(mqtt_server), "required");
IntParameter custom_mqtt_port("mqtt_port", "MQTT port", mqtt_port, 4, "required");
WiFiManagerParameter custom_mqtt_user("mqtt_user", "MQTT user", mqtt_user, sizeof(mqtt_user),
                                      "placeholder=\"Leave blank if not aplicable\"");
WiFiManagerParameter custom_mqtt_password("mqtt_password", "MQTT password", mqtt_password, sizeof(mqtt_password),
        "placeholder=\"Leave blank if not aplicable\" type=\"password\"");


// ota.h
#if defined(NOFUSS_OTA)
    #include "NoFUSSClient.h"
    char nofuss_server[40];
    WiFiManagerParameter custom_nofuss_server("nofuss_server", "NoFUSS server", nofuss_server, sizeof(nofuss_server), "required");
#endif

#if defined(HTTP_OTA)
    #include <ESP8266HTTPClient.h>
    bool do_http_update = false;
    char http_ota_url[100];

    void httpUpdate(const char* url);
#endif

#if defined(ARDUINO_OTA)
    #include <ArduinoOTA.h>
#endif

bool ota_in_progess = false;

// button.h
#include <JC_Button.h>
Button button(BUTTON_PIN);

// clock.h
#if defined(NTP_SUPPORT) || defined(HAS_RTC)
    #include <Wire.h>
    #include "TimeLib.h"

    time_t current_time = 0;
    Timezone myTZ(summerTime, standardTime);
    TimeChangeRule *tcr;

    #if defined(NTP_SUPPORT)
        #include "NtpClientLib.h"
        bool trigger_ntp_event = false;
        NTPSyncEvent_t ntpEvent;
    #endif

    #if defined(HAS_RTC)
        #include "DS3232RTC.h"
        DS3232RTC RTC(false);
    #endif
#endif

// display.h
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include "fonts/OpenSans.h"
#include "icons/home.h"
#include "icons/power.h"
#include "icons/sunny.h"
#include "icons/moon.h"
#include "icons/cloudy.h"
#include "icons/partly_cloudy_night.h"
#include "icons/fog.h"
#include "icons/heavy_rain.h"
#include "icons/partly_cloudy.h"
#include "icons/fog_night.h"
#include "icons/overcast.h"
#include "icons/rain_night.h"
#include "icons/showers_night.h"
#include "icons/snow.h"
#include "icons/rain.h"
#include "icons/showers.h"
#include "icons/sleet.h"
#include "icons/snowmelt.h"
#include "icons/thunder.h"
#include "icons/tornado.h"
#include "icons/wind_rain.h"
#include "icons/refresh.h"
#include "icons/wifi_0.h"
#include "icons/wifi_1.h"
#include "icons/wifi_2.h"
#include "icons/wifi_3.h"

enum coverIcon {
    ICON_WIFI = 1,
    ICON_REFRESH,
    ICON_OTA
};

enum displayEvent {
    NONE = 0,
    TIME,
    IN_TEMP,
    OUT_TEMP,
    OUT_TEMP_FEEL,
    POWER,
    FORECAST,
    ALL
};

float out_temp = NAN;
float out_temp_feel = NAN;
float in_temp = NAN;
uint16_t power_value = USHRT_MAX;
int forecast_icon = SHRT_MIN;
bool night_mode = false;

/*bool night_mode_prev = false;

bool popup = false, draw_all = false;*/

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
displayEvent display_event = NONE;
uint8_t ota_percentage = 0;
bool display_init = false;

void drawIcon(const uint16_t* icon, int x = 0, int y = 0, int w = 95, int h = 122);
void drawCover(coverIcon icon, const char* text1, const char* text2 = NULL);
void drawOTABar();
void drawForecast();
void drawOutTemp();
void drawOutTempFeel();
void drawInTemp();
void drawPower();
void drawTime();
void drawAll();

