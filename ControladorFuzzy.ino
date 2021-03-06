
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

SoftwareSerial mySerial(10,11);
//  - - - - - - -- - - - - -  *Controlador Fuzzy* - - - - - - - - - -- - - - - - - - - - - //
Fuzzy* fuzzy = new Fuzzy();
//Angulo
FuzzySet* zero = new FuzzySet(-30, 0, 0, 30);
FuzzySet* pPequeno = new FuzzySet(0, 30, 30, 60);
FuzzySet* pMedio = new FuzzySet(60, 120, 120, 180);
FuzzySet* pGrande = new FuzzySet(120, 180, 180, 180);

FuzzySet* nPequeno = new FuzzySet(-60, -30, -30, 0);
FuzzySet* nMedio = new FuzzySet(-180, -120, -120, -60);
FuzzySet* nGrande = new FuzzySet(-180, -180, -180, -120);

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
  
  pinMode(OUTPUT,3);
  pinMode(OUTPUT,4);

  mySerial.begin(38400);
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  
  // FuzzyInput
  FuzzyInput* angulo = new FuzzyInput(1);
  angulo->addFuzzySet(zero);
  angulo->addFuzzySet(pPequeno);
  angulo->addFuzzySet(pMedio);
  angulo->addFuzzySet(pGrande);
  angulo->addFuzzySet(nPequeno);
  angulo->addFuzzySet(nMedio);
  angulo->addFuzzySet(nGrande);
  
  fuzzy->addFuzzyInput(angulo);

  // FuzzyInput distancia
  FuzzyInput* dist = new FuzzyInput(2);
  dist->addFuzzySet(dmuitoPequena);
  dist->addFuzzySet(distanciaPequena);
  dist->addFuzzySet(distanciaMedia);
  dist->addFuzzySet(distanciaGrande);
  dist->addFuzzySet(distanciaMuitoGrande);

  fuzzy->addFuzzyInput(dist);

  // FuzzyOutput vd
  FuzzyOutput* vd = new FuzzyOutput(1);
  
  vd->addFuzzySet(vZero_d);
  vd->addFuzzySet(vBaixa_d);
  vd->addFuzzySet(vMedia_d);
  vd->addFuzzySet(vGrande_d);
  vd->addFuzzySet(vMuitoGrande_d);
  
  fuzzy->addFuzzyOutput(vd);


  // FuzzyOutput ve
  FuzzyOutput* ve = new FuzzyOutput(2);
  
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
  
    // Building FuzzyRule  distancia Muito Pequena e Angulo Zero -> vd = ve =Baixa;
  FuzzyRuleAntecedent* distanceMPAndAnguloZero = new FuzzyRuleAntecedent();
  distanceMPAndAnguloZero->joinWithAND(dmuitoPequena, zero);
  
  FuzzyRuleConsequent* thenVdisBAndVeismB = new FuzzyRuleConsequent();
  thenVdisBAndVeismB->addOutput(vBaixa_d);
  thenVdisBAndVeismB->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule4 = new FuzzyRule(4, distancePAndAnguloZero, thenVdisBAndVeismB);
  fuzzy->addFuzzyRule(fuzzyRule4);
  /* - - - - -- -  - -- - - - - - - - - - Fuzzy Rule distancia Pequena -----------------------*/
  
  // Building FuzzyRule  distancia Pequena e Angulo Zero -> vd = ve =Baixa;
  FuzzyRuleAntecedent* distancePAndAnguloZero = new FuzzyRuleAntecedent();
  distancePAndAnguloZero->joinWithAND(dPequena, zero);
  
  FuzzyRuleConsequent* thenVdisBAndVeisB = new FuzzyRuleConsequent();
  thenVdisBAndVeisB->addOutput(vBaixa_d);
  thenVdisBAndVeisB->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule5 = new FuzzyRule(5, distancePAndAnguloZero, thenVdisBAndVeisB);
  fuzzy->addFuzzyRule(fuzzyRule5);
  
  // Building FuzzyRule  distancia Pequena e Angulo Positivo Pequeno -> vd = ve =Baixa;
  FuzzyRuleAntecedent* distancePAndAnguloPPequeno = new FuzzyRuleAntecedent();
  distancePAndAnguloPPequeno->joinWithAND(distanciaPequena, pPequeno);
  
  FuzzyRuleConsequent* thenVdisBAndVeisZ = new FuzzyRuleConsequent();
  thenVdisBAndVeisZ->addOutput(vBaixa_d);
  thenVdisBAndVeisZ->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule6 = new FuzzyRule(6, distancePAndAnguloPPequeno, thenVdisBAndVeisZ);
  fuzzy->addFuzzyRule(fuzzyRule6);
  
  // Building FuzzyRule  distancia Pequena e Angulo Positivo Medio -> vd = Media  ve =Baixa;
  FuzzyRuleAntecedent* distancePAndAnguloPMedio = new FuzzyRuleAntecedent();
  distancePAndAnguloPMedio->joinWithAND(distanciaPequena, pMedio);
  
  FuzzyRuleConsequent* thenVdisMAndVeisB = new FuzzyRuleConsequent();
  thenVdisMAndVeisB->addOutput(vMedio_d);
  thenVdisMAndVeisB->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule7 = new FuzzyRule(7, distancePAndAnguloPMedio, thenVdisMAndVeisB);
  fuzzy->addFuzzyRule(fuzzyRule7);
  
  // Building FuzzyRule  distancia Pequena e Angulo Positivo Grande -> vd = baixa  ve =Média;
  FuzzyRuleAntecedent* distancePAndAnguloPGrande = new FuzzyRuleAntecedent();
  distancePAndAnguloPGrande->joinWithAND(distanciaPequena, pGrande);
  
  FuzzyRuleConsequent* thenVdisBAndVeisM = new FuzzyRuleConsequent();
  thenVdisBAndVeisM->addOutput(vMedio_d);
  thenVdisBAndVeisM->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule8 = new FuzzyRule(8, distancePAndAnguloPGrande, thenVdisBAndVeisM);
  fuzzy->addFuzzyRule(fuzzyRule8);
  
  
  /* - - - - -- -  - -- - - - - - - - - - Fuzzy Rule distancia Media e Angulo Positivo -----------------------*/
  
  // Building FuzzyRule  distancia Media e Angulo Zero -> vd = ve = Média;
  FuzzyRuleAntecedent* distanceMAndAnguloZero = new FuzzyRuleAntecedent();
  distanceMAndAnguloZero->joinWithAND(distanciaMedia, zero);
  
  FuzzyRuleConsequent* thenVdisMAndVeisM = new FuzzyRuleConsequent();
  thenVdisMAndVeisM->addOutput(vMedia_d);
  thenVdisMAndVeisM->addOutput(vMedia_e);

  FuzzyRule* fuzzyRule9 = new FuzzyRule(9, distanceMAndAnguloZero, thenVdisMAndVeisM);
  fuzzy->addFuzzyRule(fuzzyRule9);
  
  // Building FuzzyRule  distancia Média e Angulo Positivo Pequeno -> vd=Baixa ve =media;
  FuzzyRuleAntecedent* distanceMAndAnguloPPequeno = new FuzzyRuleAntecedent();
  distanceMAndAnguloPPequeno->joinWithAND(distanciaMedia, pPequeno);
  
  FuzzyRuleConsequent* thenVdisBAndVeisM = new FuzzyRuleConsequent();
  thenVdisBAndVeisM->addOutput(vBaixa_d);
  thenVdisBAndVeisM->addOutput(vMedia_e);

  FuzzyRule* fuzzyRule10 = new FuzzyRule(10, distanceMAndAnguloPPequeno, thenVdisBAndVeisM);
  fuzzy->addFuzzyRule(fuzzyRule10);
  
  // Building FuzzyRule  distancia Média e Angulo Positivo Medio -> vd = Media  ve =Baixa;
  FuzzyRuleAntecedent* distanceMAndAnguloPMedio = new FuzzyRuleAntecedent();
  distanceMAndAnguloPMedio->joinWithAND(distanciaMedia, pMedio);
  
  FuzzyRuleConsequent* thenVdisBAndVeisM = new FuzzyRuleConsequent();
  thenVdisBAndVeisM->addOutput(vBaixa_d);
  thenVdisBAndVeisM->addOutput(vMedia_e);

  FuzzyRule* fuzzyRule11 = new FuzzyRule(11, distanceMAndAnguloPMedio, thenVdisBAndVeisM);
  fuzzy->addFuzzyRule(fuzzyRule11);
  
  // Building FuzzyRule  distancia Media e Angulo Positivo Grande -> vd = Media ve =Grande;
  FuzzyRuleAntecedent* distancePAndAnguloPGrande = new FuzzyRuleAntecedent();
  distancePAndAnguloPGrande->joinWithAND(distanciaPequena, pGrande);
  
  FuzzyRuleConsequent* thenVdisBAndVeisM = new FuzzyRuleConsequent();
  thenVdisBAndVeisM->addOutput(vMedio_d);
  thenVdisBAndVeisM->addOutput(vGrande_e);

  FuzzyRule* fuzzyRule12 = new FuzzyRule(12, distancePAndAnguloPGrande, thenVdisBAndVeisM);
  fuzzy->addFuzzyRule(fuzzyRule12);
 /* ***** * * * * * * * * * * * * * *  Angulo Negativo * * * * * * * * * * * * * * * * * * * * * * * */

 /* - - - - -- -  - -- - - - - - - - - - Fuzzy Rule distancia Grande e Angulo Positivo -----------------------*/
  //      
  // Building FuzzyRule  distancia Pequena e Angulo Negativo Pequeno -> vd = ve =Baixa;
  FuzzyRuleAntecedent* distancePAndAngulonPequeno = new FuzzyRuleAntecedent();
  distancePAndAngulonPequeno->joinWithAND(distanciaPequena, nPequeno);
  
  FuzzyRuleConsequent* thenVdisZAndVeisB = new FuzzyRuleConsequent();
  thenVdisZAndVeisB->addOutput(vBaixa_d);
  thenVdisZAndVeisB->addOutput(vZero_e);

  FuzzyRule* fuzzyRule13 = new FuzzyRule(13, distancePAndAngulonPequeno, thenVdisZAndVeisB);
  fuzzy->addFuzzyRule(fuzzyRule13);
  
  // Building FuzzyRule  distancia Pequena e Angulo Negativo Medio -> vd = Media  ve =Baixa;
  FuzzyRuleAntecedent* distancePAndAngulonMedio = new FuzzyRuleAntecedent();
  distancePAndAngulonMedio->joinWithAND(distanciaPequena, nMedio);
  
  FuzzyRuleConsequent* thenVdisMAndVeisBn = new FuzzyRuleConsequent();
  thenVdisMAndVeisBn->addOutput(vMedio_d);
  thenVdisMAndVeisBn->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule14 = new FuzzyRule(14, distancePAndAngulonMedio, thenVdisMAndVeisBn);
  fuzzy->addFuzzyRule(fuzzyRule14);
  
  // Building FuzzyRule  distancia Pequena e Angulo Negativo Grande -> vd = baixa  ve =Média;
  FuzzyRuleAntecedent* distancePAndAngulonGrande = new FuzzyRuleAntecedent();
  distancePAndAngulonGrande->joinWithAND(distanciaPequena, nGrande);
  
  FuzzyRuleConsequent* thenVdisGAndVeisB = new FuzzyRuleConsequent();
  thenVdisGAndVeisB->addOutput(vGrande_d);
  thenVdisGAndVeisB->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule15 = new FuzzyRule(15, distancePAndAngulonGrande, thenVdisGAndVeisB);
  fuzzy->addFuzzyRule(fuzzyRule15);
  
  /* - - - - -- -  - -- - - - - - - - - - Fuzzy Rule distancia Media e Angulo Negativo -----------------------*/
  
  // Building FuzzyRule  distancia Media e Angulo Zero -> vd = ve = Média;
  FuzzyRuleAntecedent* distanceMAndAnguloZeron = new FuzzyRuleAntecedent();
  distanceMAndAnguloZero->joinWithAND(distanciaMedia, zero);
  
  FuzzyRuleConsequent* thenVdisMAndVeisB = new FuzzyRuleConsequent();
  thenVdisMAndVeisB->addOutput(vMedia_d);
  thenVdisMAndVeisB->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule16 = new FuzzyRule(16, distanceMAndAnguloZeron, thenVdisMAndVeisB);
  fuzzy->addFuzzyRule(fuzzyRule16);
  
  // Building FuzzyRule  distancia Média e Angulo Negativo Pequeno -> vd=Baixa ve =media;
  FuzzyRuleAntecedent* distanceMAndAnguloNPequeno = new FuzzyRuleAntecedent();
  distanceMAndAnguloNPequeno->joinWithAND(distanciaMedia, nPequeno);
  
  FuzzyRuleConsequent* thenVdisMMAndVeisB = new FuzzyRuleConsequent();
  thenVdisMMAndVeisB->addOutput(vMedia_d);
  thenVdisMMAndVeisB->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule17 = new FuzzyRule(17, distanceMAndAnguloNPequeno, thenVdisMMAndVeisB);
  fuzzy->addFuzzyRule(fuzzyRule17);
  
  // Building FuzzyRule  distancia Média e Angulo Negativo Medio -> vd = Media  ve =Baixa;
  FuzzyRuleAntecedent* distanceMAndAnguloNMedio = new FuzzyRuleAntecedent();
  distanceMAndAnguloNMedio->joinWithAND(distanciaMedia, nMedio);
  
  FuzzyRuleConsequent* thenVdisMediaAndVeisBaixa = new FuzzyRuleConsequent();
  thenVdisMediaAndVeisBaixa->addOutput(vMedia_d);
  thenVdisMediaAndVeisBaixa->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule18 = new FuzzyRule(18, distanceMAndAnguloNMedio, thenVdisMediaAndVeisBaixa);
  fuzzy->addFuzzyRule(fuzzyRule18);
  
  // Building FuzzyRule  distancia Media e Angulo Negativo Grande -> vd = Media ve =Grande;
  FuzzyRuleAntecedent* distanceMAndAngulonGrande = new FuzzyRuleAntecedent();
  distanceMAndAngulonGrande->joinWithAND(distanciaMedia, nGrande);
  
  FuzzyRuleConsequent* thenVdisGrandeAndVeisBaixa = new FuzzyRuleConsequent();
  thenVdisGrandeAndVeisBaixa->addOutput(vGrande_d);
  thenVdisGrandeAndVeisBaixa->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule19 = new FuzzyRule(19, distanceMAndAngulonGrande, thenVdisGrandeAndVeisBaixa);
  fuzzy->addFuzzyRule(fuzzyRule19);
  
  
  /* - - - - -- -  - -- - - - - - - - - - Fuzzy Rule distancia Grande e Angulo Negativo -----------------------*/
  
  // Building FuzzyRule  distancia Media e Angulo Zero -> vd = ve = Média;
  FuzzyRuleAntecedent* distanceGrandeAndAnguloZeron = new FuzzyRuleAntecedent();
  distanceGrandeAndAnguloZeron->joinWithAND(distanciaGrande, zero);
  
  FuzzyRuleConsequent* thenVdisGAndVeisG = new FuzzyRuleConsequent();
  thenVdisGAndVeisG->addOutput(vGrande_d);
  thenVdisGAndVeisG->addOutput(vGrande_e);

  FuzzyRule* fuzzyRule120 = new FuzzyRule(20, distanceGrandeAndAnguloZeron, thenVdisGAndVeisG);
  fuzzy->addFuzzyRule(fuzzyRule20);
  
  // Building FuzzyRule  distancia Grande e Angulo Negativo Pequeno -> vd=Baixa ve =media;
  FuzzyRuleAntecedent* distanceGAndAnguloNPequeno = new FuzzyRuleAntecedent();
  distanceGAndAnguloNPequeno->joinWithAND(distanciaGrande, nPequeno);
  
  FuzzyRuleConsequent* thenVdisMediaAndVeisBaixa = new FuzzyRuleConsequent();
  thenVdisMediaAndVeisBaixa->addOutput(vMedia_d);
  thenVdisMediaAndVeisBaixa->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule21 = new FuzzyRule(21, distanceGAndAnguloNPequeno, thenVdisMediaAndVeisBaixa);
  fuzzy->addFuzzyRule(fuzzyRule21);
  
  // Building FuzzyRule  distancia Grande e Angulo Negativo Medio -> vd = Media  ve =Baixa;
  FuzzyRuleAntecedent* distanceGAndAnguloNMedio = new FuzzyRuleAntecedent();
  distanceGAndAnguloNMedio->joinWithAND(distanciaGrande, nMedio);
  
  FuzzyRuleConsequent* thenVdisGrandeAndVeisBaixa = new FuzzyRuleConsequent();
  thenVdisGrandeAndVeisBaixa->addOutput(vGrande_d);
  thenVdisGrandeAndVeisBaixa->addOutput(vBaixa_e);

  FuzzyRule* fuzzyRule22 = new FuzzyRule(22, distanceGAndAnguloNMedio, thenVdisGrandeAndVeisBaixa);
  fuzzy->addFuzzyRule(fuzzyRule22);
  
  // Building FuzzyRule  distancia Grande e Angulo Negativo Grande -> vd = Media ve =Grande;
  FuzzyRuleAntecedent* distanceGrandeAndAngulonGrande = new FuzzyRuleAntecedent();
  distanceGrandeAndAngulonGrande->joinWithAND(distanciaGrande, nGrande);
  
  FuzzyRuleConsequent* thenVdisGrandeAndVeisMedia = new FuzzyRuleConsequent();
  thenVdisGrandeAndVeisMedia->addOutput(vGrande_d);
  thenVdisGrandeAndVeisMedia->addOutput(vMedia_e);

  FuzzyRule* fuzzyRule23= new FuzzyRule(23, distanceGrandeAndAngulonGrande, thenVdisGrandeAndVeisMedia);
  fuzzy->addFuzzyRule(fuzzyRule23);
  //}

  /*Verifica Domínio*/
  
  /*Modela em Pulso*/
  //transforma 0-70 em 0 a 255
  
}

  
void loop(){
    
while(mySerial.available()){          // enquanto serial estiver funcionando
    
    Serial.println(mySerial.read());  // mostrar o que foi recebido no monitor serial
    if (mySerial.read() == '\n')
    {
      for (i = 0; i < 12; i++)            // lê recebidos até uma mensagem estar completa
        {
          buffer1[i] = mySerial.read();
        }
      
      if (buffer1[0] == 'C')              // se for o nosso marcador ele salva as informações nas variáveis _n   
      {
        nos = buffer1;
        for (j = 0; j < 12; j++)          
        {
          if (j == 0)                    // salva nosso id    
          {
            aid_n = nos[j];
          }
          if (j >= 1 || j <= 4)          // salva nosso x
          {
            ax_n[j] = nos[j];
          }              
           if (j >= 5 || j <= 8)        // salva nosso y
           {
             ay_n[j] = nos[j];
           }              
           if (j >= 9 || j <= 11)       // salva nosso ângulo
           {
             aphi_n[j] = nos[j];
           }
          }
          id_n = string.toInt(aid_n);
          x_n = string.toInt(ax_n);
          y_n = string.toInt(ay_n);
          phi_n = string.toInt(aphi_n);
      }                              
      
      if (buffer1[0] != 'C')    // se for outro marcador ele salva as informações nas variáveis
      {
        marcador = buffer1;
        for (j = 0; j < 12; j++)
        {
          if (j == 0)                  // salva id de outro  
          {
            aid = marcador[j];
          }
    
          if (j >= 1 || j <= 4)       // salva outro x
          {
            ax[j] = marcador[j];
          }
    
           if (j >= 5 || j <= 8)      // salva y de outro
           {
             ay[j] = marcador[j];
           }
    
           if (j >= 9 || j <= 11)      // salva ângulo de outro
           {
             aphi[j] = marcador[j];
           }
        }
        id = string.toInt(aid);
        x = string.toInt(ax);
        y = string.toInt(ay);
        phi = string.toInt(aphi);
        
      }
    }
    
    
  // }//fecha o while do bluetooh
   double distancia = CalculaDistancia(x_n,y_n,x,y);
  
  fuzzy->setInput(1, 180);
  fuzzy->setInput(2, 40);

  fuzzy->fuzzify();
  
  Serial.print("Distancia: ");
  Serial.print(dmuitoPequena->getPertinence());
  Serial.print(", ");
  Serial.print(distanciaMedia->getPertinence());
  Serial.print(", ");
  Serial.println(distanciaGrande->getPertinence());
  
  Serial.print("Angulo: ");
  Serial.print(zero->getPertinence());
  Serial.print(", ");
  Serial.print(pPequeno->getPertinence());
  Serial.print(", ");
  Serial.print(pMedio->getPertinence());
  Serial.print(", ");
  Serial.println(pGrande->getPertinence());
  
 
  float v1 = fuzzy->defuzzify(1);
  float v2 = fuzzy->defuzzify(2);
  
  Serial.print("Velocidade Roda Direita: ");
  Serial.print(v1);
  Serial.print(", Velocidade Roda Esquerda: ");
  Serial.println(v2);
   //Modulando em pulso
  velocidadeD = (v1/70)*255;
  velocidadeE = (v2/70)*255;
  
  analogWrite(3,velocidadeD);
  analogWrite(4,velocidadeE);
  
  delay(100000);
    }//fecha laço
  }
double CalculaDistancia(double x1, double y1, double x2, double y2)
{  

    double diffx = x1 - x2;
    double diffy = y1 - y2;
    double diffx_sqr = square (diffx);
    double diffy_sqr = square (diffy);
    double distance = sqrt (diffx_sqr + diffy_sqr);
    return distance;
}
