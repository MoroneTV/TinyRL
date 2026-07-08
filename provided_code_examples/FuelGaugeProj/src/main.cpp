#include <Arduino.h>

#include "Adafruit_MAX1704X.h" //fuel gauge library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//define OLED screen dimensions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # 
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

//energy sampling time
#define SAMPLING_TIME 1000   //1000 millisecs

unsigned long lastMainTimerValue =0;
unsigned long lastTimeValue =0;
unsigned long timer = 0;
unsigned long timeChange = 0;
unsigned long now = 0;
unsigned long prevTime = 0;

//battery values
float cellVoltagePre =0.0;
float cellVoltageNow =0.0;
float cellVoltageChange = 0.0;

//create OLED display object "display"
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_MAX17048 maxlipo;   //creates fuel gauge object



//declare functions used
void readFuelGaugeMeasurement();

void setup() 
{
  Wire.begin();
  Serial.begin(115200);
 
  while (!maxlipo.begin())    //starts fuel gauge measurement sensor
  {
    Serial.println(F("Couldnt find Adafruit MAX17048!"));
    delay(2000);
  }
  Serial.print(F("Fuel gauge sensor found!"));

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
  }

  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}

void loop() 
{
   //start forever loop
  now = millis();   //current time in milliseconds

  timeChange = (now - lastTimeValue);
  if(timeChange> SAMPLING_TIME)              //computes angle, angular velocity
  {                                          //reaction wheel speed every 10ms
      cellVoltageNow = maxlipo.cellVoltage();
      cellVoltageChange = cellVoltageNow - cellVoltagePre;
      cellVoltagePre = cellVoltageNow;
      lastTimeValue = now;  

      readFuelGaugeMeasurement();
  }
}

void readFuelGaugeMeasurement()
{
  display.clearDisplay();                 //clear OLED screen
  display.setTextSize(1);                 //Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);    //Draw white text
  
  float cellVoltage = maxlipo.cellVoltage();  //reads cell voltage
  if (isnan(cellVoltage)) {
    Serial.println("Failed to read cell voltage, check battery is connected!");
    delay(1000);
    return;
  }
  
  float battPercent = (cellVoltage/4.2)*100;     //converts cell voltage reading to proportion of 4.2v
  if(battPercent >=100.0)
    battPercent = 100;
  
  display.setCursor(0,18);                  //Start at top-left corner (Col=0, Row=18)
  display.print(F("Voltage: "));
  display.print(String(cellVoltage, 2));    //converts integer to String before invoking display() function
                                            //a simple print with cursor spec will position value at the
                                            //end of previous cursor position

  display.setCursor(0,30);                  //Col=0,Row=30
  display.print(F("Proportion: "));
  display.print(String(battPercent, 1)); 
  display.println(F("%"));
  display.setCursor(0,45);                  //Col=0,Row=50
  display.print(F("V change: "));
  display.print(String(cellVoltageChange,1));
  display.println(F("/sec"));
  display.display(); 
  //delay(1000);  // save energy, dont query too often!
}