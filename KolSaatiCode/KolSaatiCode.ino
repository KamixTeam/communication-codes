////////////////////////////////KOL SAATİ BU OLACAK YANİ VERİLERİ BU GÖNDERECEK /////////////////////

///////Kütüphaneler////////
#define BUTTON_PIN 38                /// Buton Kütüphanesi
#define BUTTON_PIN_MASK GPIO_SEL_38  //
#include <SPI.h>                     // Lora için kütüphane
#include <LoRa.h>                    // Lora için kütüphane
#include "SSD1306Wire.h"             // OLED ekran için kütüphane
#include <TinyGPS++.h>               //GPS için kütüphane
#include <HardwareSerial.h>          //GPS için kütüphane
#include <Arduino.h>                 //Buton için Kütüphane

///////OLED TANIMLAMA////////
#define I2C_SDA 21                            // OLED PIN
#define I2C_SCL 22                            // OLED PIN
SSD1306Wire display(0x3c, I2C_SDA, I2C_SCL);  // Ekran Tanımlama

///////LORA TANIMLAMA////////
#define SCK 5
#define MISO 19
#define MOSI 27
#define RADIO_CS_PIN 18
#define RADIO_DI0_PIN 26
#define RADIO_RST_PIN 14
#define BAND 866E6  // GEÇERLİ FREKANS ARALIĞI

///////GPS TANIMLAMA////////
#define RXD2 34
#define TXD2 12
#define GPS_SERIAL_NUM 1
TinyGPSPlus gps;
HardwareSerial GPSSerial(GPS_SERIAL_NUM);

// LORA_GPS Struct
struct Data {
  double lat;
  double lng;
  char z[10];
} data;

// Buton Struct
struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};
//////BUTON TANIMLAMA////
Button button1 = { 13, 0, false };
Button button2 = { 25, 0, false };
Button button3 = { 38, 0, false };

// Ana Sayfada Dolanmak ve Seçmek İçin Belirleyiciler//
int selected = 0;
int entered = -1;

void setup() {
  initButon();
  Serial.begin(115200);
  initOLED();
  initLora();
  initGPS();
}

void loop() {
  displaymenu();
}

/////////////////Lora - OLED - GPS ÇALIŞTIIRMAK İÇ GEREKLİ İŞLMELER/////////////
void initLora() {  // LORA TANIMLANDI BAŞLAMASI İÇİN

  SPI.begin(SCK, MISO, MOSI, RADIO_CS_PIN);
  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DI0_PIN);  // PIN'LER TANIMLANDI

  if (!LoRa.begin(BAND)) {  // FREKANS ARALIĞINDA ÇALIŞMADIĞI DURUMDA HATA VERİYOR
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
  Serial.println("LoRa Initializing OK!");
}

void initOLED() {  // OLED EKRAN TANIMLANDI BAŞLAMASI İÇİN
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void initGPS()  // GPS TANIMLANMASI İÇİN
{
  GPSSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

///////////////////////////GPS verilerini düzenleyinp Lora ile gönderme
void gpsdataSet(char decider) {
  Data data = { gps.location.lat(), gps.location.lng(), decider };
  // Gönderilecek veri Paketi
  //Serial.print(decider);                                                    // Oled ekranın değişmesi için gerekli olanlar
  while (GPSSerial.available() > 0) {
    if (gps.encode(GPSSerial.read())) {
      data.lat = 41.0378216879;
      data.lng = 40.4932071245;
      sendLoRaData(data);  // DATA GÖNDERME
    }
  }
}

/// LORA VERİ GÖNDERME////
void sendLoRaData(Data data) {
  Serial.print("\n Lokasyon Sending packet: ");
  Serial.print("lat = ");
  //data.lat = 41.0378216879;
  //data.lng = 40.4932071245;
  //data.z[0] = 'M';
  Serial.print(data.lat, 10);
  Serial.print(" lng = ");
  Serial.print(data.lng, 10);
  Serial.print(" z = ");
  Serial.print(data.z);
  LoRa.beginPacket();
  for (unsigned int i = 0; i < sizeof(Data); i++) {
    Serial.print(' ');
    LoRa.write(((byte *)&data)[i]);
  }
  LoRa.endPacket();

  delay(200);
}

/// OLED için gerekli bileşenleri barındırır////////////
void setOLED() {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
}

void initButon() {
  pinMode(button1.PIN, INPUT_PULLUP);
  pinMode(button2.PIN, INPUT_PULLUP);
  pinMode(button3.PIN, INPUT_PULLUP);
}

// BUTON BASMA ANASAYFA//
void displaymenu(void) {

  int down = digitalRead(38);
  int enter = digitalRead(25);
  int back = digitalRead(13);

  if (down == LOW) {
    selected = selected + 1;  //0
    if (selected == 2) {
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
    setOLED();
    int setCursorCounter = 0;  // Menülerin alt alta sıralanması için
    //display.setCursor(0, 0);
    //display.drawString(F("Dsn menu test"));
    //display.drawString("");

    for (int i = 0; i < 2; i++) {
      if (i == selected) {  //
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.setFont(ArialMT_Plain_16);
        display.setColor(INVERSE);
        display.fillRect(0, setCursorCounter, 128, 18);
        display.drawString(0, setCursorCounter, options[i]);
      } else if (i != selected) {
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.setFont(ArialMT_Plain_16);
        display.setColor(WHITE);
        display.drawString(0, setCursorCounter, options[i]);
      }
      setCursorCounter = setCursorCounter + 18;
    }


    //Muhimmat Kodları Buraya Gelecek
  } else if (entered == 0) {
    setOLED();
    display.drawString(0, 18, "SELAMLAR");
    gpsdataSet('M');

  }
  //Ilk Yardım Kodları Buraya Gelecek
  else if (entered == 1) {
    setOLED();
    display.drawStringMaxWidth(0, 0, 128, "ALEYKUM SELAMLAR");
    gpsdataSet('I');
  }

  display.display();
}
