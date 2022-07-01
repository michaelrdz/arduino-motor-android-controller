#include <LCD_I2C.h>
#include <DHT.h>
#define DHTPIN 9
#define DHTTYPE DHT11
//Declarar pines de motor
#define PIN_mot_1 8
int PIN_mot_2 = 7;
int PIN_mot_v = 6;
int v=150; //Valor de la velocidad del motor (debe estar entre 0 y 255)
LCD_I2C lcd(0x20);
LCD_I2C lcd2(0x24);
DHT dht(DHTPIN,DHTTYPE); //dht.readHumidit(); dht.readTemperature(); isnan()

void setup() {
  Serial.begin(9600);  //Configuramos velocidad del puerto
  pinMode(PIN_mot_1,OUTPUT);
  pinMode(PIN_mot_2,OUTPUT);
  void aumentarVel();  
  dht.begin();
  lcd.begin();
  lcd.backlight();
  lcd2.begin();
  lcd2.backlight();
  lcd.setCursor(0, 0); // se puede de 0 a 15 y de 0 a 1
  lcd.print("Control de motor!");
  lcd.setCursor(0, 1); // se puede de 0 a 15 y de 0 a 1
  lcd.print("por puerto serie");
  lcd2.setCursor(0, 0); // se puede de 0 a 15 y de 0 a 1
  lcd2.print("IDEGS 9C");
  
  analogWrite(PIN_mot_v,v);
  digitalWrite(PIN_mot_1,HIGH);
  digitalWrite(PIN_mot_2,LOW);
  menu();
}

void loop() {
  //Los pines analogicos tienen una resolución de 10 bits
  //esto equivale a un rango de 0 a 1023

  //v_recibido = (valor(PIN_S0)/1023)*5 <--- Los resultados son enteros
  //v_recibido = (valor(PIN_S0)/1023.0)*(5.0) <--- Los resultados son flotantes

  //el lm35 varia 10mV por cada grado centigrado si tenemos 27C -> voltaje=27*10mV=
  if(Serial.available()>0) {
    char dato=Serial.read();
    switch(dato) {
      case 'a': aumentarVel();
               break; 
      case 'd':  disminuirVel();
               break; 
      case 'r': giroDerecha();
               break;
      case 'l': giroIzquierda();
               break;
      case 's': detenerMot();
               break;
      case 't': leerLM35();
              break;
      case 'w':leerDHT11();
              break;
     default: break;
    }
  }
}

void aumentarVel() {
  lcd.setCursor(0,0);
               lcd.print("(x)v+(d)v-(r)der");
               lcd.setCursor(0,1);
               lcd.print("(l) izq (s)det");
               v=v+10;
               if(v>250) {
                v=250;
                 Serial.print("\fvel maxima");
               }
               else {
                Serial.print("\faumentando vel");
               }
               analogWrite(PIN_mot_v, v); 
}

void disminuirVel() {
  lcd.setCursor(0,0);
               lcd.print("(a)v+(x)v-(r)der");
               lcd.setCursor(0,1);
               lcd.print("(l) izq (s)det");
               v=v-10;
               if(v<0) {
                v=0;
                 Serial.print("\fvel minima");
               }
               else {
                Serial.print("\fdisminuyendo vel");
               }
               analogWrite(PIN_mot_v, v);
}

void giroDerecha() {
  lcd.setCursor(0,0);
               lcd.print("(a)v+(d)v-(x)der");
               lcd.setCursor(0,1);
               lcd.print("(l) izq (s)det");
               Serial.print("\fgiro derecha");
               digitalWrite(PIN_mot_1, LOW);
               digitalWrite(PIN_mot_2, HIGH);
}

void giroIzquierda() {
  lcd.setCursor(0,0);
               lcd.print("(a)v+(d)v-(r)der");
               lcd.setCursor(0,1);
               lcd.print("(x) izq (s)det");
               Serial.print("\fgiro izquierda");
               digitalWrite(PIN_mot_1, HIGH);
               digitalWrite(PIN_mot_2, LOW);
}

void detenerMot() {
    lcd.setCursor(0,0);
               lcd.print("(a)v+(d)v-(r)der");
               lcd.setCursor(0,1);
               lcd.print("(l) izq (x)det");
               Serial.print("\fdeteniendo ");
               digitalWrite(PIN_mot_1, HIGH);
               digitalWrite(PIN_mot_2, HIGH); 
}

void leerLM35() {
  lcd.setCursor(0,0);
              lcd.print("(a)v+(d)v-(r)der");
               lcd.setCursor(0,1);
               lcd.print("(l)izq(r)der(x)temp");
               Serial.print("\fleer temperatura");
               int valor =analogRead(A0);
               float voltaje=(valor*5.0)/1023.0;
               float temperatura=(int)(voltaje*100.0);
               lcd2.setCursor(0,0);
               lcd2.print("val=");
               lcd2.print(valor);
               Serial.print("Val= ");
               Serial.print(valor);
               lcd2.print("vol=");
               lcd2.print(voltaje);
               Serial.print("Volt= ");
               Serial.print(voltaje);
               lcd2.setCursor(0,1);
               lcd2.print("Temperatura=");
               lcd2.print(temperatura);
               Serial.print("Temp= ");
               Serial.print(temperatura);
}

void leerDHT11() {
  Serial.print("Temeratura y humedad desde DHT");
                byte RH=dht.readHumidity();
                byte Temp=dht.readTemperature();
                if(isnan(RH) || isnan(Temp)) {
                  lcd2.clear();
                  lcd2.setCursor(5,0);
                  lcd2.print("ERROR");
                  return;
                }
                else {
                  char temperature[]="Temp=00.0 C";
                  char humidity[]="RH=00.0%";
                  temperature[5]=Temp/10+48;
                  temperature[6]=Temp%10+48;
                  temperature[9]=Temp%10+48;
                  lcd2.clear();
                  lcd2.setCursor(0,0);
                  lcd2.print(temperature);
                  Serial.println(temperature);
                  humidity[3]=RH/10+48;
                  humidity[4]=RH%10+48;
                  lcd.setCursor(0,1);
                  lcd2.print(humidity);
                  Serial.println(humidity);
                }
}

void menu() {
   Serial.println("Control de motor");
   Serial.println("a para aumentar velocidad");
   Serial.println("d para disminuir velocidad");
   Serial.println("r para giro derecha");
   Serial.println("l para giro izquierda");
   Serial.println("s detener");
   Serial.println("t leer temperatura");
   Serial.println("w leer temp/humedad");
}
