#ifndef Display_H
#define Display_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class Display {
    private:
        Adafruit_SSD1306 screen;

    public:
        Display();

        bool begin();
        void clear();
        void writeLines(String line1 = "", String line2 = "", String line3 = "", String line4 = "", String line5 = "" , String line6 = "");

};

#endif