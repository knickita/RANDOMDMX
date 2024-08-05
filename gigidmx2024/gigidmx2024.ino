#define BUTTON_1A D1
#define BUTTON_2A D2
#define BUTTON_3A D3
#define BUTTON_1B D5
#define BUTTON_2B D6
#define BUTTON_3B D7

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

void setup() {  
  // Set up the DMX-Protocol
  Serial.begin(DMXSPEED);
  Serial1.begin(DMXSPEED);
  pinMode(sendPin1, OUTPUT);
  pinMode(sendPin2, OUTPUT);

  //setup buttons
  pinMode(BUTTON_1A, INPUT);  
  pinMode(BUTTON_2A, INPUT);
  pinMode(BUTTON_3A, INPUT);
  pinMode(BUTTON_1B, INPUT);
  pinMode(BUTTON_2B, INPUT);
  pinMode(BUTTON_3B, INPUT);
}

void loop(){
  //set DMX values  
  dmxData1[1] = digitalRead(BUTTON_1A)*255;
  dmxData1[2] = digitalRead(BUTTON_2A)*255;
  dmxData1[3] = digitalRead(BUTTON_3A)*255;
  dmxData2[1] = digitalRead(BUTTON_1B)*255;
  dmxData2[2] = digitalRead(BUTTON_2B)*255;
  dmxData2[3] = digitalRead(BUTTON_3B)*255;
  updateDMX();
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
