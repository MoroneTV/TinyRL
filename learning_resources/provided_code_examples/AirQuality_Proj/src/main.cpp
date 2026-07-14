//This program uses the BSEC 2 library from Bosch sensortec

#include <Arduino.h>
#include <bsec2.h>
#include "SPI.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Macros used 
#define PANIC_LED   LED_BUILTIN
#define ERROR_DUR   1000
#define TEMP_OFFSET 7
#define SAMPLE_RATE		BSEC_SAMPLE_RATE_CONT

//-- defines OLED screen dimensions ---
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # 
#define SCREEN_ADDRESS 0x3C //OLED I2C address

//creates OLED display object "display"
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//declare functions
void errLeds(void);
void checkBsecStatus(Bsec2 bsec);
void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec);
void displayAirQualityData();

//create an object of the class Bsec2 
Bsec2 envSensor;
String iaq, iaqAccuracy, temp, humidity, pressure, co2Eqv, bVOCEqv;

void setup(void){
    Serial.begin(115200);
    Wire.begin();
    SPI.begin();
    //desired subscription list of BSEC2 outputs that the sensor will provide 
    bsecSensor sensorList[] = {
        BSEC_OUTPUT_IAQ,
        BSEC_OUTPUT_RAW_TEMPERATURE,
        BSEC_OUTPUT_RAW_PRESSURE,
        BSEC_OUTPUT_RAW_HUMIDITY,
        BSEC_OUTPUT_RAW_GAS,
        BSEC_OUTPUT_STABILIZATION_STATUS,
        BSEC_OUTPUT_RUN_IN_STATUS,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
       
        BSEC_OUTPUT_STATIC_IAQ,
        BSEC_OUTPUT_CO2_EQUIVALENT,
        BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
        BSEC_OUTPUT_GAS_PERCENTAGE,
        BSEC_OUTPUT_COMPENSATED_GAS
    };

    //initialize & start bme688 object  
    if (!envSensor.begin(0x76, Wire)){
        checkBsecStatus(envSensor);
    }
	
	//the default offset provided has been determined by testing the sensor in 
    //ULP and LP mode (i.e. ultra low power mode and low power mode)
	if (SAMPLE_RATE == BSEC_SAMPLE_RATE_CONT){
	   envSensor.setTemperatureOffset(TEMP_OFFSET);
	}
	else if (SAMPLE_RATE == BSEC_SAMPLE_RATE_LP){
		envSensor.setTemperatureOffset(TEMP_OFFSET_LP);
	}

    //subsribe to the desired BSEC2 outputs 
    if (!envSensor.updateSubscription(sensorList, ARRAY_LEN(sensorList), SAMPLE_RATE)){
        checkBsecStatus(envSensor);
    }

    //whenever new data is available call the newDataCallback function 
    envSensor.attachCallback(newDataCallback);

  // initializes OLED display 
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
  }
  // OLED display library initializes this with an Adafruit splash screen.
  display.display();    //this function must be called at the end of display statements
  delay(2000);          // pauses for 2 seconds

  // clears the display buffer
  display.clearDisplay();
}

void loop(void){
    //Call the run function often so that the library can 
    //check if it is time to read new data from the sensor and process it.
    if (!envSensor.run()){
        checkBsecStatus(envSensor);
    }
}

void errLeds(void){
    while(1){
        digitalWrite(PANIC_LED, HIGH);
        delay(ERROR_DUR);
        digitalWrite(PANIC_LED, LOW);
        delay(ERROR_DUR);
    }
}

//this function is invoked by the BSEC library when a new output is available
//input     : BME68X sensor data before processing
//outputs   : Processed BSEC BSEC output data
//bsec      : Instance of BSEC2 calling the callback
void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec){
    if (!outputs.nOutputs){
        return;
    }
    
    Serial.println("BSEC outputs:\n\tTime stamp = " + String((int) (outputs.output[0].time_stamp / INT64_C(1000000))));
    for (uint8_t i = 0; i < outputs.nOutputs; i++){
        const bsecData output  = outputs.output[i];
        switch (output.sensor_id)
        {
            case BSEC_OUTPUT_IAQ:
                iaq = String(output.signal);
                iaqAccuracy = String((int) output.accuracy);
                Serial.println("\tIAQ = " + iaq);
                Serial.println("\tIAQ accuracy = " + iaqAccuracy);
                break;
            case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE:
                temp = String(output.signal -3.0);
                Serial.println("\tCompensated temperature = " + temp);
                break;
            case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY:
                humidity = String(output.signal);
                Serial.println("\tCompensated humidity = " + humidity);
                break;
            case  BSEC_OUTPUT_RAW_PRESSURE:
                pressure = String(output.signal/1000);
                Serial.println("\tCompensated humidity = " + pressure);
                break;
            //case BSEC_OUTPUT_STATIC_IAQ:
            //    Serial.println("\tStatic IAQ = " + String(output.signal));
            //    break;
            case BSEC_OUTPUT_CO2_EQUIVALENT:
                co2Eqv = String(output.signal);
                Serial.println("\tCO2 Equivalent = " + co2Eqv);
                break;
            case BSEC_OUTPUT_BREATH_VOC_EQUIVALENT:
                bVOCEqv = String(output.signal);
                Serial.println("\tbVOC equivalent = " + bVOCEqv);
                break;
            case BSEC_OUTPUT_GAS_PERCENTAGE:
                Serial.println("\tGas percentage = " + String(output.signal));
                break;
            case BSEC_OUTPUT_COMPENSATED_GAS:
                Serial.println("\tCompensated gas = " + String(output.signal));
                break;
            default:
                break;  
        }
        displayAirQualityData();
    }
}

//checks the BSEC status, prints the respective error code. Halts in case of error
void checkBsecStatus(Bsec2 bsec){
    if (bsec.status < BSEC_OK){
        Serial.println("BSEC error code : " + String(bsec.status));
        errLeds(); //Halt in case of failure 
    }
    else if (bsec.status > BSEC_OK){
        Serial.println("BSEC warning code : " + String(bsec.status));
    }
    if (bsec.sensor.status < BME68X_OK){
        Serial.println("BME68X error code : " + String(bsec.sensor.status));
        errLeds(); //Halt in case of failure 
    }
    else if (bsec.sensor.status > BME68X_OK){
        Serial.println("BME68X warning code : " + String(bsec.sensor.status));
    }
}

void displayAirQualityData(){
    display.clearDisplay();                 //clears OLED screen
    display.setTextSize(1);                 //Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);    //Draw white text
    display.setCursor(0,2); 

    display.print(F("IAQ ("));
    display.print(iaqAccuracy);
    display.print(F("):     "));
    display.println(iaq);
    
    display.setCursor(0,16); 
    display.print(F("Temp('C):    "));
    display.println(temp);

    display.setCursor(0,26);
    display.print(F("Humidity(%): "));
    display.println(humidity);

    display.setCursor(0,36);
    display.print(F("Pressur(kPa): "));
    display.println(pressure);

    display.setCursor(0,46);
    display.print(F("CO2 Eqv:    "));
    display.println(co2Eqv);

    display.setCursor(0,56);
    display.print(F("bVOC Eqv:     "));
    display.println(bVOCEqv);
    display.display();
}