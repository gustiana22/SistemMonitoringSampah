#include<KRwifi.h>
#include <HX711.h>
#include <Servo.h>
#include <Ultrasonic.h>
const int PIN_DT = 5;
const int PIN_SCK = 4;
const int PIN_DT2 = 7;
const int PIN_SCK2 = 6;
#define calibration_factor 99000
HX711 timbangan, timbangan2;
Servo myservo;

Ultrasonic ultrasonic(24, 25);
int distance;

char* ssid = "affogato";   //Ganti xxx dengan nama WiFi / Hotspot
char* pass = "chiperman";   //Ganti xxx dengan password WiFi / Hotspot
char* server = "api.thingspeak.com";


const int sensor = A0;
int basah = 140, kering = 20, awal = 74;

int benda, sampah , hitung, hitung2;
void setup() {
  Serial.begin(9600);
  myservo.attach(40);
  myservo.write(awal);
  pinMode(sensor, INPUT);
  setWifi(ssid, pass);

  timbangan.begin(PIN_DT, PIN_SCK);
  timbangan2.begin(PIN_DT2, PIN_SCK2);
  timbangan.set_scale(calibration_factor); // Tukar dengan faktor penyekala Anda
  timbangan.tare();
  timbangan2.set_scale(calibration_factor); // Tukar dengan faktor penyekala Anda
  timbangan2.tare();


}

void loop() {
  float berat_basah = timbangan.get_units();
  float berat_kering = timbangan2.get_units();

  if (berat_basah <= 0 || berat_kering <= 0) {
    berat_basah = 0;
    berat_kering = 0;
  }


  distance = ultrasonic.read();
  sampah = analogRead(sensor);

  if (distance <= 20) {
    hitung = hitung + 1;
  } else {
    hitung = 0;
  }

  if (hitung > 3) {
    if (sampah > 500) {
      Serial.println("kering");
      myservo.write(kering);
      delay(2000);
      myservo.write(awal);
    }
    else {
      Serial.println("basah");
      myservo.write(basah);
      delay(2000);
      myservo.write(awal);
    }
    hitung = 0;
  }

  hitung2 = hitung2 + 1;
  if (hitung2 > 20) {
    String kirim = "/update?api_key=4X040H138UV11FV7&field1=" + String(berat_basah) + "&field2=" + String(berat_kering);
    httpGet(server, kirim, 80);
    Serial.println(kirim);
    hitung2 = 0;
  }

  Serial.println(hitung);
  Serial.println(distance);
  Serial.println(sampah);
  Serial.print("Berat: ");
  Serial.println(berat_basah, 1);
  Serial.print("Berat2: ");
  Serial.println(berat_kering, 1);
  delay(1000);
}
