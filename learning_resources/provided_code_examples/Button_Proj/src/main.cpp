//This program  demonstrates the use of hardware interrupts. The Red and Black buttons
//are connected to pins GP12 and GP13. The pins are setup as INPUT pins in the setup
//section, using the pinMode command. When a button is momentarily pressed, its INPUT pin 
//changes from LOW (0) to HIGH (1), then reverts to LOW when the button is released. 

//The INPUT pins are attached to interrupts that transfer the program control to  
//interrupt handler functions when the button state changes. For the Red button, the
//handler function is redButtonPressed and for the Black button it is blackButtonPressed

//The handler functions turn ON and OFF the built-in LED on the Pico. In the program
//the variable associated with the built-in LED is called INDICATOR, and it's setup
//as an OUTPUT pin.

#include <Arduino.h>
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

#define REDButton 12     //connected to pin GP12
#define BLACKButton 13   //connected to pin GP13

volatile int buttonStatus = 0;

//declare functions used in program here

void redButtonPressed();
void blackButtonPressed();
void showRedButtonPressed();
void showBlackButtonPressed();

void setup(){
  Wire.begin();
  Serial.begin(115200);  // Serial is used for debugging
  
  //set REDButton and BLACKButton pins as INPUT (i.e., read pins)
  //both input pins are pulled LOW by default using 10k resistors
  pinMode(REDButton, INPUT);
  pinMode(BLACKButton, INPUT);

  //rather than polling the state of the buttons continously, we'll using 
  //interrupts to monitor them

  //attach an interrupt to each button. If the state of the button CHANGEs (i.e., is momentarily pressed)
  //invoke interrupt processing function - redButtonPressed() for REDButton and 
  //blackButtonPressed for BLACKButton
  attachInterrupt(REDButton, redButtonPressed, CHANGE);
  attachInterrupt(BLACKButton, blackButtonPressed, CHANGE);

  // initializes OLED display 
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
  }

  // OLED display library initializes this with an Adafruit splash screen.
  display.display();    //this function must be called at the end of display statements
  delay(2000);          // pauses for 2 seconds

  display.clearDisplay();                //clears OLED screen
  display.setTextSize(1);                 //Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);    //Draw white text
  display.setCursor(0,20);                 //Start at top-left corner (Col=0, Row=0)
  display.print(F("Press a Button"));
  display.display();
}

void loop(){
  //you can use the loop to process other aspects of the program
  
}

void showRedButtonPressed(){
  //prints red button pressed on OLED display
  display.clearDisplay();                 //clears OLED screen
  display.setTextSize(1);                 //Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);    //Draw white text
  display.setCursor(0,20);                 //Start at top-left corner (Col=0, Row=0)
  display.print(F("RED Button Pressed"));
  display.display();
}


void showBlackButtonPressed(){
  //prints black button pressed on OLED display
  display.clearDisplay();                 //clears OLED screen
  display.setTextSize(1);                 //Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);    //Draw white text
  display.setCursor(0,20);                 //Start at top-left corner (Col=0, Row=0)
  display.print(F("BLACK Button Pressed"));
  display.display();
}

//called when the red button is pressed
void redButtonPressed()
{
    buttonStatus = 1;
    showRedButtonPressed();
    Serial.println("Red Button Pressed");
}

//called when the black button is pressed
void blackButtonPressed(){
    buttonStatus = 0;
    showBlackButtonPressed();
    Serial.println("Black Button Pressed");
}

