
// Declaração dos Pinos
int boiaCM = 7;
int boiaMB = 6;



void setup()
{
  Serial.begin(9600);
  pinMode(boiaCM, INPUT);
  pinMode(boiaMB, INPUT);


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

int estado2 = digitalRead(boiaMB);
  Serial.print("Captacao de agua: ");
  Serial.println(estado2);
  switch (estado2)
  {
   
    case 1:
      // Fica verde o led pois a caixa estaria enchendo
      Serial.println("Nivel Baixo!!");

  }




}

void loop()
{}
