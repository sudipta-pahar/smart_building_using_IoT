#include<SoftwareSerial.h>
int data;
int data1;
boolean lock = true;
int counter = 0;
int Light_status = 10;
#define SSID "xxx"
#define PASS "xxx"
SoftwareSerial esp8266(3,4);
String sendAT(String command, const int timeout)
{
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while((time + timeout)>millis())
  {
    while (esp8266.available())
    {
      char c = esp8266.read();
      response += c;
    }
  }
  Serial.print(response);
  return response;
}

void connectwifi()
{
  sendAT("AT\r\n",1000);
  sendAT("AT+CWMODE=1\r\n",1000);
  sendAT("AT+CWJAP=\""SSID"\",\""PASS"\"\r\n",2000);
  while(!esp8266.find("OK"))
  {
    
  }
  sendAT("AT+CIFSR\r\n",1000);
  sendAT("AT+CIPMUX=0\r\n", 1000);
}
 
void setup()
{
  esp8266.begin(9600);
  Serial.begin(9600);
  sendAT("AT+RST\r\n",2000);
  connectwifi();
  pinMode(10,INPUT);
  pinMode(11,INPUT);
  pinMode(8,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  Serial.print("Calibrating sensor");
  for(int i=0; i<3; i++)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("done");
  Serial.println("Sensor Active");
  digitalWrite(12,HIGH);
  delay(1000);
  digitalWrite(12,LOW);
  delay(50);
}

void loop()
{
  data = digitalRead(10);
  data1 = digitalRead(11);
  if(data = HIGH && lock == true)
  {
    lock = false;
    digitalWrite(12,HIGH);
    delay(500);
    digitalWrite(12,LOW);
    Serial.print("sensor ON");
    counter++;
    if(counter>0)
    {
      digitalWrite(8, HIGH);
      Light_status=100;
      passTS(counter,Light_status);
    }
    delay(4000);
    lock = true;
  }

   if(data == HIGH && lock == true)
   {
    lock = false;
    digitalWrite(12, HIGH);
    delay(500);
    digitalWrite(12, LOW);
    Light_status=10;
   }
   passTS(counter, Light_status);
   delay(4000);
   lock=true;
}

void passTS(int c, int l)
{
  String counter1 = String(c);
  Serial.print("Number of persons");
  Serial.println(c);
  String Light_status1 = String(l);
  Serial.print("Light STatus");
  Serial.println(l);
  updateTS(counter1, Light_status1);
  delay(3000);
}
void updateTS(String C, String L)
{
  Serial.print("");
  sendAT("AT+CIPSTART=\"TCP\", \"api.thingspeak.com\",80\r\n", 1000);
  delay(2000);
  String cmdlen;
  String cmd = "GET /update?key=xxx="+C+"&field2="+L+"\r\n";
  cmdlen = cmd.length();
  sendAT("AT+CIPSEND="+cmdlen+"\r\n",2000);
  esp8266.print(cmd);
  Serial.println("");
  sendAT("AT+CIPCLOSE\r\n",2000);
  Serial.println("");
  delay(1000);
}
