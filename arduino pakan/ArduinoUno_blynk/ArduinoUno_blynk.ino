#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno (7, 6);
#define triggerPin  2
#define echoPin     3
#include <Servo.h>
Servo myservo;
#include "RTClib.h"
RTC_DS3231 rtc;
#include "HX711.h"
#define DOUT  A0
#define CLK  A1
HX711 scale(DOUT, CLK);
float calibration_factor = -439.91;
int GRAM, tanggal, bulan, tahun, jam, menit, detik;


void setup() {
   if (! rtc.begin()) {
    Serial.println("RTC Tidak Ditemukan");
    Serial.flush();
    abort();
  }
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  ArduinoUno.begin(4800);
  myservo.attach(9);
  myservo.write(0); 
  scale.set_scale();
  scale.tare();
  
}

void logical(){
    DateTime now = rtc.now();
  jam     = now.hour(), DEC;
  menit   = now.minute(), DEC;
  detik   = now.second(), DEC;
  String waktumakan = String() + jam + ":" + menit + ":" + detik;
 long duration, jarak;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration/2) / 29.1;
  scale.set_scale(calibration_factor);
  GRAM = scale.get_units(), 4;

  if (GRAM>100){
    myservo.write(0);
    }
  
  else if (jarak<=10 && waktumakan>"9:10:0" && waktumakan<"9:13:00" && GRAM<=100){
  Serial.println("Jarak Hidup");
  Serial.println(GRAM);
  myservo.write(120);
  }

  else if(jarak<=10 && waktumakan<"9:10:0" && waktumakan>"9:13:00" && GRAM>=100){
    Serial.println("Berat : ");
    Serial.print(GRAM);
     myservo.write(0);
    }


  else if(jarak<=10 && waktumakan>"9:13:00"){
     myservo.write(0);
  Serial.println("Bukan Jam Makan");
  Serial.println(GRAM); 
    }

else{
  myservo.write(0);
  Serial.println("No Activity");  
  Serial.println(GRAM);
  }
   Serial.print("Berat : ");
  Serial.println(GRAM);
  Serial.print("Waktu : ");
   Serial.println(waktumakan);
  delay(1000);
  }

  
void loop() {
   DateTime now = rtc.now();
  jam     = now.hour(), DEC;
  menit   = now.minute(), DEC;
  detik   = now.second(), DEC;
  String waktumakan = String() + jam + ":" + menit + ":" + detik;
 long duration, jarak;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration/2) / 29.1;
  scale.set_scale(calibration_factor);
  GRAM = scale.get_units(), 4;

 logical();
 
  while (ArduinoUno.available() > 0) {
    int val = ArduinoUno.parseInt();
    if (ArduinoUno.read() == '\n') {
      Serial.println(val);
      
      if (val == HIGH && GRAM>=100) {
        Serial.println("Wadah Penuh");
        Serial.println(GRAM);
        myservo.write(0);
//        val == LOW; 
        
      }
      
    else if ( val == HIGH ) {
        myservo.write(90); 
        Serial.println("Servo Terbuka");
         delay (3000);
         myservo.write(0);
      }
      else if ( val == LOW) {
         myservo.write(90); 
        Serial.println("Servo Terbuka");
         delay (3000);
         myservo.write(0);
       
      }
      else {
      myservo.write(0); 
      Serial.println(GRAM);
       
      }
    }
  }
 
}
