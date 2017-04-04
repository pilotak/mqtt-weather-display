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

enum myVar {
  ICON_WIFI = 1,
  ICON_REFRESH = 2,
  ICON_OTA = 3
};
myVar my_var;

time_t current_time;
float out_temp, out_temp_feel, in_temp;
unsigned int power_value;
byte forecast_icon = 0xFF;

bool popup = false, draw_all = false;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);


/** _____HELPERS_____ **/
byte centerHelper(float number){
  byte score = (number < 0.0 ? 1 : 0);
  number < -9.99 ? score++ : score; // -10
  number > 9.99 ? score++ : score; // 10 
  return score;
}

void drawMyIcon(const uint16_t* icon, int x = 0, int y = 0, int w = 95, int h = 122);

void drawMyIcon(const uint16_t* icon, int x, int y, int w, int h){
  int buffidx = 0;
  
  for (int row = 0; row < h; row++) { // For each scanline...
    for (int col = 0; col < w; col++) { // For each pixel...
      uint16_t colour = pgm_read_word(icon + buffidx);
      if (colour != background_colour) tft.drawPixel(col+x, row+y, colour);
      buffidx++;
    } // end pixel
  }   
}


void animateWifi(int part){
  if(part == 0 ) {
    tft.fillRect(70, 91, 100, 58, background_colour);
  }
  else if(part == 1){
    drawMyIcon(wifi_1, 97, 130, 46, 17);
  }
  else if(part == 2){
    drawMyIcon(wifi_2, 84, 110, 72, 23);
  }
  else if(part == 3){
    drawMyIcon(wifi_3, 70, 91, 100, 28);
  }
}

void drawIconText(byte icon, char* text1, char* text2 = "");

void drawIconText(byte icon, char* text1, char* text2){
  byte text1_chars = strlen(text1);
  byte text2_chars = strlen(text2);
  tft.fillScreen(background_colour);
  popup = true;

  if(icon == ICON_WIFI){
    drawMyIcon(wifi_0, 110, 150, 20, 20);
    drawMyIcon(wifi_1, 97, 130, 46, 17);
    drawMyIcon(wifi_2, 84, 110, 72, 23);
    drawMyIcon(wifi_3, 70, 91, 100, 28);
  }
  else if(icon == ICON_REFRESH || icon == ICON_OTA){
    drawMyIcon(refresh, 70, 86, 100, 84);

    if(icon == ICON_OTA) {
      tft.drawRect(18, 184, 204, 14, font_colour);
    }
  }
  
  if(text1_chars > 0){
    tft.setTextColor(font_colour);
     tft.setFont(&Open_Sans_Bold_18);
    tft.setCursor((byte)((240-text1_chars*11)/2)+4,200);
    tft.print(text1);
  }

  if(text2_chars > 0){
    tft.setCursor((byte)((240-text2_chars*11)/2)+4,230);
    tft.print(text2);
  }
}

/** _____COMPONENTS_____ **/

void drawOTABar(byte percent){
  tft.fillRect(20, 186, percent*2, 10, font_colour);
}

void drawForecast(){
  tft.fillRect(0, 0, 95, 122, background_colour);
  switch(forecast_icon){
    case 0:
    case 28:
      drawMyIcon(sunny);
      break;
    case 1:
      drawMyIcon(moon);
      break;
    case 2:
    case 3:
    case 5:
    case 34:
      drawMyIcon(cloudy);
      break;
    case 4:
      drawMyIcon(partly_cloudy_night);
      break;
    case 6:
    case 7:
    case 10:
      drawMyIcon(fog);
      break;
    case 8:
    case 12:
      drawMyIcon(heavy_rain);
      break;
    case 9:
    case 19:
      drawMyIcon(partly_cloudy);
      break;
    case 11:
      drawMyIcon(fog_night);
      break;
    case 13:
    case 18:
      drawMyIcon(overcast);
      break;
    case 14:
      drawMyIcon(rain_night);
      break;
    case 15:
      drawMyIcon(showers_night);
      break;
    case 16:
    case 25:
      drawMyIcon(snow);
      break;
    case 17:
    case 29:
    case 30:
    case 31:
      drawMyIcon(thunder);
      break;
    case 20:
    case 21:
      drawMyIcon(rain);
      break;
    case 22:
      drawMyIcon(showers);
      break;
    case 23:
    case 24:
    case 27:
      drawMyIcon(sleet);
      break;
    case 26:
      drawMyIcon(snowmelt);
      break;
    case 32:
      drawMyIcon(tornado);
      break;
    case 35:
      drawMyIcon(wind_rain);
      break;
    default:
      tft.setCursor(20,63);
      tft.setTextColor(0x8de4);
      tft.setFont(&Open_Sans_Bold_32);
      tft.println(forecast_icon);
  }
}

void drawTime(){
  tft.fillRect(0, 147, 240, 59, background_colour);

  int hourNow = hour(current_time);
  int minuteNow = minute(current_time);
  String time = (hourNow < 10 ? "0" + String(hourNow) : String(hourNow));
  time.concat(":");
  time.concat((minuteNow < 10 ? "0" + String(minuteNow) : String(minuteNow)));

  tft.setCursor(15,205);
  tft.setTextColor(font_colour);
  tft.setFont(&Open_Sans_Bold_80);
  tft.println(time);
}

void drawOutTemp(){
  int y = 63;
  byte x = 0;
  byte score = centerHelper(out_temp);

  switch(score){
    case 0:
      x = 148;
      break;
    case 1:
      x = 129;
      break;
    case 2:
      x = 119;
      break;
  }
  
  tft.fillRect(117, y-19, 113, 26, background_colour);
  tft.setCursor(x,y);
  tft.setTextColor(font_colour);
  tft.setFont(&Open_Sans_Bold_32);
  tft.print(out_temp, 1);

  byte pos = tft.getCursorX();

  tft.fillCircle(pos+4, y-19, 5, font_colour);          //°
  tft.drawFastHLine(pos+3, y-21, 3, background_colour); //°
  tft.drawFastHLine(pos+2, y-20, 5, background_colour); //°
  tft.drawFastHLine(pos+2, y-19, 5, background_colour); //°
  tft.drawFastHLine(pos+2, y-18, 5, background_colour); //°
  tft.drawFastHLine(pos+3, y-17, 3, background_colour); //°
  
  tft.print(" C");
}


void drawOutTempFeel(){
  int y = 89;
  byte x = 0;
  byte score = centerHelper(out_temp_feel);

  switch(score){
    case 0:
      x = 188;
      break;
    case 1:
      x = 178; 
      break;
    case 2:
      x = 172;
      break;
  }
  tft.fillRect(170, y-10, 60, 11, background_colour);
  tft.setCursor(x,y);
  tft.setTextColor(font_colour);
  tft.setFont(&Open_Sans_14);
  tft.print(out_temp_feel,1);

  byte pos = tft.getCursorX();

  tft.drawPixel(pos+1, 79, font_colour); //°
  tft.drawPixel(pos+2, 79, font_colour); //°
  tft.drawPixel(pos, 80, font_colour);   //°
  tft.drawPixel(pos+3, 80, font_colour); //°
  tft.drawPixel(pos, 81, font_colour);   //°
  tft.drawPixel(pos+3, 81, font_colour); //°
  tft.drawPixel(pos+1, 82, font_colour); //°
  tft.drawPixel(pos+2, 82, font_colour); //°

  tft.println(" C");
}

void drawInTemp(){
  int y = 315;
  byte x = 0;
  byte score = centerHelper(in_temp);

  switch(score){
    case 0:
      x = 36;
      break;
    case 1:
      x = 30;
      break;
    case 2:
      x = 24;
      break;
  }
  tft.fillRect(0, y-14, 120, 15, background_colour);
  tft.setCursor(x,y);
  tft.setTextColor(font_colour);
  tft.setFont(&Open_Sans_Bold_18);
  tft.print(in_temp, 1);
  
  byte pos = tft.getCursorX();
  tft.fillCircle(pos+2, y-11, 2, font_colour);          //°
  tft.drawFastHLine(pos+1, y-11, 3, background_colour); //°
  tft.drawFastVLine(pos+2, y-12, 3, background_colour); //°
  
  tft.println(" C");
}

void drawPower(){
  String output = String(power_value);
  byte length = output.length();
  byte x = 0;
  int y = 315;

  switch(length){
    case 1:
      x = 163;
      break;
    case 2:
      x = 157;
      break;
    case 3:
      x = 148;
      break;
    case 4:
      x = 145;
      break;
  }
  tft.fillRect(121, y-14, 120, 15, background_colour);
  tft.setCursor(x,y);
  tft.setTextColor(font_colour);
  tft.setFont(&Open_Sans_Bold_18);
  output.concat("Wh");
  tft.print(output);
}

void drawAll(){
  tft.fillScreen(background_colour);
  drawForecast();
  drawOutTemp();
  drawOutTempFeel();
  drawTime();

  tft.drawFastVLine(120, 262, 49, font_colour);  // divider

  drawMyIcon(home, 47, 256, 27, 29);
  drawMyIcon(power, (47 + 120), 256, 27, 29);
  
  drawInTemp();
  drawPower();

  popup = false;
  draw_all = false;
}


