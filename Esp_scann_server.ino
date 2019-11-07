#include <ESP8266WiFi.h>

#include <WiFiUdp.h>

 

//********************************Изменить для себя ***********

#define AP_SSID                  "RTS_Z408"        // Логин к вашему WiFi(кавычки оставить)

#define AP_PASS                  "fobusfobus"   //Пароль у вашему WiFi

#define TIMEZONE 3                                // Часовой пояс

const char* SSIDDAT = "test";                  // имя сети к которой будет подключаться втророй модуль изменить на желаемую

const char* PASSSIDDAT = "1234";              // пароль сети к которой будет подключаться втророй модуль изменить на желаемый

//*************************************************************

char packetBuffer[255];                    //buffer to hold incoming packet

char ReplyBuffer[] = "acknowledged";       // a string to send back

char ssid[] = ""; 

char pass[] = "";     

const char* error = "E0_"; 

 

unsigned int  localPort = 2590;      // local port to listen for UDP packets

const int byte_am2321 = 10;

byte packet_am2321[ byte_am2321];

WiFiUDP Udp;

 

void setup()

{

   Serial.begin(115200);

   delay(1000);

   WiFi.mode( WIFI_AP_STA);             // режим точки и клиента

   WiFi.softAP( SSIDDAT,PASSSIDDAT);

    

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

 

 

//###########################################

// Основная программа

void loop()

{ 

    

    int packetSize = Udp.parsePacket();

    if (packetSize) {

    Serial.print("Received packet of size ");

    Serial.println(packetSize);

    Serial.print("From ");

    IPAddress remoteIp = Udp.remoteIP();

    Serial.print(remoteIp);

    Serial.print(", port ");

    Serial.println(Udp.remotePort());

 

    // чтение пакета

    int len = Udp.read(packetBuffer, 255);

    if (len > 0) {

      packetBuffer[len] = 0;

    }

    Serial.println("Contents:");

    Serial.println(packetBuffer);

 

    // отправка принятого пакета

    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());

    Udp.write(ReplyBuffer);

    Udp.endPacket();

  }

  

}

    

//####################################

/**
079
 * Соединение с WiFi
080
 */

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
