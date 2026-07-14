#include <Arduino.h>

#include <PDM.h>
#include "Wire.h"
#include "SPI.h"
#include "PicoWav.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SD.h"

//define PDM mic parameters
#define BIT_PER_SAMPLE 16

//define OLED screen dimensions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # 
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

//define length of recorded clips (60000 milliseconds = 1 min) 
#define RECORDING_TIME 60000     //*** RECORDING TIME for each in milliseconds ****
                                 //*** Change time to suit your application

#define FILE_BASE_NAME "recFile" //base name for audio files

//buttons for controlling recording
#define REDButton   12          //connected to pin GP12 //6
#define BLACKButton 13          //connected to pin GP13 //7
#define NSS 8                   //select pin for LoRa module (pin GP 8)
#define CS1 17                  //select pin for the SD card (pin GP17)

//create OLED display object "display"
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//default number of output channels
static const char channels = 1;

//default PCM output frequency or sample rate
static const int frequency = 16000;     

// variables for tracking recording
float duration = RECORDING_TIME/1000;  //***RECORDING DURATION IN SECs***
unsigned long startMillis = 0;         //start-time for recording
int recordingInterrupted = 0;

//Buffer to read samples into, each sample is 16-bits
short sampleBuffer[256]; 

//Number of audio samples read
volatile int samplesRead;

//track recording status
int recordingStatus = 0;
unsigned long recordingStart;

//file parameters
char fileName[] = FILE_BASE_NAME "00.wav";
const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
File audio_file;

//define and initialize device states
enum State {
    WAITING,
    OPEN_NEW_FILE,
    RUNNING,
    FINISHED
};
State state = WAITING;       //initialise state

//create PDM microphone object with pin GP2 as data input and pin GP3 as clock output
PDMClass PDM(2,3);
//create .wav object using filename, duration, channel, sampling freq & bits/sample
PicoWav picoWav(fileName, duration, 1, frequency, BIT_PER_SAMPLE);

//declare implemented functions
void initializePDMic();
void onPDMdata();
void redButtonPressed();
void blackButtonPressed();
bool openNewFile();
void recordAudioOnSD();
void displayStartup();
void displayFailedSDCardMsg();

void displayRecording();
void displayStopRecording();

void setup() {
  Wire.begin();
  Serial.begin(115200);
  

  pinMode(CS1, OUTPUT);       //set select for SD card as output
  pinMode(NSS, OUTPUT);       //set select for LoRa module as output
  digitalWrite(CS1,LOW);      //enable SD card by pulling select pin LOW
  digitalWrite(NSS,HIGH);     //disable LoRa module by pulling select pin HIGH

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);                  //don't proceed, loop forever
  }
  //the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000);                //pause for 2 seconds

  //set REDButton and BLACKButton pins as INPUT (i.e., read pins)
  //both input pins are pulled LOW by default using 10k resistors
  pinMode(REDButton, INPUT);
  pinMode(BLACKButton, INPUT);
  
  //attach an interrupt to each button. If the state of the button CHANGEs (i.e., is momentarily pressed)
  //invoke interrupt processing function - redButtonPressed() for REDButton and 
  //blackButtonPressed() for BLACKButton
  attachInterrupt(REDButton, redButtonPressed, CHANGE);
  attachInterrupt(BLACKButton, blackButtonPressed, CHANGE);

  display.clearDisplay();                 //clears OLED screen
  display.setTextSize(1);                 //Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);    //Draw white text
  display.setCursor(0,10);  
 
  initializePDMic();                      //initialize PDM microphone

  if(!SD.begin(CS1)) {
    //start access to the SD card
    Serial.println();
    Serial.println("Card Mount Failed");
    displayFailedSDCardMsg();
    while (1);
  }
  displayStartup();
}

void initializePDMic(){

  //configure the data receive callback
  PDM.onReceive(onPDMdata);

  //initialize PDM mic with one channel (mono mode), 16 kHz sample rate 
  if (!PDM.begin(channels, frequency)) {
    Serial.println("Failed to start PDM!");
    display.print(F("Failed to start PDM!"));
    display.display();
    while (1);
  }
   //optionally set the gain (0-60) defaults to 20 
  PDM.setGain(15);   
}

void loop(){
  switch (state){
    case WAITING:
        //waiting for the user to press the RED button. 
        break;

    case OPEN_NEW_FILE:
        //open and prep new audio file
        if (openNewFile()){
            displayRecording();
            recordingStart = millis(); // set recording start time
            state = RUNNING;           //move to 'RUNNING' state and start recording
        }
        else{
            displayFailedSDCardMsg();
            state = WAITING;
        }
        break;

    case RUNNING:
       //wait for audio samples to be read
      if (samplesRead) {
        audio_file.write((uint8_t *)sampleBuffer,128);
      }
      samplesRead = 0;  //clear the read count
      if ((millis() - recordingStart >= RECORDING_TIME)||(recordingInterrupted == 1)){
            state = FINISHED;
      }
      break;

    case FINISHED:
        //close file and go to WAITING state
        recordingInterrupted = 0;
        audio_file.close();
        displayStopRecording();
        state = WAITING;
        break;
    }
}

//callback function to process the data from the PDM microphone.
//please note that this callback is executed as part of an ISR.
//therefore using `Serial` to print messages inside this function isn't supported.
void onPDMdata() {
  // Query the number of available bytes
  int bytesAvailable = PDM.available();

  // Read into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);
  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
}

//RED button handler is used to set the state of recording. Application is initially
//in 'WAITING' state, but moves to 'OPEN_NEW_FILE' state when button is pressed
void redButtonPressed(){
    switch (state){
      case WAITING:
        delay(1000);
        state = OPEN_NEW_FILE;
        break;
    }
}

//BLACK button handler used to set the state of recording. Application is initially
//in 'RUNNING' state, but moves to 'FINISHED' state when button is pressed
void blackButtonPressed(){
  if (recordingInterrupted == 0){
      recordingInterrupted = 1;
      state = FINISHED;
  }
}

//attempst to create a new file and returns true if successful
bool openNewFile(){
  bool success = true;
  //create new filename - Names are sequenced from filename00 to filaname99
   while (SD.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      Serial.println(F("Can't create file name"));
      success = false;
    }
  }
  //creates .wav header for file
  if(!picoWav.begin()){
     Serial.println("Unable to create header for .wav file");
    success = false;
  }

  //open created .wav file in append+binary mode to add PCM data
  audio_file = SD.open(fileName, FILE_WRITE);
  if (audio_file == NULL) {
    Serial.println("Failed to create .wav file");
    success = false;
  }

  return success;
}

//records audio onto SD card
void recordAudioOnSD(){
   //data size in bytes - > this amount of data should be recorded from microphone
  uint32_t data_size = frequency * BIT_PER_SAMPLE * duration / 8;

  startMillis = millis();    //set recording start time ...
  while((millis() - startMillis)<= RECORDING_TIME) {             
    //wait for audio samples to be read
    if (samplesRead) {
      audio_file.write((uint8_t *)sampleBuffer,128);
    }
     samplesRead = 0;  //clear the read count
  }
  recordingStatus = 0;
  void displayStopRecording();
  audio_file.close();
}

void displayStartup(){
    display.clearDisplay();
    display.setTextSize(1); // Normal 1:1 pixel scale
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println(F("Press RED button to"));
    display.println(F("start recording..."));
    display.setCursor(0, 35);
    display.println(F("Max length of clip"));
    display.print(F("will be: "));
    display.print(duration);
    display.println(F(" secs"));

    // OLED display library initializes this with an Adafruit splash screen.
    display.display(); // this function must be called at the end of display statements
    delay(2000);
}

void displayFailedSDCardMsg(){
    display.clearDisplay(); // clears OLED screen
    display.setTextSize(1); // Normal 1:1 pixel scale
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.println(F("Failed to open file"));
    display.println(F("please check that you"));
    display.println(F("have a valid SD card"));
    display.println(F("in card slot"));
    display.display();
}

void displayRecording(){
    //DateTime now = rtc.now();
    display.clearDisplay(); // clears OLED screen
    display.setTextSize(1); // Normal 1:1 pixel scale
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
  
    display.println(F("Recording ...."));
    display.setCursor(0, 20);
    display.println(F("Press BLACK button to"));
    display.println(F("STOP recording"));
    display.display();
}

void displayStopRecording(){
    display.clearDisplay(); // clears OLED screen
    display.setTextSize(1); // Normal 1:1 pixel scale
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(F("Recording completed "));
    display.println(F(""));
    display.println(F("Press RED button to"));
    display.println(F("record new clip"));
    display.display();
}

