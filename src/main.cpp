#include    <Arduino.h>
#include    <SPI.h>
#include    <SPI.h>
#include    <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include    "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

#define     OLED_SCL       22
#define     OLED_SDA       21

SSD1306Wire  display(0x3c, OLED_SDA, OLED_SCL);

#define ntc_pin A10         // Pin,to which the voltage divider is connected
#define vd_power_pin 23        // 5V for the voltage divider
#define nominal_resistance 100000       //Nominal resistance at 25⁰C
#define nominal_temeprature 25   // temperature for nominal resistance (almost always 25⁰ C)
#define samplingrate 5    // Number of samples
#define beta 3950  // The beta coefficient or the B value of the thermistor (usually 3000-4000) check the datasheet for the accurate value.
#define Rref 100000   //Value of  resistor used for the voltage divider
int samples = 0;   //array to store the samples
void setup(void) {
  pinMode(vd_power_pin, OUTPUT);
  display.init();
  display.flipScreenVertically();

  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.clear();
}

void loop(void) {
  uint8_t i;
  float average;
  samples = 0;
  // take voltage readings from the voltage divider
  digitalWrite(vd_power_pin, HIGH);
  for (i = 0; i < samplingrate; i++) {
    samples += analogRead(ntc_pin);
                                                                                                                                                    
    delay(10);
  }
  digitalWrite(vd_power_pin, LOW);
  average = 0;
  average = samples / samplingrate;


  average = 4095 / average - 1;

  average = Rref / average;

  float temperature;
  temperature = average / nominal_resistance;     // (R/Ro)
  temperature = log(temperature);                  // ln(R/Ro)
  temperature /= beta;                   // 1/B * ln(R/Ro)
  temperature += 1.0 / (nominal_temeprature + 273.15); // + (1/To)
  temperature = 1.0 / temperature;                 // Invert
  temperature -= 273.15;                         // convert absolute temp to C

  String displayedstring;
  displayedstring = String(temperature);
  int16_t offset = 34;
  if (displayedstring.length() == 6) {
    offset = 24;
  }
  if (displayedstring.length() == 5) {
    offset = 34;
  }
  int16_t offset2 = 81;
  if (displayedstring.length() == 6) {
    offset2 = 84;
  }
  if (displayedstring.length() == 5) {
    offset2 = 81;
  }

  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(offset, 18, displayedstring.substring(0, displayedstring.length() - 1));
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(offset2, 19, "C");
  display.display();
  delay(250);
  display.clear();
}