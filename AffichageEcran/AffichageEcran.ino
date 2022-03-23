#include <Wire.h>
#include "rgb_lcd.h"
#include <Adafruit_BME280.h>                        

//Constante écran 
rgb_lcd lcd;
 
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

//Constante Capteur 

#define adresseI2CduBME280                0x76            // Adresse I2C du capteur
#define pressionAuNiveauDeLaMerEnHpa      1024.90    
#define delaiRafraichissementAffichage    1500            // Délai de rafraîchissement de l'affichage (en millisecondes)

Adafruit_BME280 bme;
void setup() 
{
  Serial.print(F("Initialisation du BME280, à l'adresse [0x"));
  Serial.print(adresseI2CduBME280, HEX);
  Serial.println(F("]"));
  if(!bme.begin(adresseI2CduBME280)) {
    Serial.println(F("--> ÉCHEC…"));
  } 
  else {
    Serial.println(F("--> RÉUSSIE !"));
  }
  Serial.println();
  
  //setup ecran
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    Serial.begin(9600);
    while(!Serial);
    Serial.println();
 
    lcd.setRGB(colorR, colorG, colorB);
 
    // Print a message to the LCD. 
    delay(1000);
    //setup capteur
}
 
void loop() 
{
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    float t=bme.readTemperature();
    lcd.print(t); 
    delay(100);
    Serial.print(F("Température = "));
    Serial.print(bme.readTemperature());
    Serial.println(F(" °C"));

}
