#include <UnoWiFiDevEd.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Wifi (doit être déjà connecté au réseau via les réglages arduino uno wifi rev3)
#define CONNECTOR     "rest"
#define SERVER_ADDR   "api.thingspeak.com"
#define APIKEY_THINGSPEAK  "XXXXXX"

#define GEIGER 3
// Pins 1 et 2 -> i2c

// Voyants leds
#define SYSTEME_ON LED_BUILTIN
#define WIFI_ON 4
#define BME_ON 5
#define TEMP_ON 7
#define DATA 8
#define RAYON  9

unsigned long compteur_geiger = 0;
unsigned long previousMillis;
int periode = 30*1000;

int temp = 0;
unsigned long pression = 0;
int humid = 0;

bool detection_rayon = false;
bool envoi_data = false;
unsigned long heure_detection_rayon;
unsigned long heure_envoi_data;

String uri;


// Baromètre
Adafruit_BME280 bme;





void allumer_voyant(int n) {
  digitalWrite(n, HIGH);
}

void impact_geiger() {
  compteur_geiger++;
  detection_rayon = true;
  heure_detection_rayon = millis();
  allumer_voyant(RAYON);
}




void setup() {
  allumer_voyant(SYSTEME_ON);
  Ciao.begin();
  allumer_voyant(WIFI_ON);

  uri.reserve(56);
  
  pinMode(SYSTEME_ON, OUTPUT);
  pinMode(WIFI_ON, OUTPUT);
  pinMode(BME_ON, OUTPUT);
  pinMode(TEMP_ON, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RAYON, OUTPUT);
  pinMode(GEIGER, INPUT);
  
if (!bme.begin(0x76)) {
    digitalWrite(SYSTEME_ON, LOW);
}  
  attachInterrupt(digitalPinToInterrupt(GEIGER), impact_geiger, FALLING);
  previousMillis = millis();
}






void loop() {
  unsigned long currentMillis = millis();
  if (detection_rayon and currentMillis - heure_detection_rayon > 90) {
    digitalWrite(RAYON, LOW);
    detection_rayon = false;
  }
  if (envoi_data and currentMillis - heure_envoi_data > 90) {
    digitalWrite(DATA, LOW);
    envoi_data = false;
  }
  if (currentMillis - previousMillis > periode) {
    detachInterrupt(digitalPinToInterrupt(GEIGER));
    previousMillis = currentMillis;
    
    temp = bme.readTemperature();
    pression = bme.readPressure();
    humid = bme.readHumidity();
    
    allumer_voyant(DATA);
    heure_envoi_data=millis();
    envoi_data=true;
    
    uri = "/update?api_key=";
    uri += APIKEY_THINGSPEAK;
    uri += "&field1=";
    uri += String(compteur_geiger);
    uri += "&field2=";
    uri += String(pression);
    
    Serial.println(uri);
    Ciao.println("Sending data on ThingSpeak Channel");
    CiaoData data = Ciao.write(CONNECTOR, SERVER_ADDR, uri);
    
    if (!data.isEmpty()) {
      Ciao.println( "State: " + String (data.get(1)) );
      Ciao.println( "Response: " + String (data.get(2)) );
    }
    else Ciao.println("Write Error");
    
    compteur_geiger = 0;
    attachInterrupt(digitalPinToInterrupt(GEIGER), impact_geiger, FALLING);
  }
}
