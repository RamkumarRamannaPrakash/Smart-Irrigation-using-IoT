/*Smart Irrigation using IOT*/

#include<SoftwareSerial.h>
int srdata;
int prdata;
int pump_status;
SoftwareSerial esp(3,4);
#define SSID "RCB"
#define PASS "roomies1814"

/* Function to send AT commands to ESP 8266*/
String sendAT(String cmdd, const int timeout)
{
  String response = "";
  esp.print(cmdd);
  long int time = millis();
  while ((time + timeout) > millis())
  {
    while (esp.available())
    {
      char c = esp.read();
      response += c;
    }
   }
  Serial.print(response);
  return response;
}


/*function to initialize arduino & ESP8266 communication*/
void setup() 
{
  // put your setup code here, to run once:
Serial.begin(9600);
esp.begin(9600);
sendAT("AT+RST\r\n",2000);
sendAT("AT\r\n",1000);
sendAT("AT+CWMODE=1\r\n",1000);
sendAT("AT+CWJAP=\""SSID"\",\""PASS"\"\r\n",2000);
while(!esp.find("OK"))
{
  Serial.print("gone gone");
}
sendAT("AT+CIFSR\r\n",1000);
sendAT("AT+CIPMUX=0\r\n",1000);
pinMode(A0, INPUT);
pinMode(8, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
srdata=analogRead(A0);
prdata=map(srdata,0,1023,100,0);
Serial.print("Sensor data=");
Serial.println(prdata);
String sensor_value = String(prdata);
if(prdata<60)
{
  digitalWrite(8,LOW);
  pump_status=100;
}
else
{
  digitalWrite(8,HIGH);
  pump_status=0;
}
String pump=String(pump_status);
updateTS(sensor_value,pump);
delay(2000);
}

void updateTS(String T,String P)
{
  Serial.println("");
  sendAT("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\r\n",1000);
  delay(2000);
  String cmdlen;
  String cmd = "GET /update?key=N6TERI3JXFJMWH7K&field1="+T+"&field2="+P+"\r\n";
  cmdlen = cmd.length();
  sendAT("AT+CIPSEND="+cmdlen+"\r\n",2000);
  esp.print(cmd);
  Serial.println("");
  sendAT("AT+CIPCLOSE\r\n",2000);
  Serial.println("");
  delay(10000);
}


  
