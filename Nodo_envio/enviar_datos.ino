#include "DHT.h"

#define DHTPIN 5     // Pin donde está conectado el sensor

//#define DHTTYPE DHT11   // Descomentar si se usa el DHT 11
#define DHTTYPE DHT22   // Sensor DHT22
#include <SPI.h>
#include <RH_RF95.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int s_analogica_mq135=A2;
int aire =12;
int CO2=11;
int gas=10;
int SensorPin2 = A0;
RH_RF95 rf95;
DHT dht(DHTPIN, DHTTYPE);
float frequency = 923.9; 
int SensorPin = A1;

float temperature,humidity,tem,hum,humedadsuelo,gas2,bebederos;
char tem_1[8]={"\0"},hum_1[8]={"\0"},humsuelo_1[8]={"\0"},gas_1[8]={"\0"},bebederos_1[8]={"\0"};
char *node_id = "<5680>";  //From LG01 via web Local Channel settings on MQTT.Please refer <> dataformat in here.
uint8_t datasend[36];
uint8_t datasend1[36];
uint8_t datasend2[36];
unsigned int count = 1;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Start MQTT Example"));
  dht.begin();
   if (!rf95.init())
      Serial.println(F("init failed"));
         rf95.setFrequency(frequency);
         rf95.setTxPower(13);
         rf95.setSyncWord(0x34);

 
}
long tiempoUltimaLectura=0;; //Para guardar el tiempo de la última lectura
void dhtTem() {
  //---------Lectura del Sensor--------------------------
  if(millis()-tiempoUltimaLectura>2000)
  {    
       humidity = dht.readHumidity(); //Leemos la Humedad
       hum = humidity*1.0;
       temperature = dht.readTemperature(); //Leemos la temperatura en grados Celsius
       tem = temperature*1.0;
       humedadsuelo= (100 - map(analogRead(SensorPin),0,1023,0,100));
       bebederos = (0 + map(analogRead(SensorPin2),0,1023,0,100));
       s_analogica_mq135 = analogRead(A2);  
       gas2 =   s_analogica_mq135;
      float f = dht.readTemperature(true); //Leemos la temperatura en grados Fahrenheit
      //--------Enviamos las lecturas por el puerto serial-------------
      Serial.println(F("The temperature and humidity:"));
      Serial.print("Humedad ");
      Serial.print(hum);
      Serial.print(" %t");
      Serial.print("Temperatura: ");
      Serial.print(tem);
      Serial.print(" *C ");
       Serial.println(("humedad del suelo: "));
      Serial.println(humedadsuelo);
      Serial.println("calidad aire: ");
      Serial.println(s_analogica_mq135, DEC);
      Serial.println("ppm");
      Serial.println("Nivel agua bebederos: ");
      Serial.println(bebederos);
       if(s_analogica_mq135>=56 && s_analogica_mq135<=65)
  {
  Serial.println("aire con un poco de CO2"); 
  delay(50);
  }
   if(s_analogica_mq135>=74 && s_analogica_mq135<=350)
  {
  Serial.println(" dioxido de carbono CO2");
  delay(50);
  }
  if(s_analogica_mq135>=400)
  {
  Serial.println("propano metano");
  delay(50);
  }
      //Serial.print(f);
      //Serial.println(" *F");
      tiempoUltimaLectura=millis(); //actualizamos el tiempo de la última lectura
  }
  //----Fin de la lectura---------------------------
  
}
void dhtWrite()
{
    char data[50] = "\0";
    for(int i = 0; i < 50; i++)
    {
       data[i] = node_id[i];
    }

    dtostrf(tem,0,1,tem_1);
   dtostrf(hum,0,1,hum_1);
 
     strcat(data,"field1=");
     strcat(data,tem_1);
     strcat(data,"&field2=");
     strcat(data,hum_1);
     strcpy((char *)datasend,data);
     
   //Serial.println((char *)datasend);
    //Serial.println(sizeof datasend);
      
}
 void dhtWrite1()
{
    char data1[50] = "\0";
    for(int i = 0; i < 50; i++)
    {
       data1[i] = node_id[i];
    }

  
    dtostrf(humedadsuelo,0,1,humsuelo_1);
    dtostrf(gas2,0,1,gas_1);
   
     strcat(data1,"&field3=");
   strcat(data1,humsuelo_1);
    strcat(data1,"&field4=");
   strcat(data1,gas_1);
     strcpy((char *)datasend1,data1);
     
}

   void dhtWrite2()
{
    char data2[50] = "\0";
    for(int h = 0; h < 50; h++)
    {
       data2[h] = node_id[h];
    }

    dtostrf(bebederos,0,1,bebederos_1);
 
     strcat(data2,"field5=");
     strcat(data2,bebederos_1);
     strcpy((char *)datasend2,data2);
     
   //Serial.println((char *)datasend);
    //Serial.println(sizeof datasend);
      
}
void SendData()
{
      Serial.println(F("Sending data to LG01"));
           
   
      rf95.send((char *)datasend,sizeof(datasend));  
      rf95.waitPacketSent();  // Now wait for a reply
    
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);

     if (rf95.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
     
      Serial.print("got reply from LG01: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is LoRa server running?");
  }
  delay(5000);
}
void SendData1()
{
      Serial.println(F("Sending data to LG01")); 
      rf95.send((char *)datasend1,sizeof(datasend1));   
      rf95.waitPacketSent();  // Now wait for a reply
    
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);

     if (rf95.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
     
      Serial.print("got reply from LG01: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is LoRa server running?");
  }
  delay(5000);
}
void SendData2()
{
      Serial.println(F("Sending data to LG01"));

      rf95.send((char *)datasend2,sizeof(datasend2));    
      rf95.waitPacketSent();  // Now wait for a reply
    
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);

     if (rf95.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
     
      Serial.print("got reply from LG01: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is LoRa server running?");
  }
  delay(5000);
}
void loop()
{
    Serial.print("###########    ");
    Serial.print("COUNT=");
    Serial.print(count);
    Serial.println("    ###########");
     count++;
     dhtTem();
     dhtWrite();
     dhtWrite1();
     dhtWrite2();
     SendData();
     SendData1();
     SendData2();
}
