#include <Arduino.h>

#include "BH1745NUC.h"          //light measurement sensor library
#include "Wire.h"
#include <Adafruit_GFX.h>       //OLED display support library
#include <Adafruit_SSD1306.h>   //OLED display library


//-- defines OLED screen dimensions ---
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # 
#define SCREEN_ADDRESS 0x3C //OLED I2C address

//creates OLED display object "display"
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//----defines light light measurement sensor parameters
#define BH1745NUC_DEVICE_ADDRESS_38 0x38    //light measurement sensor I2C address
BH1745NUC bh1745nuc = BH1745NUC();

//declares functions
void readLightMeasurements(); 

void setup()
{
    Wire.begin();         //Initializes the Wire library and join the I2C bus as a controller
                          //or a peripheral. It is normally be called only once.
    Serial.begin(115200); //Sets the data rate in bits per second (baud) for serial data transmission. 
                          //For communicating with Serial Monitor, make sure to use one of the baud ...

    // initializes OLED display 
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
    {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }

     // OLED display library initializes this with an Adafruit splash screen.
     display.display();    //this function must be called at the end of display statements
     delay(2000);          // pauses for 2 seconds

     // clears the display buffer
     display.clearDisplay();

     //start light level measurements
     bh1745nuc.begin(BH1745NUC_DEVICE_ADDRESS_38);
     bh1745nuc.startMeasurement();
}

//loops forever
void loop()
{
  display.clearDisplay();                 //clears OLED screen
  display.setTextSize(1);                 //Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);    //Draw white text
  display.setCursor(0,0);                 //Start at top-left corner (Col=0, Row=0)

  readLightMeasurements();                //reads and prints light measurement values
  delay(1000);                             //pauses for 1 second
}

void readLightMeasurements()
{
  if (!bh1745nuc.read())  //attempts to read light measurement values
  { 
    Serial.println("Light sensor failed to read measurement values!");
    delay(500);
    return;
  }
  unsigned short rgbc[4];    //creates a 4 value array
  rgbc[0] = bh1745nuc.red;
  rgbc[1] = bh1745nuc.green;
  rgbc[2] = bh1745nuc.blue;
  rgbc[3] = bh1745nuc.clear;

  //print light values on the serial monitor
  Serial.write("Red = ");
  Serial.print(rgbc[0]);
  Serial.write(" Green = ");
  Serial.print(rgbc[1]);
  Serial.write(" Blue = ");
  Serial.print(rgbc[2]);
  Serial.write(" Clear = ");
  Serial.println(rgbc[3]);  

  //display light values in OLED display
  display.println(F("Light measurements.. "));
  display.print(F("Red:   "));
  display.println(String(rgbc[0]));
  display.print(F("Green: "));
  display.println(String(rgbc[1]));
  display.print(F("Blue:  "));
  display.println(String(rgbc[2]));
  display.print(F("Clear: "));
  display.println(String(rgbc[3]));
  display.display();      //this function must be called for the OLED to display values
}
