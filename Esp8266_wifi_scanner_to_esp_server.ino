#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>

#define AP_SSID                  "RTS_Z408"//"RTS_Z408"        // Логин к вашему WiFi(кавычки оставить)

#define AP_PASS                  "fobusfobus"//"fobusfobus"

//String url = "http://mrblackdog.ddns.net:48706";
String namestring = "ESPTEST";
unsigned int  localPort = 2590;
char ssid[] = ""; 
char pass[] = "";     
const char* error = "E0_";
const char* buffer_data ="";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];
IPAddress ip_test (192, 168, 4, 1);
WiFiUDP Udp;

void setup(void) {
   Serial.begin(115200);
   delay(1000);
   WiFi.mode( WIFI_AP_STA);             // режим точки и клиента

// Соединение с WiFi  
   if( !ConnectWiFi(AP_SSID,AP_PASS) )
   {
        error = "E1_";   // неудачное подключение WiFi
   }
  else
       error = "E2_";   // удачное подключение WiFi       
   Serial.print('A');                 // _ означает начало строки с данными       
//&nbsp;&nbsp;&nbsp; Serial.print(error);              // выдача в порт информации об ошибке  
 Serial.print(error);              // выдача в порт информации об ошибке
   delay(1000);
   Udp.begin(localPort);
}

void loop() {
 // Serial.println(WiFi.localIP());

  String logstring = "";
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print("#");
      Serial.print(WiFi.RSSI(i));
      Serial.print("#");
      Serial.println(WiFi.BSSIDstr(i));
      
    if(WiFi.SSID(i).indexOf(" ")!= -1)
    {
    String newSSID = WiFi.SSID(i);
    newSSID.replace(" ","+");
    logstring = logstring + newSSID + "+" +WiFi.BSSIDstr(i)+ "+" +WiFi.RSSI(i) + '|';
    }
    else
    {
    logstring = logstring + WiFi.SSID(i) +"+"+WiFi.BSSIDstr(i)+"+"+WiFi.RSSI(i) + '|';
    }
      delay(10);
    }
  }

if (Udp.beginPacket(ip_test, localPort))  //устанавливаем соединение с удаленным устройством
    {
      Serial.println ("IP OK");
    }
    else
    Serial.println ("IP Error");
    
    Udp.write("xd");            // отправляем температур
    Udp.endPacket();
    Udp.stop();            //Разрывает соединение с сервером, освобождая все ресурсы, используемые во время UDP-сессии.
  delay(1000);
}

bool ConnectWiFi(const char *ssid, const char *pass)
{
// Три попытки соединения по WiFi
   for( int i=0; i<3; i++)
   {
      WiFi.begin(ssid,pass);
      delay(1000);
// Максиммум 5 раз проверка соединения      
      for( int j=0; j<5; j++ )
      {
          if (WiFi.status() == WL_CONNECTED)
          {
              return true;
          }
          delay(1000); 
      }
   }
return false;
}
