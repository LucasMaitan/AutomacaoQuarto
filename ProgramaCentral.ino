//Bibliotecas
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "DHT.h"

//Referências
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME  "xxxxxxxxxxxxxxx" //Nome do usuario da conta no Adafruit
#define AIO_KEY       "xxxxxxxxxxxxxxxxxxxxxxxxxxxx" //Chave encontrada na plataforma do Adafruit
#define DHTPIN 4 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11

//Declaração para sensor umidade e temperatura
DHT dht(DHTPIN, DHTTYPE);

//Declarações de variáveis
const uint16_t kIrLed = 14;
const long interval = 1000000;
long lastMillis = 0;
int humidity_anterior = 0;
int temperature_anterior = 0;

//Função declara códigos RAW para sensor emissor Infravermelho (Códigos clonados do Controle remoto)
IRsend irsend(kIrLed);
  uint16_t liga19[199] = {4376, 4388, 596, 1580, 600, 492, 596, 1584, 600, 1580, 600, 492, 600, 492, 600, 1580, 596, 496, 600, 496, 600, 1580, 600, 496, 600, 492, 600, 1576, 600, 1580, 576, 516, 600, 1584, 576, 1608, 576, 520, 572, 1608, 576, 1604, 572, 1604, 576, 1604, 572, 1604, 548, 1632, 548, 548, 572, 1604, 548, 548, 572, 520, 572, 520, 572, 520, 572, 520, 572, 524, 548, 548, 572, 520, 548, 1632, 548, 1632, 548, 544, 548, 544, 548, 548, 544, 548, 548, 1636, 548, 1628, 552, 544, 548, 544, 548, 1628, 548, 1632, 548, 1628, 552, 1628, 548, 5236, 4456, 4316, 572, 1608, 572, 520, 572, 1608, 576, 1604, 596, 496, 600, 492, 600, 1580, 600, 492, 600, 496, 600, 1584, 596, 496, 596, 496, 596, 1580, 600, 1580, 600, 492, 596, 1588, 596, 1588, 596, 496, 576, 1608, 572, 1604, 572, 1608, 572, 1604, 572, 1608, 568, 1612, 568, 528, 544, 1632, 548, 544, 548, 548, 544, 544, 548, 548, 544, 548, 548, 544, 548, 548, 548, 544, 548, 1636, 548, 1628, 548, 544, 548, 544, 548, 544, 548, 548, 548, 1636, 548, 1632, 544, 548, 544, 548, 548, 1628, 548, 1632, 548, 1628, 548, 1636, 596};
  uint16_t ligado[199] = {4400, 4376, 548, 1628, 548, 548, 544, 1636, 548, 1628, 548, 548, 544, 548, 544, 1632, 548, 548, 548, 544, 548, 1636, 548, 544, 548, 544, 548, 1632, 548, 1628, 548, 544, 548, 1636, 548, 1636, 548, 548, 544, 1636, 548, 1628, 548, 1632, 544, 1632, 548, 1632, 548, 1632, 548, 548, 544, 1632, 548, 544, 548, 544, 548, 544, 548, 548, 544, 548, 544, 552, 544, 548, 548, 544, 548, 1636, 544, 548, 548, 544, 548, 544, 548, 544, 548, 548, 548, 1632, 548, 1628, 548, 548, 544, 1636, 548, 1632, 544, 1632, 548, 1632, 544, 1632, 572, 5212, 4404, 4368, 548, 1632, 544, 548, 548, 1632, 548, 1632, 548, 544, 548, 544, 548, 1628, 548, 548, 548, 548, 548, 1636, 544, 548, 548, 544, 544, 1632, 548, 1628, 548, 548, 544, 1636, 548, 1636, 548, 548, 544, 1636, 548, 1628, 548, 1628, 548, 1632, 548, 1632, 544, 1632, 548, 548, 548, 1628, 548, 548, 548, 544, 548, 544, 548, 544, 548, 548, 544, 548, 548, 548, 544, 548, 544, 1636, 548, 544, 548, 544, 548, 544, 548, 544, 548, 548, 548, 1632, 548, 1628, 548, 544, 548, 1632, 548, 1632, 548, 1628, 548, 1628, 548, 1632, 600};
  uint16_t liga21[199] = {4448, 4336, 596, 1580, 600, 492, 600, 1580, 600, 1580, 600, 492, 600, 492, 600, 1576, 600, 496, 600, 496, 596, 1584, 600, 492, 596, 496, 600, 1580, 596, 1580, 600, 492, 600, 1584, 600, 1584, 600, 492, 600, 1580, 600, 1580, 596, 1580, 600, 1576, 600, 1580, 596, 1584, 596, 500, 596, 1580, 600, 492, 600, 492, 600, 492, 600, 492, 600, 496, 600, 492, 596, 496, 600, 1580, 596, 1580, 600, 492, 600, 492, 596, 496, 600, 496, 596, 496, 600, 1580, 600, 492, 600, 492, 600, 1576, 600, 1580, 596, 1580, 596, 1580, 600, 1580, 600, 5156, 4452, 4320, 596, 1580, 600, 492, 600, 1580, 600, 1580, 600, 492, 600, 492, 596, 1580, 600, 500, 596, 496, 596, 1584, 596, 496, 600, 492, 600, 1576, 600, 1580, 600, 492, 600, 1584, 600, 1584, 596, 496, 596, 1584, 600, 1580, 600, 1576, 600, 1576, 600, 1580, 596, 1584, 596, 496, 600, 1576, 600, 496, 596, 496, 596, 496, 596, 496, 596, 496, 596, 496, 600, 496, 596, 1580, 600, 1576, 600, 496, 596, 496, 596, 496, 596, 496, 596, 500, 596, 1580, 600, 492, 600, 492, 600, 1580, 596, 1580, 548, 1628, 600, 1580, 596, 1584, 548};
  uint16_t liga22[199] = {4400, 4384, 600, 1580, 596, 496, 596, 1584, 596, 1580, 600, 496, 596, 496, 596, 1580, 596, 500, 596, 496, 600, 1584, 596, 496, 596, 496, 596, 1580, 600, 1580, 596, 496, 600, 1584, 600, 1584, 600, 496, 596, 1584, 596, 1584, 596, 1580, 600, 1580, 596, 1580, 600, 1584, 596, 496, 596, 1584, 596, 496, 548, 544, 548, 548, 548, 544, 548, 544, 548, 544, 548, 548, 548, 1632, 596, 1580, 596, 1584, 596, 496, 548, 544, 548, 544, 548, 548, 548, 1636, 548, 544, 548, 544, 548, 544, 548, 1636, 596, 1580, 600, 1580, 596, 1584, 596, 5188, 4404, 4368, 596, 1584, 596, 496, 548, 1636, 596, 1580, 596, 500, 544, 548, 548, 1628, 548, 548, 548, 548, 548, 1632, 548, 548, 544, 548, 548, 1628, 552, 1628, 596, 500, 548, 1636, 548, 1636, 596, 496, 548, 1632, 548, 1632, 572, 1604, 600, 1580, 548, 1632, 572, 1604, 552, 544, 552, 1628, 548, 544, 548, 548, 548, 544, 548, 544, 548, 544, 548, 548, 548, 548, 544, 1632, 548, 1632, 548, 1628, 572, 520, 552, 540, 552, 544, 548, 548, 548, 1632, 552, 544, 548, 544, 548, 544, 548, 1632, 552, 1628, 548, 1628, 552, 1632, 548};
  uint16_t liga23[199] = {4424, 4336, 596, 1580, 600, 492, 600, 1584, 600, 1576, 596, 496, 600, 492, 600, 1580, 596, 496, 600, 496, 596, 1584, 600, 492, 600, 496, 596, 1580, 596, 1580, 600, 492, 600, 1584, 600, 1584, 600, 492, 600, 1580, 600, 1580, 600, 1576, 600, 1576, 600, 1580, 600, 1580, 596, 500, 596, 1580, 596, 496, 596, 496, 600, 492, 600, 492, 600, 492, 600, 496, 596, 496, 600, 1580, 596, 496, 596, 1584, 600, 492, 600, 492, 600, 492, 600, 496, 596, 1588, 596, 496, 596, 1584, 600, 492, 600, 1580, 600, 1580, 600, 1576, 600, 1580, 600, 5172, 4452, 4320, 596, 1580, 600, 492, 600, 1580, 600, 1580, 596, 496, 596, 496, 600, 1576, 600, 496, 596, 496, 600, 1584, 596, 496, 600, 492, 596, 1580, 600, 1576, 600, 496, 600, 1580, 600, 1584, 600, 492, 600, 1580, 600, 1584, 592, 1580, 600, 1556, 624, 1576, 600, 1580, 600, 496, 596, 1580, 596, 496, 596, 496, 600, 492, 596, 496, 600, 492, 600, 496, 596, 496, 600, 1576, 600, 496, 596, 1584, 596, 496, 596, 496, 600, 492, 600, 492, 600, 1584, 600, 492, 600, 1580, 600, 496, 596, 1584, 600, 1576, 600, 1580, 596, 1584, 600};
  uint16_t desligado[199] = {4404, 4384, 548, 1628, 548, 548, 544, 1636, 548, 1628, 548, 548, 544, 548, 548, 1628, 548, 548, 544, 548, 548, 1632, 548, 548, 548, 544, 548, 1628, 548, 1628, 548, 548, 544, 1640, 548, 548, 544, 1632, 548, 1628, 548, 1632, 548, 1628, 548, 544, 548, 1636, 548, 1632, 548, 1636, 548, 544, 548, 544, 548, 544, 548, 544, 548, 1632, 544, 548, 548, 548, 544, 1636, 548, 1632, 548, 1628, 548, 548, 544, 548, 544, 548, 548, 544, 544, 548, 548, 548, 548, 544, 544, 548, 548, 1632, 548, 1632, 544, 1632, 548, 1628, 548, 1632, 600, 5180, 4404, 4368, 548, 1628, 548, 548, 544, 1636, 548, 1628, 548, 548, 544, 548, 544, 1632, 548, 548, 544, 548, 548, 1632, 552, 544, 544, 548, 544, 1632, 548, 1628, 548, 548, 548, 1632, 548, 548, 548, 1628, 548, 1632, 548, 1628, 548, 1632, 548, 544, 548, 1632, 548, 1632, 548, 1636, 548, 544, 548, 548, 544, 548, 548, 544, 548, 1628, 548, 544, 548, 548, 548, 1636, 544, 1632, 548, 1628, 548, 548, 544, 548, 544, 548, 544, 548, 548, 548, 544, 548, 548, 544, 548, 544, 548, 1636, 548, 1628, 548, 1628, 548, 1632, 548, 1632, 600}; 
  
WiFiClient client;

//Atribuição dos tópicos de Publish e Subscribe
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe Luz_quarto = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Luz_quarto", MQTT_QOS_1);
Adafruit_MQTT_Subscribe liga_desliga_ar = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/liga_desliga_ar", MQTT_QOS_1);
Adafruit_MQTT_Subscribe aumenta_temperatura = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/aumenta_temperatura", MQTT_QOS_1);
Adafruit_MQTT_Publish temperatura_medida = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperatura_medida", MQTT_QOS_1);
Adafruit_MQTT_Publish umidade_medida = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/umidade_medida", MQTT_QOS_1);

//Função para ligar e desligar a iluminacao
void digitalCallback(char *data, uint16_t len) 
    {
      String message = String(data);
      message.trim();
      if (message == "ON")  {digitalWrite(12, HIGH);}
      if (message == "OFF") {digitalWrite(12, LOW);} 
    }

//Função para ligar e desligar o ar condicionado
void ArCallback(char *data, uint16_t len) 
    {
      String message = String(data);
      message.trim();
        if (message == "ligado")    {irsend.sendRaw(ligado,199,38);}
        if (message == "desligado") {irsend.sendRaw(desligado,199,38);}
    }

//Função para alterar Setpoint de temperatura do Ar condicionado    
void tempCallback(char *data, uint16_t len) 
    {
      String message = String(data);
      message.trim();
        if (message == "19"){irsend.sendRaw(liga19,199,38);}   
          else if (message == "20") {irsend.sendRaw(ligado,199,38);}
          else if (message == "21") {irsend.sendRaw(liga21,199,38);} 
          else if (message == "22") {irsend.sendRaw(liga22,199,38);}
          else if (message == "23") {irsend.sendRaw(liga23,199,38);}    
    }        

//Função para saida analogica do emissor de IR
void analogicaCallback(char *data, uint16_t len) 
    {
      String message = String(data);
      message.trim();
      analogWrite(13, message.toInt());
    }

//Função para leitura da temperatura e umidade do ambiente
void readSensor() 
    {  
      if(millis() - lastMillis > interval && mqtt.connected()) 
        {
          int humidity_data = (int)dht.readHumidity();
          int temperature_data = (int)dht.readTemperature();
          Serial.print("Temperatura: ");
          Serial.println(temperature_data);
          if (temperature_data != temperature_anterior)
            {
              temperatura_medida.publish(temperature_data);
              temperature_anterior = temperature_data;
            }
          if (humidity_data != humidity_anterior)
            {
              umidade_medida.publish(humidity_data);
              humidity_anterior = humidity_data;           
            }        
          lastMillis = millis();
        }
    } 

//Declaração de usuário e senha para conexão da rede Wifi Local
const char *ssid = "xxxxxxxxxx"; //Nome da rede de internet Local
const char *password = "xxxxxxxxxxxxxxx"; //Senha da rede de internet Local

//-------------------------------------------------------------------------

void setup() 
    {
      irsend.begin();
      dht.begin();
      
      pinMode(13, OUTPUT); // D7 saída analógica
      analogWrite(13, 0); // analogWrite(pin, value);
      pinMode(12, OUTPUT); // D6 saída digital
      digitalWrite(12, LOW);
      pinMode(3, OUTPUT); //LED MQTT
      digitalWrite(3, LOW);
      pinMode(7, OUTPUT); //LED WIFI
      digitalWrite(7, LOW);
         
     // pinMode(kIrLed, OUTPUT); // D5
      Serial.begin(115200);
      Serial.println("");

      // Conexión WIFI
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) 
        { 
          delay(500);
          Serial.print(".");
          digitalWrite(7, LOW);
        }
          digitalWrite(7, HIGH);

       Luz_quarto.setCallback(digitalCallback);
       liga_desliga_ar.setCallback(ArCallback);
       aumenta_temperatura.setCallback(tempCallback);
       mqtt.subscribe(&Luz_quarto);
       mqtt.subscribe(&liga_desliga_ar);
       mqtt.subscribe(&aumenta_temperatura);
  
     }

//--------------------------LOOP--------------------------------
void loop() 
    {
       MQTT_connect();
       mqtt.processPackets(500);
       readSensor();
    }

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) 
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) 
    {
      Serial.println(mqtt.connectErrorString(ret));
      Serial.println("Retrying MQTT connection in 10 seconds...");
      digitalWrite(3, LOW);
      mqtt.disconnect();
      delay(10000);  // wait 10 seconds
      retries--;
       if (retries == 0) 
        {
         while (1);
        }
    }
  Serial.println("MQTT Connected!");
  digitalWrite(3, HIGH);
}
