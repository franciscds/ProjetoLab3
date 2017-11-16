char buffer1[12];            //Armazena o caracter recebido
char aid_n;
char ax_n[4];
char ay_n[4];
char aphi_n[3];
char aid;
char ax[4];
char ay[4];
char aphi[3];
int i = 0;
int j, id, x, y, phi, id_n, x_n, y_n, phi_n;
String nos;
String marcador;
 
void setup()
{
  Serial.begin(38400);
}
 
void loop()
{
  while(Serial.available() > 0)          // enquanto serial estiver funcionando
  {
    
    buffer1[i] = Serial.read();          // lê até recebidos até uma mensagem estar completa
    if (buffer1[i] == '\n')
    {
      if (buffer1[0] == 'C')             // se for o nosso marcador ele salva as informações nas variáveis _n
      {
        nos = buffer1;
        for (j = 0; j < 12; j++)
        {
          if (i == 0)                    // salva nosso id    
          {
            aid_n = nos[i];
          }
              
          if (i >= 1 || i <= 4)          // salva nosso x
          {
            ax_n[i] = nos[i];
          }
              
           if (i >= 5 || i <= 8)        // salva nosso y
           {
             ay_n[i] = nos[i];
           }
               
           if (i >= 9 || i <= 11)       // salva nosso ângulo
           {
             aphi_n[i] = nos[i];
           }
          }
          id_n = (int)aid_n;
          x_n = (int)ax_n;
          y_n = (int)ay_n;
          phi_n = (int)aphi_n;
        i = -1;                        // no final do loop ele incrementa i, o -1 vai fazer o i ir para depois que incrementar para iniciar o recebiento de uma nova mensagem
      }
      
      if (buffer1[0] != 'C' && buffer1[0] != 0)    // se for outro marcador ele salva as informações nas variáveis
      {
        marcador = buffer1;
        for (j = 0; j < 12; j++)
        {
          if (i == 0)                  // salva id de outro  
          {
            aid = marcador[i];
          }
    
          if (i >= 1 || i <= 4)       // salva outro x
          {
            ax[i] = marcador[i];
          }
    
           if (i >= 5 || i <= 8)      // salva y de outro
           {
             ay[i] = marcador[i];
           }
    
           if (i >= 9 || i <= 11)      // salva ângulo de outro
           {
             aphi[i] = marcador[i];
           }
        }
        id = (int)aid;
        x = (int)ax;
        y = (int)ay;
        phi = (int)aphi;
        i = -1;
      }
    
    }
    i++;
    }
    
    
  }
  
  
  
