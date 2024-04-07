#include <M5Stack.h>
#include <SoftwareSerial.h>

#include "Free_Fonts.h" 

SoftwareSerial mySerial(21,22); // RX, TX
unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;

void setup() {

  Serial.begin(9600);
  M5.begin();  // Initialize M5Stack
  M5.Power.begin();

  M5.Lcd.fillScreen(BLUE);
  header("::SML:: PM 2.5 Dectector", TFT_BLACK);

  M5.Lcd.fillRect(0, 30, 320, 140, TFT_WHITE);

  M5.Lcd.fillRect(0, 170, 160, 70, TFT_GREENYELLOW);
  M5.Lcd.fillRect(160, 170, 160, 70, TFT_PINK);

  M5.Lcd.setFreeFont(FSB12);
  M5.Lcd.setTextDatum(TC_DATUM);
  M5.Lcd.setTextColor(TFT_BLACK, TFT_GREENYELLOW);
  M5.Lcd.drawString("PM1", 80, 175, 4);
  
  M5.Lcd.setTextColor(TFT_BLACK, TFT_PINK);
  M5.Lcd.drawString("PM10", 240, 175, 4);

  Serial.println("Start done");
  while (!Serial) ;
  mySerial.begin(9600);



  Serial.println("Setup done");

}
 
void loop() {

  Serial.println("Loop Process");
  int index = 0;
  char value;
  char previousValue;
  if (mySerial.available() == false) Serial.print("Serial Not Avaliable");
  while (mySerial.available()) {
    value = mySerial.read();
    if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)){
      Serial.println("Cannot find the data header.");
      break;
    }
    
    if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
      previousValue = value;
    }
    else if (index == 5) {
      pm1 = 256 * previousValue + value;
      Serial.print("{ ");
      Serial.print("\"pm1\": ");
      Serial.print(pm1);
      Serial.print(" ug/m3");
      Serial.print(", ");
    }
    else if (index == 7) {
      pm2_5 = 256 * previousValue + value;
      Serial.print("\"pm2_5\": ");
      Serial.print(pm2_5);
      Serial.print(" ug/m3");
      Serial.print(", ");
    }
    else if (index == 9) {
      pm10 = 256 * previousValue + value;
      Serial.print("\"pm10\": ");
      Serial.print(pm10);
      Serial.print(" ug/m3");
    } else if (index > 15) {
      break;
    }
    index++;
  }


  while(mySerial.available()) mySerial.read();

  Serial.println(" }");

  M5.Lcd.setFreeFont(FSSBO24);
  M5.Lcd.setTextDatum(TC_DATUM);
  M5.Lcd.setTextColor(RED, WHITE);
  M5.Lcd.drawNumber(pm1, 160, 90);   // Display the integer 100 at (55,100)

  M5.Lcd.setFreeFont(FSB18);
  M5.Lcd.setTextColor(TFT_BLACK, TFT_GREENYELLOW);
  M5.Lcd.drawNumber(pm2_5, 80, 200); 

  M5.Lcd.setTextColor(TFT_BLACK, TFT_PINK);
  M5.Lcd.drawNumber(pm10, 240, 200); 

  delay(1000);
}

// Print the header for a display screen
void header(const char *string, uint16_t color) {
    M5.Lcd.fillScreen(color);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_MAGENTA, TFT_BLUE);
    M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLUE);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString(string, 160, 2,
                      4);  // Font 4 for fast drawing with background
}