#include <ESP8266WiFi.h>

// Access Point credentials
const char *ssid = "ATSBUTTONS1";
const char *password = "12345678"; // must be at least 8 chars


#define BUTTON_1B D6
#define BUTTON_2B D7
#define BUTTON_3B D5

#define dmxChannelQuantity  32 //max 512

#define DMXSPEED       250000
#define DMXFORMAT      SERIAL_8N2
#define BREAKSPEED     83333
#define BREAKFORMAT    SERIAL_8N1

int sendPin1 = 1;
int sendPin2 = 2;

//DMX value array and size. Entry 0 will hold startbyte
uint8_t dmxData1[dmxChannelQuantity] = {};
uint8_t dmxData2[dmxChannelQuantity] = {};

// Create server on port 80
WiFiServer server(80);

void setup() {  

  //start AP
  WiFi.softAP(ssid, password);
  
  //Start Server
  server.begin();

  // Set up the DMX-Protocol
  Serial.begin(DMXSPEED);
  Serial1.begin(DMXSPEED);
  pinMode(sendPin1, OUTPUT);
  pinMode(sendPin2, OUTPUT);

  //setup buttons
  pinMode(BUTTON_1B, INPUT);
  pinMode(BUTTON_2B, INPUT);
  pinMode(BUTTON_3B, INPUT);
}

void loop(){

  handleRequest ();

  //set DMX values  
  dmxData2[1] = (1 - digitalRead(BUTTON_1B))*255;
  dmxData2[2] = (1 - digitalRead(BUTTON_2B))*255;
  dmxData2[3] = (1 - digitalRead(BUTTON_3B))*255;
  updateDMX();
}

void handleRequest(){
  //Server Listening
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  while (!client.available()) {
    delay(1);
  }
  // Read request
  String request = client.readStringUntil('\r');
  client.flush();
 
  if (request.indexOf("/state") != -1) {
    String info = "";
    info += String("Button 1B: ") + (digitalRead(BUTTON_1B) == HIGH ? "RILASCIATO" : "PREMUTO") + "<br>";
    info += String("Button 2B: ") + (digitalRead(BUTTON_2B) == HIGH ? "RILASCIATO" : "PREMUTO") + "<br>";
    info += String("Button 3B: ") + (digitalRead(BUTTON_3B) == HIGH ? "RILASCIATO" : "PREMUTO") + "<br>";

    String json = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n";
    json += "{\"status\":\"";
    json += info;
    json += "\"}";
    client.print(json);
    return;
  }

  // Simple webpage
 String html = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  html += "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<title>ATS DMX BUTTON INFO</title>";
  html += "<h1>CIAO</h1>";
  html += "<h2>Sono Vivo</h2>";
  html += "<body>ora sei sicuro che almeno la scheda sia viva. Un primo passo verso la fine della serata!</body>";
  html += "<script>";
  html += "function updateState(){";
  html += "fetch('/state').then(r=>r.json()).then(data=>{";
  html += "document.getElementById('status').innerHTML = data.status;";
  html += "});}";
  html += "setInterval(updateState, 30);"; // aggiorna 
  html += "</script></head><body>";
  html += "<h1>Stato: <br> <span id='status'>...</span></h1>";
  html += "</body></html>";


    // Send response
  client.print(html);
}

void updateDMX(){
  //Universe1  //Send break
  digitalWrite(sendPin1, HIGH);
  Serial.begin(BREAKSPEED, BREAKFORMAT);
  Serial.write(0);
  Serial.flush();
  delay(1);
  Serial.end();

  //Universe1  //send data
  Serial.begin(DMXSPEED, DMXFORMAT);
  digitalWrite(sendPin1, LOW);
  Serial.write(dmxData1, dmxChannelQuantity);
  Serial.flush();
  delay(1);
  Serial.end();

  //Universe2  //Send break
  digitalWrite(sendPin2, HIGH);
  Serial1.begin(BREAKSPEED, BREAKFORMAT);
  Serial1.write(0);
  Serial1.flush();
  delay(1);
  Serial1.end();

  //Universe2  //send data
  Serial1.begin(DMXSPEED, DMXFORMAT);
  digitalWrite(sendPin2, LOW);
  Serial1.write(dmxData2, dmxChannelQuantity);
  Serial1.flush();
  delay(1);
  Serial1.end();
}
