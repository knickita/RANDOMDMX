  
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>


#include <ESPDMX.h>
#define BUTTON1 D6
#define BUTTON2 D7
#define BUTTON3 D5
#define LED A0

byte value1;
byte value2;
byte value3;


const char *ssid = "TRUMENBUTTONS";

ESP8266WebServer server(80);
/*
   Go to http://192.168.4.1 in a web browser
*/

DMXESPSerial dmx;

void handleRoot() {
  Serial.write("home page served\n");
  String info = "<h1>TRUMEN BUTTONS</h1>";
  info+= "<form enctype='application/json' action='/change' >";
  info+= "  BUTTONE 1 <input type='number' name='BUTTON1' value='";
  info+=value1;
  info+="'>Attuale: ";
  info+=value1;
  info+="<br>";
  info+= "  BUTTONE 2 <input type='number' name='BUTTON2' value='";
  info+=value2;
  info+="'>Attuale: ";
  info+=value2;
  info+="<br>";
  info+= "  BUTTONE 3 <input type='number' name='BUTTON3' value='";
  info+=value3;
  info+="'>Attuale: ";
  info+=value3;
  info+="<br>";
  info+= "  <input type='submit' value='Salva Configurazione'>";
  info+= "</form>";
  server.send(200, "text/html", info);
}

void changeConfiguration(){
  Serial.write("change page served\n");
  value1 = server.arg("BUTTON1").toInt();
  value2 = server.arg("BUTTON2").toInt();
  value3 = server.arg("BUTTON3").toInt();
  EEPROM.write(0,value1);
  EEPROM.write(1,value2);
  EEPROM.write(2,value3);
  EEPROM.commit();
  server.sendHeader("Location","/");
  server.send(303);
}

void setup() {
  dmx.init(512);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(LED, OUTPUT);
  analogWrite(LED, 255);

  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/change", changeConfiguration);
  server.begin();
  Serial.println("HTTP server started");

  EEPROM.begin(4);
  value1=EEPROM.read(0);
  value2=EEPROM.read(1);
  value3=EEPROM.read(2);

  Serial.print("test");
  Serial.print(value1);

  
  Serial.write("data Recovered from internal memory:\n");
  Serial.write("Button1: ");
  Serial.print(value1);
  Serial.write("Button2: ");
  Serial.print(value2);
  Serial.write("Button3: ");
  Serial.print(value3);
}

void loop() {
  server.handleClient();
  if (digitalRead(BUTTON1)) {
    dmx.write(value1, 0);
  }
  else {
    dmx.write(value1, 255);
  }
  if (digitalRead(BUTTON2)) {
    dmx.write(value2, 0);
  }
  else {
    dmx.write(value2, 255);
  }

  
  if (analogRead(BUTTON3)>170) {
    dmx.write(value3, 0);
  }
  else {
    dmx.write(value3, 255);
  }

  
  dmx.update();

}
