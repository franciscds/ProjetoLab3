#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>
#include <SoftwareSerial.h>
//  - - - - - - -- - - - - -  *Controlador Fuzzy* - - - - - - - - - -- - - - - - - - - - - //
//Variaveis para modelar Entrada do Comunicador Bluetooh
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

//  - - - - - - -- - - - - -  *Controlador Fuzzy* - - - - - - - - - -- - - - - - - - - - - //
Fuzzy* fuzzy = new Fuzzy();
//Angulo
FuzzySet* zero = new FuzzySet(-30, 0, 0, 30);
FuzzySet* pPequeno = new FuzzySet(0, 30, 30, 60);
FuzzySet* pMedio = new FuzzySet(60, 120, 120, 180);
FuzzySet* pGrande = new FuzzySet(120, 180, 180, 180);

FuzzySet* nPequeno = new FuzzySet(0, -30, -30, -60);
FuzzySet* nMedio = new FuzzySet(-60, -120, -120, -180);
FuzzySet* nGrande = new FuzzySet(-120, -180, -180, -180);

//distancia
FuzzySet* dmuitoPequena = new FuzzySet(0, 0, 0, 125);
FuzzySet* distanciaPequena = new FuzzySet(0, 125, 125, 250);
FuzzySet* distanciaMedia = new FuzzySet(125, 250, 250, 375);
FuzzySet* distanciaGrande = new FuzzySet(250, 375, 375, 500);
FuzzySet* distanciaMuitoGrande = new FuzzySet(375, 500, 500, 500);

//velocidade Vd
FuzzySet* vZero_d = new FuzzySet(0, 0, 0, 0);
FuzzySet* vBaixa_d = new FuzzySet(30, 30, 30, 30);
FuzzySet* vMedia_d = new FuzzySet(50, 50, 50, 50);
FuzzySet* vGrande_d = new FuzzySet(60, 60, 60, 60);
FuzzySet* vMuitoGrande_d = new FuzzySet(70,70, 70, 70);
//velocidade Ve
FuzzySet* vZero_e = new FuzzySet(0, 0, 0, 0);
FuzzySet* vBaixa_e = new FuzzySet(30, 30, 30, 30);
FuzzySet* vMedia_e = new FuzzySet(50, 50, 50, 50);
FuzzySet* vGrande_e = new FuzzySet(60, 60, 60, 60);
FuzzySet* vMuitoGrande_e = new FuzzySet(70,70, 70, 70);
void setup(){
  Serial.begin(9600);
  
  // FuzzyInput
  FuzzyInput* angulo = new FuzzyInput(1);
  angulo->addFuzzySet(zero);
  angulo->addFuzzySet(pPequeno);
  angulo->addFuzzySet(pMedio);
  angulo->addFuzzySet(pGrande);
  angulo->addFuzzySet(nPequeno);
  angulo->addFuzzySet(nMedio);
  angulo->addFuzzySet(nGrande);
  
  fuzzy->addFuzzyInput(distance);

  // FuzzyInput distancia
  FuzzyInput* dist = new FuzzyInput(2);
  dist->addFuzzySet(dmuitoPequena);
  dist->addFuzzySet(distanciaPequena);
  dist->addFuzzySet(distanciaMedia);
  dist->addFuzzySet(distanciaGrande);
  dist->addFuzzySet(distanciaMuitoGrande);

  fuzzy->addFuzzyInput(inputSpeed);

  // FuzzyOutput vd
  FuzzyOutputt* vd = new Fuzzyoutput(1);
  
  vd->addFuzzySet(vZero_d);
  vd->addFuzzySet(vBaixa_d);
  vd->addFuzzySet(vMedia_d);
  vd->addFuzzySet(vGrande_d);
  vd->addFuzzySet(vMuitoGrande_d);
  
  fuzzy->addFuzzyOutput(vd);


  // FuzzyOutput ve
  FuzzyOutputt* ve = new Fuzzyoutput(2);
  
  ve->addFuzzySet(vZero_e);
  ve->addFuzzySet(vBaixa_e);
  ve->addFuzzySet(vMedia_e);
  ve->addFuzzySet(vGrande_e);
  ve->addFuzzySet(vMuitoGrande_e);
  

  fuzzy->addFuzzyOutput(ve);
  //*********** distancia Muito Pequena*************
  // Building FuzzyRule  distancia MuitoPequena e Angulo Positivo Pequeno -> vd = baixa e ve = 0;
  FuzzyRuleAntecedent* distancePAndAnguloPequenoPositivo = new FuzzyRuleAntecedent();
  distancePAndAnguloPequenoPositivo->joinWithAND(dmuitoPequena, pPequeno);
  
  FuzzyRuleConsequent* thenVdisBmAndVeZ = new FuzzyRuleConsequent();
  thenVdisBmAndVeZ->addOutput(vBaixa_d);
  thenVdisBmAndVeZ->addOutput(vZero_e);

  FuzzyRule* fuzzyRule1 = new FuzzyRule(1, distancePAndAnguloPequenoPositivo, thenVdisBmAndVeZ);
  fuzzy->addFuzzyRule(fuzzyRule1);
  
  // 
  // Building FuzzyRule  distancia MuitoPequena e Angulo Positivo Medio -> vd = baixa e ve = 0;
  FuzzyRuleAntecedent* distanceMAndAnguloPequenoPositivo = new FuzzyRuleAntecedent();
  distanceMAndAnguloPequenoPositivo->joinWithAND(dmuitoPequena, pMedio);
  
  FuzzyRuleConsequent* thenVdisVmAndVeB = new FuzzyRuleConsequent();
  thenVdisVmAndVeB->addOutput(vMedia_d);
  thenVdisVmAndVeB->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule2 = new FuzzyRule(2, distanceMAndAnguloPequenoPositivo, thenVdisVmAndVeB);
  fuzzy->addFuzzyRule(fuzzyRule2);
  
  // Building FuzzyRule  distancia MuitoPequena e Angulo Positivo Grande -> vd = baixa e ve = 0;
  FuzzyRuleAntecedent* distancePAndAnguloPositivoGrande = new FuzzyRuleAntecedent();
  distancePAndAnguloPositivoGrande->joinWithAND(dmuitoPequena, pGrande);
  
  FuzzyRuleConsequent* thenVdisGmAndVeB = new FuzzyRuleConsequent();
  thenVdisGmAndVeB->addOutput(vGrande_d);
  thenVdisGmAndVeB->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule3 = new FuzzyRule(3, distancePAndAnguloPositivoGrande, thenVdisBmAndVeZ);
  fuzzy->addFuzzyRule(fuzzyRule3);
  
// if distancia Pequena e Angulo Negativo pequeno
   FuzzyRuleAntecedent* distancePequenaAndAnguloPequenoNegativo = new FuzzyRuleAntecedent();
  distanceCloseAndSpeedQuick->joinWithAND(distanciaPequena, nPequeno);
  
  FuzzyRuleConsequent* thenVdisBmAndVeZ = new FuzzyRuleConsequent();
  thenVdisBmAndVeZ->addOutput(vZero_d);
  thenVdisBmAndVeZ->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule2 = new FuzzyRule(2, distancePAndAnguloPequenoNegativo, thenVdisBmAndVeZ);
  fuzzy->addFuzzyRule(fuzzyRule2);
 
  
  void loop(){
    
   while(mySerial.available())   // enquanto serial estiver funcionando
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
    
    } i++;
   }//fecha o while do bluetooh
    
  
  fuzzy->setInput(1, 10);
  fuzzy->setInput(2, 30);
  fuzzy->setInput(3, -15);

  fuzzy->fuzzify();
  
  Serial.print("Distancia: ");
  Serial.print(dmuitoPequena->getPertinence());
  Serial.print(", ");
  Serial.print(distanciaMedia->getPertinence());
  Serial.print(", ");
  Serial.println(dDistanciaGrande->getPertinence());
  
  Serial.print("Angulo: ");
  Serial.print(zero->getPertinence());
  Serial.print(", ");
  Serial.print(pPequeno->getPertinence());
  Serial.print(", ");
  Serial.print(pMedio->getPertinence());
  Serial.print(", ");
  Serial.println(pGrande->getPertinence());
  
 
  float output1 = fuzzy->defuzzify(1);
  float output2 = fuzzy->defuzzify(2);
  
  Serial.print("Velocidade Roda Direita: ");
  Serial.print(output1);
  Serial.print(", Velocidade Roda Esquerda: ");
  Serial.println(output2);

  delay(100000);
}
