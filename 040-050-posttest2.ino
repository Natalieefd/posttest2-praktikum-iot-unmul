#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Define pins
const int BUZZER_PIN = D8;
const int LED_PIN = D4;
const int BUTTON_PIN1 = D1;
const int BUTTON_PIN2 = D2; // nambah
const int BUTTON_PIN3 = D3; // kurang

int startFreq = 100;
int setFreq = 10;

int startLed = 0;
int setLed = 3; 

bool power = true;

const char* ssid = "Universitas Mulawarman";      
const char* password = "";   

// const char* ssid = "o.o";      
// const char* password = "mauapakamuu";   

ESP8266WebServer server(80);      
String webpage;

void handleRoot() {
 
  String frekuensi = String(startFreq);
  String lampu = String(startLed);
  String status;

  if (power == true){
    status = "Dalam Keadaan Mati";
  } else {
    status = "Dalam Keadaan Nyala";
  }
  
  webpage = "<html><meta http-equiv=refresh content=2><head><title>POSTTEST 2 - IoT Kelompok 3</title></head><body>";
  webpage += "<div align=center><h1>POSTTEST 2 - IoT</h1>";
  webpage += "<h1>Monitoring Kecerahan Lampu & Nada Buzzer</h1>";
  webpage += "<p><b>Frekuensi: </b>" + frekuensi + "</p>";
  webpage += "<p><b>Lampu: </b>" + lampu + "</p>";
  webpage += "<p><b>Status: </b>" + status + "</p><br>";
  webpage += "</div></body></html>";
  server.send(200, "text/html", webpage);
}

void setup() {
  Serial.begin(115200);
  delay(10);
  // Initialize pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);

  Serial.println();
  Serial.print("Configuring access point...");

  WiFi.mode(WIFI_STA);                      
  WiFi.begin(ssid, password);  
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Print status Connect 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Server dijalankan");
}

void loop(){
  server.handleClient(); 

  int buttonState1= digitalRead(BUTTON_PIN1);
  int buttonState2 = digitalRead(BUTTON_PIN2);
  int buttonState3 = digitalRead(BUTTON_PIN3); 

 if(buttonState1 == LOW){
    if(power){
      tone(BUZZER_PIN, startFreq);
      digitalWrite(LED_PIN,startLed);
      power = false;
    }else{
      noTone(BUZZER_PIN);
      digitalWrite(LED_PIN,LOW);
      power = true;
    }
    delay(10);
  }

  if(buttonState2 == LOW){
    startFreq += setFreq;
    if(startFreq > 2500){
      startFreq = 2500;
    }
    tone(BUZZER_PIN, startFreq);
    delay(100);

    analogWrite(LED_PIN, startLed);
    startLed += setLed;
    if(startLed >= 249){
      startLed = 249;
    }
  }

  if(buttonState3 == LOW){
    startFreq -= setFreq;
    if(startFreq < 100){
      startFreq = 100;
    }
    tone(BUZZER_PIN, startFreq);
    delay(100);

    analogWrite(LED_PIN, startLed);
    startLed -= setLed;
    if(startLed < 0){
      startLed = 0;
    }
  }
   
}