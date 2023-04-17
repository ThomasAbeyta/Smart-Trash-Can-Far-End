#include "Particle.h"

// Constants
const unsigned long SEND_INTERVAL_MS = 6000;
const size_t READ_BUF_SIZE = 64;

const int RADIOADDRESS = 102; // 
const int RADIONETWORKID = 5; // range of 0-16
const int SENDADDRESS = 101; // address of radio to be sent to
String TESTDATA = "THIS is farEnd LoRa";
char readBuf[READ_BUF_SIZE + 1];

// Global variables
unsigned long lastSend = 0;
String password = "FABC0002EEDCAA90FABC0002EEDCAA90"; // AES password
String Location = "PinoYards";


SYSTEM_MODE(SEMI_AUTOMATIC);
 
void setup() {
    Serial.begin(9600);
    waitFor(Serial.isConnected, 5000);
    Serial1.begin(115200);
    reyaxSetup(password);
}

void loop() {

  if (millis() - lastSend >= SEND_INTERVAL_MS) {
    lastSend = millis();
    // sendData();
    // resetF();
    readData();
  }
}

void sendData() {
    char buffer[60];
    sprintf(buffer, "AT+SEND=%i,60,%s,%s\r\n", SENDADDRESS,TESTDATA, Location.c_str());
    Serial1.printf("%s",buffer);
    if (Serial1.available() > 0) {
      Serial.printf("Awaiting Reply from send\n");
      String reply = Serial1.readStringUntil('\n');
      Serial.printf("LoRa Module: %s\n", reply.c_str());
  }
}

void readData() {
 if (Serial1.available() > 0) {
      Serial.printf("Awaiting Reply from send\n");
      String reply = Serial1.readStringUntil('\r\n');
      if (reply.startsWith("+RCV=101")) {
        Serial.printf("LoRa Module: %s\n", reply.c_str());
      }
 }
}

void resetF(){
  String reply; // string to store replies from module
  Serial1.printf("AT+FACTORY\r\n");
  delay(300);
  if (Serial1.available() > 0) {
    Serial.printf("Awaiting Reply from reset\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Reply from reset: %s\n", reply.c_str());
  }
}

void reyaxSetup(String password) {
  const int SPREADINGFACTOR = 10;
  const int BANDWIDTH = 7;
  const int CODINGRATE = 1;
  const int PREAMBLE = 7;
  int delay_MS = 100;
  String reply; // string to store replies from module

  Serial1.printf("AT+ADDRESS=%i\r\n", RADIOADDRESS); 
  delay(delay_MS);
  if (Serial1.available() > 0) {
    Serial.printf("Awaiting Reply from address\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Reply address: %s\n", reply.c_str());
  }

  Serial1.printf("AT+NETWORKID=%i\r\n", RADIONETWORKID); 
  delay(delay_MS);
  if (Serial1.available() > 0) {
    Serial.printf("Awaiting Reply from networkid\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Reply network: %s\n", reply.c_str());
  }

  Serial1.printf("AT+CPIN=%s\r\n", password.c_str());
  delay(delay_MS);
  if (Serial1.available() > 0) {
    Serial.printf("Awaiting Reply from password\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("password Reply: %s\n", reply.c_str());
  }

  Serial1.printf("AT+PARAMETER=%i,%i,%i,%i\r\n", SPREADINGFACTOR, BANDWIDTH, CODINGRATE, PREAMBLE);
  delay(delay_MS);
  if (Serial1.available() > 0) {
    Serial.printf("Awaiting Reply for parameters\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("parameter reply: %s\n", reply.c_str());
  }

  Serial1.printf("AT+ADDRESS?\r\n");
  delay(delay_MS);
  if (Serial1.available() > 0) {
    Serial.printf("Awaiting Reply from network\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Radio Address: %s\n", reply.c_str());
  }

  Serial1.printf("AT+NETWORKID?\r\n");
  delay(delay_MS);
  if (Serial1.available() > 0) {
    Serial.printf("Awaiting Reply from networkid\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Radio Network: %s\n", reply.c_str());
  }

  Serial1.printf("AT+PARAMETER?\r\n");
  delay(300);
  if (Serial1.available() > 0) {
    Serial.printf("Awaiting Reply from Parameter\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Radio Parameters: %s\n", reply.c_str());
  }

  Serial1.printf("AT+CPIN?\r\n");
  delay(delay_MS);
  if (Serial1.available() > 0) {
    Serial.printf("Awaiting Reply cpin\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Radio Password: %s\n", reply.c_str());
  }
  Serial1.printf("AT+CRFOP?\r\n"); // RF output power
  delay(delay_MS);
  if (Serial1.available() > 0) {
    Serial.printf("Awaiting Reply from RF output power:\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Reply RF output power: %s\n\n\n", reply.c_str());
  }
}
