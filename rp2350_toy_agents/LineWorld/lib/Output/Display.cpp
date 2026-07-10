#include "Display.h"

Display ::Display() : screen(Adafruit_SSD1306(128, 64, &Wire, -1)) {
}

bool Display :: begin() {

    if (!screen.begin(SSD1306_SWITCHCAPVCC, 0x3C)) return false;
    
    // Allow time for splash screen
    screen.display();
    sleep_ms(2000);
    screen.clearDisplay();
    return true;
}

void Display :: clear() {
    screen.clearDisplay();
    screen.display();
}

void Display :: writeLines(String line1, String line2, String line3, String line4, String line5, String line6, String line7, String line8) {
    screen.clearDisplay();
    screen.setTextSize(1);
    screen.setTextColor(SSD1306_WHITE);
    screen.setCursor(0, 0);
    screen.println(line1);
    screen.println(line2);
    screen.println(line3);
    screen.println(line4);
    screen.println(line5);
    screen.println(line6);
    screen.println(line7);
    screen.println(line8);
    screen.display();
}
