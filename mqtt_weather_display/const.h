#define DEVICE_NAME "mqtt-weather-display"  // used for MQTT, OTA, AP
#define FW_VERSION "1.0.1"

#define CONFIG_PATH               "/config.json"
#define CONFIG_AP_SSID            DEVICE_NAME "-config"
#define CONFIG_AP_PASSWORD        "StrongPasswordHere"
#define CONFIG_PORTAL_TIMEOUT     120  // sec

#define DEFAULT_MQTT_PORT         1883
#define MQTT_QOS                  1
#define MQTT_RETAIN               true
#define MQTT_CHECK_INTERVAL       5000
#define MQTT_STATUS_INTERVAL      30000
#define MQTT_STATUS_TOPIC         DEVICE_NAME "/status"
#define MQTT_UPGRADE_TOPIC        DEVICE_NAME "/upgrade"
#define MQTT_UPGRADE_STATUS_TOPIC DEVICE_NAME "/upgrade/status"
#define MQTT_RESTART_TOPIC        DEVICE_NAME "/restart"
#define MQTT_NTP_TOPIC            DEVICE_NAME "/ntp"

#define MQTT_TOPIC_IN_TEMP        DEVICE_NAME "/in-temp"
#define MQTT_TOPIC_OUT_TEMP       DEVICE_NAME "/out-temp"
#define MQTT_TOPIC_OUT_TEMP_FEEL  DEVICE_NAME "/out-temp-feel"
#define MQTT_TOPIC_FORECAST       DEVICE_NAME "/forecast"
#define MQTT_TOPIC_POWER          DEVICE_NAME "/power"
#define MQTT_TOPIC_NIGHT_MODE     DEVICE_NAME "/night"

#define MQTT_STATUS_ALIVE 1
#define MQTT_STATUS_DEAD 0

#define BUTTON_PIN 0
#define LONG_PRESS 1000  // ms

#define NOFUSS_CHECK_INTERVAL 3600000  // 1h
#define OTA_PORT              8266  // for local updates

#if defined(NTP_SUPPORT) || defined(HAS_RTC)
#if defined(HAS_RTC)
#define TIME_SYNC_INTERVAL 180  // sec
#define NTP_TIME_RESYNC    24  // this * TIME_SYNC_INTERVAL = resync time
#else
#define TIME_SYNC_INTERVAL 600  // sec; NTP should not be that often
#endif

#define NTP_SERVER_NAME    "ntp.nic.cz"
#define NTP_SERVER_NAME    "pool.ntp.org"

TimeChangeRule summerTime   = {"CEST", Last, Sun, Mar, 2, 120};    // UTC + 2 hours
TimeChangeRule standardTime = {"CET", Last, Sun, Oct, 2, 60};    // UTC + 1 hour
#endif

#define SDA_PIN 4
#define SCL_PIN 2
#define TFT_DC  16
#define TFT_CS  15
#define LED_PIN 5

const uint16_t background_colour = 0x0000;
const uint16_t font_colour       = 0xFFFF;

#define IN_TEMP_POS_Y       315
#define OUT_TEMP_POS_Y      63
#define OUT_TEMP_FEEL_POS_Y 89
#define POWER_POS_Y         IN_TEMP_POS_Y
