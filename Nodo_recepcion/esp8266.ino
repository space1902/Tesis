#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>


#include <ESP8266WiFi.h>
//#include "FirebaseESP8266.h"





//MrLueees: cocina, electrónica, costura ...
//Hace 1 año
// TUTORIAL REGAR PLANTAS DESDE INTERNET CON ESP8266, FIREBASE Y APPINVENTOR.


//#include <FirebaseArduino.h>
//#include <ESP8266wifi.h>

//#include <ArduinoJson.h>

//#include <String>

// Definir Firebase e internet..
#define FIREBASE_HOST "tesis-8765b.firebaseio.com" // Poner la direccion del proyecto Firevase Sin "http://" y sin "/" final.
#define FIREBASE_AUTH "vjaI9do0VX0bO8jrwYU7nVXVd8gC4XPCVTAJiLk8" // buscar en la página de Firebase. Configuracion del proyecto Cuentas de servicio Secretos base de datos- mostrar- copiar y pegar aqui con las comillas.
#define WIFI_SSID "alv-we" // nombre del wifi
#define WIFI_PASSWORD "1090525663" // contraseña del wifi


//Las variables a utilizar.
int  humedad;

int  ENCENDER = 1;

int  ASPERSOR;

String limite;


void setup() {
  Serial.begin(115200);

 

pinMode(A0, INPUT); //Aqui conectaremos el pin de control del sensor de humedad.
pinMode(15, OUTPUT); // conectamos al relé.(15 =D8)
 pinMode(16, OUTPUT); 

  // conectamos al wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
//  Firebase.reconnectWiFi(true);
}



void loop() {
  
  humedad = (100 - map(analogRead(A0),0,1023,0,100)); // Con esto conseguimos el porcentage de humedad en tantos por cien (el 1.45 puede variar segun el sensor de humedad).


  // Monitorizar los parametros.
  Serial.println("....");
  Serial.print(humedad);Serial.println (" %");
  Serial.println(".....");
  Serial.print("ON/Off: "); Serial.print(ENCENDER);
    Serial.println("....");
  Serial.print("ON/Off: "); Serial.print(ASPERSOR);
Serial.println("....");


  
  //Esto crea una etiqueta en firebase y le asigna el valor
  
  Firebase.setInt("humedad", humedad); // entre parentesis primero se pone la etiqueta entre comillas, despues una "," y despues el valor que se le asigna a la etiqueta.
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);


  
//Coger datos

 ENCENDER = Firebase.getInt("ENCENDER"); // Para encender o apagar el motor1.
 ASPERSOR = Firebase.getInt("ASPERSOR"); //Para encender o apagar el motor2.
 
 
 
// Acciones

     //Cuando pulsamos boton On se encenderá el motor.
    if ((ENCENDER == 1)) {
      digitalWrite(15, HIGH);
      }
     //Cuando pulsamos boton Off se apagará el motor.
    if ((ENCENDER == 0)) {
      digitalWrite(15, LOW);}
    //Cuando pulsamos boton On se encenderá el motor.
    if ((ASPERSOR == 1)) {
      digitalWrite(16, HIGH);
      }
     //Cuando pulsamos boton Off se apagará el motor.
    if ((ASPERSOR == 0)) {
      digitalWrite(16, LOW);}
    
 }
