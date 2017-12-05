#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>

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
  fuzzy->setInput(1, 10);
  fuzzy->setInput(2, 30);
  fuzzy->setInput(3, -15);

  fuzzy->fuzzify();
  
  Serial.print("Distancia: ");
  Serial.print(close->getPertinence());
  Serial.print(", ");
  Serial.print(safe->getPertinence());
  Serial.print(", ");
  Serial.println(distante->getPertinence());
  
  Serial.print("Velocidade: ");
  Serial.print(stoped->getPertinence());
  Serial.print(", ");
  Serial.print(slow->getPertinence());
  Serial.print(", ");
  Serial.print(normal->getPertinence());
  Serial.print(", ");
  Serial.println(quick->getPertinence());
  
  Serial.print("Temperatura: ");
  Serial.print(cold->getPertinence());
  Serial.print(", ");
  Serial.print(good->getPertinence());
  Serial.print(", ");
  Serial.println(hot->getPertinence());

  float output1 = fuzzy->defuzzify(1);
  float output2 = fuzzy->defuzzify(2);
  
  Serial.print("Saida risco: ");
  Serial.print(output1);
  Serial.print(", Saida velocidade: ");
  Serial.println(output2);

  delay(100000);
}
