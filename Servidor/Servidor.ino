/*
   Aquaponia HAP
   Segundo site para a coleta de dados pela internet do nosso sistema de aquaponia
   que fica na escola estadual Hermelina de Albuquerque Passarella, Mairiporã - SP/
   Página do Facebook: @Aquaponia HAP
   Site: AquaponiaHAP.com.br

   @author Brenon Kalevi  Email: guilherme.brenon@gmail.com   Whats: (11)956527593
   @author Leandro Derwin Email: ldsleandro@gmail.com         Whats: (11)942541760

*/
//BIBLIOTECAS
#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h> //INCLUSÃO DE BIBLIOTECA
#include <DallasTemperature.h> //INCLUSÃO DE BIBLIOTECA
#include <DHT.h> //Carrega a biblioteca DHT
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <DS3231.h>

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

//DEFINIÇÃO DOS PINOS
#define pinSensorD2 25
#define pinSensorD 8
#define DS18B20 7 //DEFINE O PINO DIGITAL UTILIZADO PELO SENSOR
#define DHTPIN 6
#define DHTTYPE DHT22

//VARIÁVEIS
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 16);
EthernetServer server(80);
OneWire ourWire(DS18B20); //CONFIGURA UMA INSTÂNCIA ONEWIRE PARA SE COMUNICAR COM O SENSOR
DallasTemperature sensors(&ourWire); //BIBLIOTECA DallasTemperature UTILIZA A OneWire
DHT dht(DHTPIN, DHTTYPE);
DS3231  rtc(SDA, SCL);

unsigned int pingSpeed = 50; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;     // Holds the next ping time.




void setup()
{

  Ethernet.begin(mac, ip);
  server.begin();
  sensors.begin(); //INICIA O SENSOR
  delay(1000);
  pinMode(pinSensorD, INPUT);
  pinMode(pinSensorD2, INPUT);
  dht.begin();
  rtc.begin();
  if (!tsl.begin())
  {

    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  pingTimer = millis(); // Start now.



  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);

}

void loop() {

  EthernetClient client = server.available();

  if (client)
  {
    boolean currentLineIsBlank = true;
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();

        if (c == '\n' && currentLineIsBlank) {

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");

          client.println();

          client.println("<!DOCTYPE html>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Aquaponia HAP</title>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1><font color=#4279c7>Aquaponia HAP</font></h1>");
          client.println("<hr/>");
          client.println("<h1>Rel&oacute;gio</h1>");
          Relogio(client);
          client.println("<h1>Temperatura da &Aacute;gua</h1>");
          SensorDeTemperatura(client);
          client.println("<h1>Chuva</h1>");
          SensorDeChuva(client);
          client.println("<h1>&Aacute;gua na cama de cultivo</h1>");
          SensorDeUmidadeDoSolo(client);
          client.println("<br/>");
          client.println("<h1>Temperatura ambiente</h1>");
          SensorDeTemperaturaAr(client);
          client.println("<br/>");
          client.println("<h1>Umidade do ar</h1>");
          SensorUmidade(client);
          client.println("<h1>Luminosidade</h1>");
          Luminosidade(client);
          client.println("<br/>");
          client.println("</body>");
          client.println("</html>");
          break;
        }

        if (c == '\n')
        {
          currentLineIsBlank = true;
        }
        else if (c != '\r')
        {
          currentLineIsBlank = false;
        }
      }
    }

    delay(1);
    client.stop();

  }

  digitalWrite(26, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(27, HIGH);    // turn the LED off by making the voltage LOW
  delay(100);

  digitalWrite(26, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(27, LOW);    // turn the LED off by making the voltage LOW
  delay(100);
}

void SensorDeChuva(EthernetClient client_aux) {

  if (digitalRead(pinSensorD)) {
    client_aux.print("Sem chuva ");
  } else {
    client_aux.print("Est&aacute; chovendo ");
  }

}

void Luminosidade(EthernetClient client_aux) {
  sensors_event_t event;
  tsl.getEvent(&event);


  if (event.light)
  {
    client_aux.print(event.light); client_aux.println(" lux");
  }
  else
  {
    client_aux.println("Escuro");
  }
  delay(250);


}

void Relogio(EthernetClient client_aux) {

  client_aux.print("Hora:  ");
  client_aux.println(rtc.getTimeStr());

  client_aux.print("Data: ");
  client_aux.println(rtc.getDateStr());

  delay(1000);
}

void SensorDeTemperatura(EthernetClient client_aux) {

  sensors.requestTemperatures();//SOLICITA QUE A FUNÇÃO INFORME A TEMPERATURA DO SENSOR
  client_aux.print("Temperatura do tanque de peixes: "); //IMPRIME O TEXTO NA SERIAL
  client_aux.print(sensors.getTempCByIndex(0)); //IMPRIME NA SERIAL O VALOR DE TEMPERATURA MEDIDO
  client_aux.println("*C"); //IMPRIME O TEXTO NA SERIAL
  delay(250);//INTERVALO DE 250 MILISSEGUNDOS

}

void SensorDeTemperaturaAr(EthernetClient client_aux) {

  float t = dht.readTemperature(); //Le o valor da temperatura
  client_aux.print("Temp : ");
  client_aux.print(" ");
  client_aux.print(t, 1);
  client_aux.print("*C");
  delay(2000);
}

void SensorUmidade(EthernetClient client_aux) {
  float h = dht.readHumidity(); //Le o valor da umidade
  client_aux.print("Umid : ");
  client_aux.print(" ");
  client_aux.print(h, 1);
  client_aux.print("%");
  delay(2000);


}

void SensorDeUmidadeDoSolo(EthernetClient client_aux) {

  if (digitalRead(pinSensorD2)) {
    client_aux.print("Sem &Aacute;gua ");
  } else {
    client_aux.print("Com &Aacute;gua ");

  }
}
