char buffer1;            //Armazena o caracter recebido
 
void setup()
{
  //Define os pinos dos leds como saida
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(38400);
}
 
void loop()
{
  while(Serial.available() > 0)
  {
    buffer1 = Serial.read();
    if (buffer1 == 'A')    //Caso seja recebido A, acende o led 1 e confirma com mensagem
    {
      digitalWrite(3, HIGH);
      Serial.println("LED 1 ligado !");
    }
    if (buffer1 == 'B')    //Caso seja recebido B, acende o led 2 e confirma com mensagem
    {
      digitalWrite(4, HIGH);
      Serial.println("LED 2 ligado !");
    }

  }
}
