#define BLYNK_TEMPLATE_ID "TMPLXVGwt8HI"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
// ================= CONNECT TO WIFI ================
char auth[] = "G1MFAeMjtvbeqriWaG1bhTMJ9BUXfmqJ";

char ssid[] = "Mi A1 ilham";
char pass[] = "ilham123";
int a;

SoftwareSerial NodeMCU(D2, D1);

long duration;
int distance;
BlynkTimer timer;
#define  trig  D8
#define  echo  D7

void setup() {
  pinMode(trig, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echo, INPUT);   // Sets the echoPin as an Inpu
  Serial.begin(9600);
  NodeMCU.begin(4800);
  Blynk.begin(auth, ssid, pass);
  pinMode(D2, INPUT);
  pinMode(D1, OUTPUT);
   timer.setInterval(1000L, sendSensor);
}

BLYNK_WRITE(V1)
{
  a = param.asInt();
  NodeMCU.print(a);
  NodeMCU.println("\n");
  Serial.println(a);
}

void loop() {
  Blynk.run();
   timer.run();
}

void sendSensor()
{
  digitalWrite(trig, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay

  digitalWrite(trig, HIGH);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trig, LOW);   // trigPin low

  duration = pulseIn(echo, HIGH);   //Read echo pin, time in microseconds
  distance = duration * 0.034 / 2;   //Calculating actual/real distance

  Serial.print("Distance = ");        //Output distance on arduino serial monitor
  Serial.println(distance);
  
  if(distance > 20)
  {
    Blynk.notify("Post has been twitted");
    Blynk.virtualWrite(V0, "Pakan Mau Habis");
  }else{
     Blynk.virtualWrite(V0, "Pakan Tersedia");
    }
 
  
  delay(1000);                        //Pause for 3 seconds and start measuring distance again
}
