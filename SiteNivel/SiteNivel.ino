/*
   Aquaponia HAP
   Segundo site para a coleta de dados pela internet do nosso sistema de aquaponia
   que fica na escola estadual Hermelina de Albuquerque Passarella, Mairiporã - SP
   Página do Facebook: @AquaponiaHAP
   Site: AquaponiaHAP.com.br

   @author Brenon Kalevi  Email: guilherme.brenon@gmail.com   Whats: (11)956527593
   @author Leandro Derwin Email: ldsleandro@gmail.com         Whats: (11)942541760

*/

#include <SPI.h>
#include <Ethernet.h>

int boiaCM = 7;
int boiaMB = 6;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xCD };
IPAddress ip(192, 168, 0, 17);
EthernetServer server(80);

void setup() {
  Ethernet.begin(mac, ip);
  server.begin();
  
  pinMode(boiaCM, INPUT);
  pinMode(boiaMB, INPUT);
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

          client.println("<!DOCTYPE html>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Aquaponia HAP</title>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1><font color=#4279c7>Aquaponia HAP seg site</font></h1>");
          client.println("<hr/>");
          client.println("<h1>Nivel da &Aacute;gua</h1>");
          NivelCM(client);
          client.println("<br/>");
          client.println("<h1>Nivel da &Aacute;gua</h1>");
          NivelMB(client);
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

}

void NivelCM(EthernetClient client_aux) {

  int estado = digitalRead(boiaCM);
  Serial.print("Captacao de agua: ");
  Serial.println(estado);
  switch (estado)
  {
    case 0:
      // Fica vermelho o led pois a caixa estaria cheia e a bomba seria ligada para passar a água para outro lugar.
      Serial.println("Nivel Maximo");
      break;
    case 1:
      // Fica verde o led pois a caixa estaria enchendo
      Serial.println("Nivel Medio");

  }

}

void NivelMB(EthernetClient client_aux) {

  int estado2 = digitalRead(boiaMB);
  Serial.print("Captacao de agua: ");
  Serial.println(estado2);
  switch (estado2)
  {
    case 0:
      // Fica vermelho o led pois a caixa estaria cheia e a bomba seria ligada para passar a água para outro lugar.
      Serial.println("Nivel Medio");
      break;
    case 1:
      // Fica verde o led pois a caixa estaria enchendo
      Serial.println("Nivel Baixo!!");

  }


}
