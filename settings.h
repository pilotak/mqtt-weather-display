const byte TFT_DC = 5;
const byte TFT_CS = 15;

const unsigned int background_colour = 0x0000;
const unsigned int font_colour = 0xFFFF;

#define NTP_SERVER_NAME "ntp.nic.cz"
#define UDP_LOCAL_PORT 8888
#define NTP_SYNC_INTERVAL 300 //seconds

#define WIFI_SSID "***"
#define WIFI_PASSWORD "***"

TimeChangeRule summerTime = {"CEST", Last, Sun, Mar, 2, 120};    // UTC + 2 hours
TimeChangeRule standardTime = {"CET", Last, Sun, Oct, 2, 60};    // UTC + 1 hour

#define TIME_REFRESH 60 //seconds; must be greater then 0
#define DEVICE_NAME "ILI9341"

#define MQTT_SERVER "xxx.xxx.xxx.xxx"
#define MQTT_USER "***"
#define MQTT_PASSWORD "***"

#define MQTT_TOPIC "display/#"
#define MQTT_TOPIC_IN_TEMP "display/in_temp"
#define MQTT_TOPIC_OUT_TEMP "display/out_temp"
#define MQTT_TOPIC_OUT_TEMP_FEEL "display/out_temp_feel"
#define MQTT_TOPIC_FORECAST "display/forecast"
#define MQTT_TOPIC_POWER "display/power"
