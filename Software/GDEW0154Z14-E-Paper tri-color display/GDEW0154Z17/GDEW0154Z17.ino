
// include library, include base class, make path known
#include <GxEPD.h>
#include <DHT.h> 
#include "thermometer.h"

#include <GxGDEW0154Z17/GxGDEW0154Z17.h>  // 1.54" b/w/r 152x152

#include GxEPD_BitmapExamples

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold12pt7b.h>
#define DHTPIN 2 // Pin where the DHT11 is connected
#define DHTTYPE DHT22 // DHT11 sensor type

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor
float temp = 0;
float hum = 0;


#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#if defined(ESP8266)

// for SPI pin definitions see e.g.:
// C:\Users\xxx\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\2.4.2\variants\generic\common.h

GxIO_Class io(SPI, /*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=D4*/ 2, /*BUSY=D2*/ 4); // default selection of D4(=2), D2(=4)
// Heltec E-Paper 1.54" b/w without RST, BUSY
//GxEPD_Class display(io, /*RST=D4*/ -1, /*BUSY=D2*/ -1); // no RST, no BUSY
// Waveshare e-Paper ESP8266 Driver Board
//GxIO_Class io(SPI, 15, 4, 5);
//GxEPD_Class display(io, 5, 16);

#elif defined(ESP32)

// for SPI pin definitions see e.g.:
// C:\Users\xxx\Documents\Arduino\hardware\espressif\esp32\variants\lolin32\pins_arduino.h

GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 16, /*BUSY=*/ 4); // arbitrary selection of (16), 4
// for LILYGO® TTGO T5 2.66 board uncomment next two lines instead of previous two lines
//GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 19, /*RST=*/ 4); // LILYGO® TTGO T5 2.66
//GxEPD_Class display(io, /*RST=*/ 4, /*BUSY=*/ 34); // LILYGO® TTGO T5 2.66

#elif defined(ARDUINO_ARCH_SAMD)

// for SPI pin definitions see e.g.:
// C:\Users\xxx\AppData\Local\Arduino15\packages\arduino\hardware\samd\1.6.19\variants\mkr1000\variant.h
// C:\Users\xxx\AppData\Local\Arduino15\packages\arduino\hardware\samd\1.6.19\variants\mkrzero\variant.h

GxIO_Class io(SPI, /*CS=*/ 4, /*DC=*/ 7, /*RST=*/ 6);
GxEPD_Class display(io, /*RST=*/ 6, /*BUSY=*/ 5);

#elif defined(ARDUINO_GENERIC_STM32F103C) && defined(MCU_STM32F103C8)

// STM32 Boards(STM32duino.com) Generic STM32F103C series STM32F103C8
// for SPI pin definitions see e.g.:
// C:\Users\xxx\Documents\Arduino\hardware\Arduino_STM32\STM32F1\variants\generic_stm32f103c\variant.h
// C:\Users\xxx\Documents\Arduino\hardware\Arduino_STM32\STM32F1\variants\generic_stm32f103c\board\board.h

// new mapping suggestion for STM32F1, e.g. STM32F103C8T6 "BluePill"
// BUSY -> A1, RST -> A2, DC -> A3, CS-> A4, CLK -> A5, DIN -> A7

GxIO_Class io(SPI, /*CS=*/ SS, /*DC=*/ 3, /*RST=*/ 2);
GxEPD_Class display(io, /*RST=*/ 2, /*BUSY=*/ 1);

#elif defined(ARDUINO_GENERIC_STM32F103V) && defined(MCU_STM32F103VB)

// STM32 Boards(STM32duino.com) Generic STM32F103V series STM32F103VB
// for SPI pin definitions see e.g.:
// C:\Users\xxx\Documents\Arduino\hardware\Arduino_STM32\STM32F1\variants\generic_stm32f103vb\variant.h
// C:\Users\xxx\Documents\Arduino\hardware\Arduino_STM32\STM32F1\variants\generic_stm32f103vb\board\board.h

// Good Display DESPI-M01
// note: needs jumper wires from SS=PA4->CS, SCK=PA5->SCK, MOSI=PA7->SDI

GxIO_Class io(SPI, /*CS=*/ SS, /*DC=*/ PE15, /*RST=*/ PE14); // DC, RST as wired by DESPI-M01
GxEPD_Class display(io, /*RST=*/ PE14, /*BUSY=*/ PE13); // RST, BUSY as wired by DESPI-M01

#elif defined(ARDUINO_AVR_MEGA2560)

// for SPI pin definitions see e.g.:
// C:\Users\xxx\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.6.21\variants\mega\pins_arduino.h

// select one, depending on your CS connection
GxIO_Class io(SPI, /*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9); // arbitrary selection of 8, 9 selected for default of GxEPD_Class
//GxIO_Class io(SPI, /*CS=*/ 10, /*DC=*/ 8, /*RST=*/ 9); // arbitrary selection of 8, 9, CS on 10 (for CS same as on UNO, for SPI on ICSP use)

GxEPD_Class display(io, /*RST=*/ 9, /*BUSY=*/ 7); // default selection of (9), 7

#else

// for SPI pin definitions see e.g.:
// C:\Users\xxx\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.6.21\variants\standard\pins_arduino.h

GxIO_Class io(SPI, /*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9); // arbitrary selection of 8, 9 selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=*/ 9, /*BUSY=*/ 7); // default selection of (9), 7

#endif




void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  dht.begin();

  display.init(115200); // enable diagnostic output on Serial

#if defined(__AVR) || false
  display.drawPaged(drawHelloWorld);
#elif (defined(_GxGDEW075Z09_H_) || defined(_GxGDEW075Z08_H_)) && (defined(ESP8266) || defined(ARDUINO_ARCH_STM32F1))
  display.drawPaged(drawHelloWorld);
#elif defined(_GxGDEW075Z09_H_)
  display.drawPaged(drawHelloWorld);
#else
  
  TempnHum();
  display.update();
#endif
  display.powerDown();

  Serial.println("setup done");
}

void loop() {
 temp = dht.readTemperature();
 hum = dht.readHumidity();


  if (isnan(temp) || isnan(hum)) {
  Serial.println("Failed to read from DHT sensor!");
      return;
  }
};

void TempnHum(){

 temp = dht.readTemperature();
 hum = dht.readHumidity();

  
  display.setRotation(3);
  display.drawExampleBitmap(epd_bitmap_image__1_, 5, 50, 54, 80, GxEPD_RED);
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0, 25); 
  display.println("Temp & Hum:");
  display.setCursor(70, 80);
  display.print(temp, 1);
  display.println("C");
  display.setCursor(70, 110);
  display.print(hum, 1);
  display.println("%");
  prevValue1 = temp;
  prevValue2 = hum;
  

}
