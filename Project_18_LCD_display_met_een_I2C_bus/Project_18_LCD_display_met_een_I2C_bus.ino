/*
  Demonstration sketch for Adafruit i2c/SPI LCD backpack
  using MCP23008 I2C expander
  ( http://www.ladyada.net/products/i2cspilcdbackpack/index.html )

  The circuit:
   5V to Arduino 5V pin
   GND to Arduino GND pin
   CLK to Analog #5 (Geel)
   DAT to Analog #4 (Oranje)
*/

// include the library code:
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

Adafruit_LiquidCrystal lcd(0);

void setup() {
  lcd.begin(16, 2);
}

char Text[] = "                Dit is de tekst die we heen en weer willen laten scrollen                ";

void loop() {

  // Hallo World!
  lcd.clear(); //Clear is geen snel kommando. Soms is het sneller om over je tekst gewoon een lege regel te schrijven.
  lcd.print("Hallo World!");
  delay(2000);

  for (int i=0; i<strlen(Text)-16; i++) {
    lcd.setCursor(0,1);
    lcd.print(Text+i);
    delay(300);
  }

  // Het displayen van een veranderend getal
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Een getal.....");
  for (int k = 0; k <= 100; k++) {
    lcd.setCursor(0, 1);
    lcd.print(k);
    delay(10);
  }
  delay(2000);


  // Het printen van een twee-regelige tekst
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Regel 1: ");
  lcd.setCursor(0, 1);
  lcd.print("Regel 2: ");
  delay(2000);


  // Het gebruik van noDisplay en display
  lcd.noDisplay();   // Turn off the display:
  delay(500);
  lcd.display();    // Turn on the display:
  delay(500);
  lcd.noDisplay();   // Turn off the display:
  delay(500);
  lcd.display();    // Turn on the display:
  delay(500);


  // Het gebruik van left and right scrolling
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Left and right scrolling");
  delay(1000);
  for (int k = 0; k <= 47; k++) {
    lcd.scrollDisplayLeft(); //Elke keer dat dit wordt gebruikt springt de tekst één positie naar links.
    //     lcd.scrollDisplayRight(); //Elke keer dat dit wordt gebruikt springt de tekst één positie naar rechts.
    delay(300);
  }
  for (int k = 0; k <= 47; k++) {
    lcd.scrollDisplayRight(); //Elke keer dat dit wordt gebruikt springt de tekst één positie naar links.
    delay(300);
  }
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Graden teken");
  lcd.setCursor(0, 1);
  lcd.print(char(223));
  lcd.print("C");
  delay(2000);
}




