#include <SPI.h> // Libairie communication SPI avec le Shield éthernet
#include <Ethernet.h> // Librairie communication Ethernet sur un réseau local
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
//On donne l'adresse MAC au shield Ethernet (ne doit pas être identique à une autre adresse MAC présente sur votre réseau)
byte MAC[6] = { 0xA8, 0x61, 0x0A, 0xAE, 0x17, 0x0E }; // Adresse MAC du shield Ethernet
String request; 
//On affecte l'adresse IP du Shield Ethernet
IPAddress IP(169,254,0,1 ); // Adresse IP donné au Shield Ethernet

// On attribue la fonction serveur au Shield Ethernet sur le port 80
EthernetServer server(80); // Le Shield devient un serveur sur le port 80 (port HTTP)
EthernetClient client;
void setup()
  {

    bme.begin(adresseI2CduBME280);
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    
  //Lancement connexion Ethernet avec l'adresse MAC et l'adresse IP
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  Ethernet.begin(MAC, IP);
  pinMode(2,OUTPUT);
  server.begin();
  Serial.println(F("W5500 initialized"));
  Serial.print(F("IP Address: "));
  Serial.println(Ethernet.localIP()); 
  }

void loop(){
 client = server.available();

  
 clientRequest();
 handleRequest();
 delay(500);
 
  }

  void clientRequest() {
 if (!client) {
   return;
 }
 while (!client.available()) {
   delay(1);
 }
 request = client.readStringUntil('\r');
 Serial.println(request);
 client.flush();
 
}
void handleRequest( ) { 
 if (request.indexOf("/dig2on") > 0) {
   digitalWrite(2, HIGH);
 }
 if (request.indexOf("/dig2off") > 0) {
   digitalWrite(2, LOW);
 }
 if (request.indexOf("GET") >= 0) {
   webpage(client);
   client.stop();
 }
}
void webpage(EthernetClient client) { /* function webpage */
 lcd.setCursor(0, 1);
 float t=bme.readTemperature();

 lcd.print(t);
 client.println(F("HTTP/1.1 200 OK"));
 client.println(F("Content-Type: text/html"));
 client.println();

 client.print(F("<!DOCTYPE HTML><html><head><title>Variateur d'ambiance</title>"));
 client.print(F("<meta http-equiv='content-type' content='text/html; charset=UTF-8'/>"));
 client.print(F("<meta http-equiv='refresh' content='5'/></head><body bgcolor='black'>"));
 client.print(F("<br><hr/><hr><h1 style='color : #3AAA35;'><center> Paramètres du Variateur d'ambiance </center></h1>"));
 client.print(F("<hr/><hr><h2 style='color:green;'>Données récoltées : </h2>"));
 client.print(F("<h1 style='color : #3AAA35;'><center> Paramètres du Variateur d'ambiance </center></h1>"));
 client.print(F("<p style='color:white;'><ul><li style ='color:white;'>Température relevée : "));
 client.print(t);
 client.print(F("°C</li>"));

 client.print(F("<li style ='color:white;'>Musique actuellement jouée : blabla</li><li style ='color:white;'>texte affiché : blabla</li>"));
 client.print(F("</ul></p><br><h2 style='color:green;'>Configuration actuelle : </h2>"));
 client.print(F("<ul><li style ='color:white;'>Prochaine Musique : </li></ul>"));
 client.print(F("<br><h2 style='color:green;'>Paramètres : </h2>"));
 client.print(F("<p style='color:white;'><br><br><b>Nouveau texte à afficher</b>"));
 client.print(F("</p><br>"));
 client.print(F("<h2 style='color:green;'>Paramètres : </h2>"));
 client.print(F("<p style='color:white;'>"));

 client.print(F("<input ></input><a href='/dig2on'><button>Turn On </button></a><a href='/dig2off'><button>Turn Off </button></a><br />"));
 client.print(F("</p><br><br></body></html>"));
 
 delay(1);
}
