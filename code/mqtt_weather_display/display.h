uint8_t centerHelper(float number) {
    uint8_t score = (number < 0.0 ? 1 : 0);
    number < -9.99 ? score++ : score;  // -10
    number > 9.99 ? score++ : score;   // 10
    return score;
}

void drawIcon(const uint16_t* icon, int x, int y, int w, int h) {
    int buffidx = 0;

    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            uint16_t colour = pgm_read_word(icon + buffidx);

            if (colour != background_colour) {
                tft.drawPixel(col + x, row + y, colour);
            }

            buffidx++;
        }
    }

    yield();
}

void drawCover(coverIcon icon, const char* text1, const char* text2) {
    uint8_t text1_len = strlen(text1);
    uint8_t text2_len = strlen(text2);
    tft.fillScreen(background_colour);

    if (icon == ICON_WIFI) {
        drawIcon(wifi_0, 110, 150, 20, 20);
        drawIcon(wifi_1, 97, 130, 46, 17);
        drawIcon(wifi_2, 84, 110, 72, 23);
        drawIcon(wifi_3, 70, 91, 100, 28);

    } else if (icon == ICON_REFRESH || icon == ICON_OTA) {
        drawIcon(refresh, 70, 86, 100, 84);

        if (icon == ICON_OTA) {
            tft.drawRect(18, 184, 204, 14, font_colour);
        }
    }

    if (text1_len > 0) {
        tft.setTextColor(font_colour);
        tft.setFont(&Open_Sans_Bold_18);
        tft.setCursor((byte)((240 - text1_len * 11) / 2) + 4, 200);
        tft.print(text1);
    }

    if (text2_len > 0) {
        tft.setCursor((byte)((240 - text2_len * 11) / 2) + 4, 230);
        tft.print(text2);
    }
}

void animateWifi(int part) {
    if (part == 0) {
        tft.fillRect(70, 91, 100, 58, background_colour);

    } else if (part == 1) {
        drawIcon(wifi_1, 97, 130, 46, 17);

    } else if (part == 2) {
        drawIcon(wifi_2, 84, 110, 72, 23);

    } else if (part == 3) {
        drawIcon(wifi_3, 70, 91, 100, 28);
    }
}

void drawOTABar() {
    tft.fillRect(20, 186, ota_percentage * 2, 10, font_colour);
}

void drawForecast() {
    if (!night_mode && forecast_icon != SHRT_MIN) {
        tft.fillRect(0, 0, 95, 122, background_colour);

        switch (forecast_icon) {
            case 0:
            case 28:
                drawIcon(sunny);
                break;

            case 1:
                drawIcon(moon);
                break;

            case 2:
            case 3:
            case 5:
            case 34:
                drawIcon(cloudy);
                break;

            case 4:
                drawIcon(partly_cloudy_night);
                break;

            case 6:
            case 7:
            case 10:
                drawIcon(fog);
                break;

            case 8:
            case 12:
                drawIcon(heavy_rain);
                break;

            case 9:
            case 19:
                drawIcon(partly_cloudy);
                break;

            case 11:
                drawIcon(fog_night);
                break;

            case 13:
            case 18:
                drawIcon(overcast);
                break;

            case 14:
                drawIcon(rain_night);
                break;

            case 15:
                drawIcon(showers_night);
                break;

            case 16:
            case 25:
                drawIcon(snow);
                break;

            case 17:
            case 29:
            case 30:
            case 31:
                drawIcon(thunder);
                break;

            case 20:
            case 21:
                drawIcon(rain);
                break;

            case 22:
                drawIcon(showers);
                break;

            case 23:
            case 24:
            case 27:
                drawIcon(sleet);
                break;

            case 26:
                drawIcon(snowmelt);
                break;

            case 32:
                drawIcon(tornado);
                break;

            case 35:
                drawIcon(wind_rain);
                break;

            case 0xFFFF:
                break;

            default:
                tft.setTextColor(0x8de4);
                tft.setFont(&Open_Sans_Bold_32);
                tft.setCursor(20, 63);
                tft.println(forecast_icon);
        }
    }
}

void drawTime() {
    tft.fillRect(0, 147, 240, 59, background_colour);
    time_t time_now = myTZ.toLocal(now(), &tcr);

    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02i:%02i", hour(time_now), minute(time_now));

    tft.setCursor(15, 205);
    tft.setTextColor(font_colour);
    tft.setFont(&Open_Sans_Bold_80);
    tft.setCursor(15, 205);
    tft.println(buffer);
}

void drawOutTemp() {
    uint8_t x = 0;
    uint8_t pos = 0;

    if (!night_mode && !isnan(out_temp)) {
        switch (centerHelper(out_temp)) {
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

        tft.fillRect(117, OUT_TEMP_POS_Y - 24, 113, 32, background_colour);
        tft.setTextColor(font_colour);
        tft.setFont(&Open_Sans_Bold_32);
        tft.setCursor(x, OUT_TEMP_POS_Y);
        tft.print(out_temp, 1);

        pos = tft.getCursorX();

        tft.fillCircle(pos + 4, OUT_TEMP_POS_Y - 19, 5, font_colour);           //°
        tft.drawFastHLine(pos + 3, OUT_TEMP_POS_Y - 21, 3, background_colour);  //°
        tft.drawFastHLine(pos + 2, OUT_TEMP_POS_Y - 20, 5, background_colour);  //°
        tft.drawFastHLine(pos + 2, OUT_TEMP_POS_Y - 19, 5, background_colour);  //°
        tft.drawFastHLine(pos + 2, OUT_TEMP_POS_Y - 18, 5, background_colour);  //°
        tft.drawFastHLine(pos + 3, OUT_TEMP_POS_Y - 17, 3, background_colour);  //°

        tft.print(" C");
    }
}

void drawOutTempFeel() {
    uint8_t x = 0;
    uint8_t pos = 0;

    if (!night_mode && !isnan(out_temp_feel)) {
        switch (centerHelper(out_temp_feel)) {
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

        tft.fillRect(170, OUT_TEMP_FEEL_POS_Y - 10, 60, 11, background_colour);
        tft.setTextColor(font_colour);
        tft.setFont(&Open_Sans_14);
        tft.setCursor(x, OUT_TEMP_FEEL_POS_Y);
        tft.print(out_temp_feel, 1);

        pos = tft.getCursorX();

        tft.drawPixel(pos + 1, OUT_TEMP_FEEL_POS_Y - 10, font_colour);  //°
        tft.drawPixel(pos + 2, OUT_TEMP_FEEL_POS_Y - 10, font_colour);  //°
        tft.drawPixel(pos, OUT_TEMP_FEEL_POS_Y - 9, font_colour);       //°
        tft.drawPixel(pos + 3, OUT_TEMP_FEEL_POS_Y - 9, font_colour);   //°
        tft.drawPixel(pos, OUT_TEMP_FEEL_POS_Y - 8, font_colour);       //°
        tft.drawPixel(pos + 3, OUT_TEMP_FEEL_POS_Y - 8, font_colour);   //°
        tft.drawPixel(pos + 1, OUT_TEMP_FEEL_POS_Y - 7, font_colour);   //°
        tft.drawPixel(pos + 2, OUT_TEMP_FEEL_POS_Y - 7, font_colour);   //°

        tft.println(" C");
    }
}

void drawInTemp() {
    uint8_t x = 0;
    uint8_t pos = 0;

    if (!night_mode && !isnan(in_temp)) {
        switch (centerHelper(in_temp)) {
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

        tft.fillRect(0, IN_TEMP_POS_Y - 14, 120, 15, background_colour);
        tft.setTextColor(font_colour);
        tft.setFont(&Open_Sans_Bold_18);
        tft.setCursor(x, IN_TEMP_POS_Y);
        tft.print(in_temp, 1);

        pos = tft.getCursorX();

        tft.fillCircle(pos + 2, IN_TEMP_POS_Y - 11, 2, font_colour);           //°
        tft.drawFastHLine(pos + 1, IN_TEMP_POS_Y - 11, 3, background_colour);  //°
        tft.drawFastVLine(pos + 2, IN_TEMP_POS_Y - 12, 3, background_colour);  //°

        tft.println(" C");
    }
}

void drawPower() {
    uint8_t x = 0;
    String output = String(power_value);

    if (!night_mode && power_value != USHRT_MAX) {
        switch (output.length()) {
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

        tft.fillRect(121, POWER_POS_Y - 14, 120, 15, background_colour);
        tft.setTextColor(font_colour);
        tft.setFont(&Open_Sans_Bold_18);

        output.concat("Wh");
        tft.setCursor(x, POWER_POS_Y);
        tft.print(output);
    }
}

void drawAll() {
    tft.fillScreen(background_colour);

    drawForecast();
    drawOutTemp();
    drawOutTempFeel();

    drawTime();

    if (!night_mode) {
        tft.drawFastVLine(120, 262, 49, font_colour);  // divider

        drawIcon(home, 47, 256, 27, 29);
        drawIcon(power, (47 + 120), 256, 27, 29);
    }

    drawInTemp();
    drawPower();

    //popup = false;
    //draw_all = false;
}

void setupDisplay() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    tft.begin(40000000);
    tft.fillScreen(background_colour);
}

void displayLoop() {
    static uint32_t last_status = 0;
    static uint8_t last_minute = 0;

    if ((millis() - last_status) >= 1000) {  // 1sec
        last_status = millis();
        uint8_t minute_now = minute();

        if (last_minute != minute_now) {
            last_minute = minute_now;
            drawTime();
        }
    }

    if (display_event > NONE) {
        if (!display_init && display_event != ALL) {
            display_init = true;
            drawAll();
        }

        switch (display_event) {
            case TIME:
                drawTime();
                break;

            case IN_TEMP:
                drawInTemp();
                break;

            case OUT_TEMP:
                drawOutTemp();
                break;

            case OUT_TEMP_FEEL:
                drawOutTempFeel();
                break;

            case POWER:
                drawPower();
                break;

            case FORECAST:
                drawForecast();
                break;

            case ALL:
                drawAll();
                break;

            default:
                break;
        }

        display_event = NONE;
    }
}
