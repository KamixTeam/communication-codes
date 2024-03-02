  #include <Arduino.h>
  #include "SSD1306Wire.h"      
  #define I2C_SDA 21                           // OLED PIN
  #define I2C_SCL 22                           // OLED PIN
  SSD1306Wire display(0x3c, I2C_SDA, I2C_SCL); // Ekran Tanımlama      // OLED ekran için kütüphane

int selected = 0;
int entered = -1;                      // OLED PIN


struct Button {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button1 = {13, 0, false};
Button button2 = {25, 0, false};
Button button3 = {38, 0, false};


void IRAM_ATTR isr(void* arg) {
    Button* s = static_cast<Button*>(arg);
    s->numberKeyPresses += 1;
    s->pressed = true;
}


void setup() {
    pinMode(button1.PIN, INPUT_PULLUP);
    pinMode(button2.PIN, INPUT_PULLUP);
    pinMode(button3.PIN, INPUT_PULLUP);

    Serial.begin(115200);
    display.init();
    display.flipScreenVertically(); 
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
}


void loop() {

  displaymenu();
}


void displaymenu(void) {

  int down = digitalRead(38);
  int enter = digitalRead(25);
  int back = digitalRead(13);

  if (down == LOW) {
    selected = selected + 1;//0
    if(selected == 2 ){
      selected = 0;
    }
    delay(200);
    
  };
  if (enter == LOW) {
    entered = selected;
  };
  if (back == LOW) {
    entered = -1;
  };
  const char *options[2] = {
    "Muhimmat",
    "Ilk Yardim"
  };

  if (entered == -1) {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    int setCursorCounter=0; // Menülerin alt alta sıralanması için
    //display.setCursor(0, 0);
    //display.drawString(F("Dsn menu test"));
    //display.drawString("");

    for (int i = 0; i < 2; i++) {
      if (i == selected) {//
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.setFont(ArialMT_Plain_16);
      display.setColor(INVERSE);
      display.fillRect(0, setCursorCounter, 128, 18);
      display.drawString(0, setCursorCounter,options[i]);
      } else if (i != selected) {
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.setFont(ArialMT_Plain_16);
      display.setColor(WHITE);
      display.drawString(0, setCursorCounter,options[i]);
      }
      setCursorCounter = setCursorCounter +18 ;
    }


    //Muhimmat Kodları Buraya Gelecek
  } else if (entered == 0) {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 18,"SELAMLAR");

  }
  //Ilk Yardım Kodları Buraya Gelecek 
  else if (entered == 1) {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawStringMaxWidth(0, 0, 128,"ALEYKUM SELAMLAR");
  }
   
  display.display();
}
