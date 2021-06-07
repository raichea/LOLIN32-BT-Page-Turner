/*******************************************************************
 *  A simple Macro keyboard built with an ESP32 and a        
 *  4*4 Button Matrix.
 *  
 *  I use a TinyPICO ESP32 for this project, but you could
 *  use any ESP32, just adjust the row and col pins if
 *  required.
 *  
 *  Parts:
 *  TinyPICO ESP32 - https://www.tinypico.com/
 *  4*4 Keypad Button Matrix* - http://s.click.aliexpress.com/e/CqnjTgGg 
 *  
 *  * = Affilate
 *  
 *  If you find what I do useful and would like to support me,
 *  please consider becoming a sponsor on Github
 *  https://github.com/sponsors/witnessmenow/
 *                                                                 
 *  Written by Brian Lough
 *  YouTube: https://www.youtube.com/brianlough
 *  Tindie: https://www.tindie.com/stores/brianlough/
 *  Twitter: https://twitter.com/witnessmenow                                 
 *******************************************************************/
#include <Arduino.h>

// OFF is LOW for DOIT board, HIGH for LOLIN32, opposite for ON
#define OFF HIGH
#define ON LOW

#include <BleKeyboard.h>
#include <Keypad.h>

const byte BLE_DELAY = 10;  // Delay to prevent BT congestion

const byte battPin = 34;    // 1/2 the battery voltage read on this pin

// Could do this without the keypad library fairly simply but this allows easy expansion
const byte ROWS = 1; // Just the one row
const byte COLS = 2; // Two columns

// the library will return the character inside this array
// when the appropriate button is pressed.
char keys[ROWS][COLS] = {
//  {'1', '2', '3', '4'}
  {'1', '2'}
};

// Looking at the keypad from the front, the row pins are on the left.

byte rowPins[ROWS] = {4};      // Connect to the row pinouts of the keypad
byte colPins[COLS] = {16, 17}; // Connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// First param is name, second is manufacturer, third is initial battery level
BleKeyboard bleKeyboard("BTPageTurner", "SteveRussell", 100);

void ledState(byte state) {
  digitalWrite(LED_BUILTIN, state);
}

void setup() {
  // Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  ledState(ON);   // turn the LED on to show looking for a BT connection
  bleKeyboard.begin();
}

bool isBatteryLow() {
  const byte SAMPLE_COUNT = 16;
  const int HI_VOLTAGE = 2200;    // About 3.7V
  const int LOW_VOLTAGE = 1750;   // About 3.0V

  static int samples[SAMPLE_COUNT];
  static byte ptr = 0;
  static unsigned long updateTimer = 0;

  int batteryAvg = 0;

  // Get a reading from the ADC
  samples[ptr++] = analogRead(battPin);
  ptr &= 0x0F;

  // Average the collected samples
  for (byte i = 0; i < SAMPLE_COUNT; i++){
    batteryAvg += samples[i];
  }
  batteryAvg += SAMPLE_COUNT / 2;  // Make sure we round correctly
  batteryAvg /= SAMPLE_COUNT;
  // Serial.printf("Battery: %d\n", batteryAvg);

  if (millis() - updateTimer > 1000) {
    bleKeyboard.setBatteryLevel(batteryAvg >= HI_VOLTAGE ? 100 : 10 + 90 * (batteryAvg - LOW_VOLTAGE) / (HI_VOLTAGE - LOW_VOLTAGE));
    delay(BLE_DELAY);
    // Serial.printf("Battery: %d%%\n", 10 + 90 * (batteryAvg - LOW_VOLTAGE) / (HI_VOLTAGE - LOW_VOLTAGE));
    updateTimer = millis();
  }

  return batteryAvg <= LOW_VOLTAGE ? true : false;
}

void loop() {
  static unsigned long flashTimer = 0;

  unsigned long current;
  bool flashLed = false;

  current = millis();
  if (isBatteryLow()){
    if(current - flashTimer > 1000)
      flashTimer = millis();
  }  
  else { // Force any current flash off if battery recovers
    flashTimer = 0; 
  }  

  flashLed = (current - flashTimer >= 900) && (current - flashTimer <= 1000) ? true : false;
  if (bleKeyboard.isConnected()) {
    char key = keypad.getKey();
    // Turn the LED off to show we have a BT connection - brief flash on if battery low
    ledState(flashLed ? ON : OFF);   
    // Serial.println(key);
    switch (key) {
      case '1':
        bleKeyboard.press(KEY_DOWN_ARROW);  // Left pedal
        break;
      case '2':
        bleKeyboard.press(KEY_UP_ARROW);    // Right pedal
        break;
      case NO_KEY:
      default:
        break;
    }

    delay(BLE_DELAY);
    bleKeyboard.releaseAll(); // Release the keys
    delay(BLE_DELAY);
  }
  else{
    // Turn the LED on to show we're looking for a BT connection - brief flash off if battery low
    ledState(flashLed ? OFF : ON);   
  }
}
