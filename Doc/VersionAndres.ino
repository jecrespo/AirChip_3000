#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5); // RX, TX

int numPruebas = 5;

char* peticion[] = {"|{ 99RDD}\r","|{ ;99;RDD}\r","|{F0199RDD}\r","|{F0199RDD\r","",""}; // CAMBIO: \n por \r
char response[1024];

void setup() {
  pinMode(3,OUTPUT);
  Serial.begin(9600);
  Serial.println("Lectura HC2\n");
  mySerial.begin(19200);
}

void loop() {
   digitalWrite(3, HIGH); //Activar envio a la sonda a traves de interface
   //delay(100);                // ??Delay para permitir al dispositivo cambiar tras mandar el código.
   mySerial.write(peticion[0,1,2,3]);
   digitalWrite(3, LOW); // Desactivar envío a la sonda a traves de interface

   int i = 0;
   int timeout = 0;
   int MAX_TIMEOUT = 1000;
   char res = '\0';

   while(res != '\r'){
     res = mySerial.read();
     while(res == -1){
       res = mySerial.read();
       delay(100);
       timeout += 100;
       if(timeout >= MAX_TIMEOUT)
         Serial.println("Can't read");
     }
     response[i] = res;
     i++;
   }
   Serial.println(response);
/*
   for(int contador = 0; contador <= numPruebas; contador++){
     digitalWrite(3, HIGH); //Activar envio a la sonda a traves de interface
     delay(100);                // ??Delay para permitir al dispositivo cambiar tras mandar el código.
     mySerial.write(peticion[contador]);
     digitalWrite(3, LOW); // Desactivar envio a la sonda a traves de interface
     delay(100); // ***************** es posible que haya que probarlo con y sin delay o cambiar la duración ********************************
     char respuesta = mySerial.read();
     Serial.print(String(contador));
     Serial.println("\t");
     Serial.println(respuesta);
     Serial.println("\r");
   }*/
delay (500);
}