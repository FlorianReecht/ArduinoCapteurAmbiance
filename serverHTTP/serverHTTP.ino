#include <SPI.h> // Libairie communication SPI avec le Shield éthernet
#include <Ethernet.h> // Librairie communication Ethernet sur un réseau local

//On donne l'adresse MAC au shield Ethernet (ne doit pas être identique à une autre adresse MAC présente sur votre réseau)
byte MAC[6] = { 0xA8, 0x61, 0x0A, 0xAE, 0x17, 0x0E }; // Adresse MAC du shield Ethernet
String request; 
unsigned long refreshCounter = 0;
//On affecte l'adresse IP du Shield Ethernet
IPAddress IP(169,254,152, 92); // Adresse IP donné au Shield Ethernet

// On attribue la fonction serveur au Shield Ethernet sur le port 80
EthernetServer server(80); // Le Shield devient un serveur sur le port 80 (port HTTP)
EthernetClient client;
void setup()
  {
    
  //Lancement connexion Ethernet avec l'adresse MAC et l'adresse IP
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  Ethernet.begin(MAC, IP);
//  while (!Ethernet.begin(MAC)) {
//    Serial.println(F("failed. Retrying in 1 second."));
//    delay(1000);
//    Serial.print(F("Starting W5500..."));
//  }
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
  }

  void clientRequest( ) { /* function clientRequest */
 ////Get client request
 if (!client) {
   return;
 }
 // Wait until the client sends some data
 while (!client.available()) {
   delay(1);
 }
 request = client.readStringUntil('\r'); // Read the first line of the request
 Serial.println(request);
 client.flush();
}client.print(F("<!DOCTYPE HTML><html><head><title>Variateur d'ambiance</title>"));
 client.print(F("<meta http-equiv='content-type' content='text/html; charset=UTF-8'>"));
 //meta-refresh page every x seconds
 client.print(F("<meta http-equiv='refresh' content='5'>"));
 client.print(F("</head><body bgcolor='black'><br>"));
 client.print(F("<hr/><hr>"));
 client.print(F("<h1 style='color : #3AAA35;'><center> Paramètres du Variateur d'ambiance </center></h1>"));
 client.print(F("<hr/><hr>"));
 client.print(F("<h2 style='color:green;'>Données récoltées : </h2>"));
 client.println("<p style='color:white;'>");
 client.print(F("<br><br>"));
 client.print("<br><br>");
 for (int i = 0; i < 6; i++) {
   client.print("<b>Input A");
   client.print(i);
   client.print(" : </b>");
   client.print(analogRead(14 + i)); //A0=14, A1=15 ,etc.
   client.print(F("<br>"));
 }
 client.print(F("</p><br>"));
 client.print(F("<h2 style='color:green;'>Paramètres : </h2>"));
 client.print(F("<p style='color:white;'>"));
 //digital output
 client.print(F("<br><br>"));
 client.print(F("<b>Digital output Pin 2 : </b>"));
 client.print("<input value=" + String(digitalRead(2)) + " readonly></input>");
 client.print(F("<a href='/dig2on'><button>Turn On </button></a>"));
 client.print(F("<a href='/dig2off'><button>Turn Off </button></a><br />"));
 client.print(F("</p><br>"));
 //file end
 client.print(F("<br></body></html>"));
void handleRequest( ) { /* function handleRequest */
 ////Handle web client request
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
 ////Send webpage to client
 //output HTML data header
 client.println(F("HTTP/1.1 200 OK"));
 client.println(F("Content-Type: text/html"));
 client.println();
 //header
 
 delay(1);
}
