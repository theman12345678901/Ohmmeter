#include <LiquidCrystal.h>

LiquidCrystal lcd(41, 39, 37, 35, 33, 31);
int digitalArray[] = {6, 7, 8, 9, 10, 11, 12};

double A = 4675;
double B = 9998;
double C = 38900;
double D = 67010;
double E = 146600;
double F = 264000;
double G = 448800;
double R[] = {A, B, C, D, E, F, G};
int timer = 5;
int vin=5;
int t;

int b1;
int b2;
int count;
int count2;
int count3;
int count4;
int count5;
int count6;
int pos;
int posMin;
int p;
int x;
int y;
int z;

double refResistor;
double refMin;
double SumR = 0;
double vx;
double rx;
double rx2[20];
double i;
double i2;
double baseline;
double gap;
double V[127];
double I[127];
double resistorArray[127];
double R2[127];
double R3[127];
double analogArray[127];
double searchArray[127];
double currentArray[127];


void setup()
{
  Serial.begin(9600);
  pinMode(vin,INPUT);
  pinMode(b1,INPUT);
  pinMode(b2,INPUT);
  lcd.begin(16,2);
  
  for (count = 0; count < 7; count++) {
    pinMode(digitalArray[count],OUTPUT);
    digitalWrite(digitalArray[count],LOW);
  }
  
  pos = 0;
  for (count = 0; count < 7; count++) {
    resistorArray[pos]=R[count];
    pos = pos+1;
    for (count2 = 1+count; count2 < 7; count2++) {
      resistorArray[pos] = ((R[count]*R[count2])/(R[count]+R[count2]));
      pos = pos+1;
      for (count3 = 1+count2; count3 < 7; count3++) {
        resistorArray[pos] = ((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]));
        pos = pos+1;
        for (count4 = 1+count3; count4 < 7; count4++) {
          resistorArray[pos] = ((((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))*R[count4])/(((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))+R[count4]));
          pos = pos+1;
          for (count5 = 1+count4; count5 < 7; count5++) {
            resistorArray[pos] = ((((((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))*R[count4])/(((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))+R[count4]))*R[count5])/(((((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))*R[count4])/(((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))+R[count4]))+R[count5]));
            pos = pos+1;
            for (count6 = 1+count5; count6 < 7; count6++) {
              resistorArray[pos] = ((((((((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))*R[count4])/(((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))+R[count4]))*R[count5])/(((((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))*R[count4])/(((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))+R[count4]))+R[count5]))*R[count6])/(((((((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))*R[count4])/(((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))+R[count4]))*R[count5])/(((((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))*R[count4])/(((((R[count]*R[count2])/(R[count]+R[count2]))*R[count3])/(((R[count]*R[count2])/(R[count]+R[count2]))+R[count3]))+R[count4]))+R[count5]))+R[count6]));
              pos = pos+1;
            }
          }
        }
      }
    }
  }
  
  b1 = analogRead(6);
  b2 = analogRead(7);
  while (b1!=1023){
    lcd.setCursor(0,0);
    lcd.print("Electrodes on?  ");
    lcd.setCursor(0,1);
    lcd.print("Yes           No");
    if(b2==1023){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please place the");
      lcd.setCursor(0,1);
      lcd.print("electrodes");
      delay(3000);}
    b1 = analogRead(6);
    b2 = analogRead(7);
  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Initializing...");
  
  delay(1000);
  
  for (p=0; p<20; p++){
    t=20-p;
    if(t==9){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Initializing...");
    }
    lcd.setCursor(0,1);
    lcd.print(t);
    
    pos = 0;
    for (count = 0; count < 7; count++){
      digitalWrite(digitalArray[count],HIGH);              
      delay(timer);
      analogArray[pos]=analogRead(count);
      pos = pos+1;
      for (count2 = count+1; count2 < 7; count2++){
        digitalWrite(digitalArray[count2],HIGH);
        delay(timer);
        analogArray[pos]=analogRead(vin);
        pos = pos+1;
        for (count3 = count2+1; count3 < 7; count3++){
          digitalWrite(digitalArray[count3],HIGH);
          delay(timer);                                   
          analogArray[pos]=analogRead(vin);
          pos = pos+1;
          for (count4 = count3+1; count4 < 7; count4++){
            digitalWrite(digitalArray[count4],HIGH);
            delay(timer);
            analogArray[pos]=analogRead(vin);
            pos = pos+1;
            for (count5 = count4+1; count5 < 7; count5++){
              digitalWrite(digitalArray[count5],HIGH);
              delay(timer);
              analogArray[pos]=analogRead(vin);
              pos = pos+1;
              for (count6 = count5+1; count6 < 7; count6++){
                digitalWrite(digitalArray[count6],HIGH);
                delay(timer);
                analogArray[pos]=analogRead(vin);
                pos = pos+1;
                digitalWrite(digitalArray[count6],LOW);
              }
              digitalWrite(digitalArray[count5],LOW);
            }
            digitalWrite(digitalArray[count4],LOW);
          }
          digitalWrite(digitalArray[count3],LOW);
        }
        digitalWrite(digitalArray[count2],LOW);
      }
      digitalWrite(digitalArray[count],LOW);
    }
    
    for (x = 0; x<127; x++){
      searchArray[x] = analogArray[x] - 450;
      searchArray[x] = abs(searchArray[x]);
    }
    refMin = searchArray[0];
    for(y=0; y<127; y++){
      refMin=min(refMin, searchArray[y]);
    }
    
    for (z = 0; z<127; z++){
      if (refMin == searchArray[z]) {
        pos = z;
      break;}
    }
    
    if (analogArray[pos]< 210){
      pos = 6;
    }
    else if (analogArray[pos]>500){
      pos = 125;
    }
  
    refResistor = resistorArray[pos];
    vx = analogArray[pos]*0.0048875855;
    if (refResistor < 3000){
      i = (5-vx-0.65)/refResistor;
    }
    else if (refResistor < 5000){
      i = (5-vx-0.575)/refResistor;
    }
    else if (refResistor < 50000){
      i = (5-vx-0.55)/refResistor;
    }
    else if (refResistor < 150000){
      i = (5-vx-0.5)/refResistor;
    }
    else{
      i = (5-vx-0.475)/refResistor;
    }
    rx2[p] = (vx/i);
  }
  
  for (z = 10; z< 20; z++){
      SumR = rx2[z] + SumR;
    }
    
  baseline = SumR/10; 
}

void loop()
{
  pos = 0;
  for (count = 0; count < 7; count++){
    digitalWrite(digitalArray[count],HIGH);              
    delay(timer);
    analogArray[pos]=analogRead(count);
    pos = pos+1;
    for (count2 = count+1; count2 < 7; count2++){
      digitalWrite(digitalArray[count2],HIGH);
      delay(timer);
      analogArray[pos]=analogRead(vin);
      pos = pos+1;
      for (count3 = count2+1; count3 < 7; count3++){
        digitalWrite(digitalArray[count3],HIGH);
        delay(timer);                                   
        analogArray[pos]=analogRead(vin);
        pos = pos+1;
        for (count4 = count3+1; count4 < 7; count4++){
          digitalWrite(digitalArray[count4],HIGH);
          delay(timer);
          analogArray[pos]=analogRead(vin);
          pos = pos+1;
          for (count5 = count4+1; count5 < 7; count5++){
            digitalWrite(digitalArray[count5],HIGH);
            delay(timer);
            analogArray[pos]=analogRead(vin);
            pos = pos+1;
            for (count6 = count5+1; count6 < 7; count6++){
              digitalWrite(digitalArray[count6],HIGH);
              delay(timer);
              analogArray[pos]=analogRead(vin);
              pos = pos+1;
              digitalWrite(digitalArray[count6],LOW);
            }
            digitalWrite(digitalArray[count5],LOW);
          }
          digitalWrite(digitalArray[count4],LOW);
        }
        digitalWrite(digitalArray[count3],LOW);
      }
      digitalWrite(digitalArray[count2],LOW);
    }
    digitalWrite(digitalArray[count],LOW);
  }
  
  for (x = 0; x< 127; x++){
    searchArray[x] = analogArray[x] - 450;
    searchArray[x] = abs(searchArray[x]);
  }
  
  refMin = searchArray[0];
  for(y=0; y<127; y++){
    refMin=min(refMin, searchArray[y]);
  }
  
  for (z = 0; z< 127; z++){
    if (refMin == searchArray[z]) {
      posMin = z;
      break;}
  }
  
  if (analogArray[posMin]< 210){
    posMin = 6;
  }
  else if (analogArray[posMin]>500){
    posMin = 125;
  }
  
  refResistor = resistorArray[posMin];
  vx = analogArray[posMin]*0.0048875855;
  
  if (refResistor < 3000){
    i = (5-vx-0.65)/refResistor;
  }
  else if (refResistor < 5000){
    i = (5-vx-0.575)/refResistor;
  }
  else if (refResistor < 50000){
    i = (5-vx-0.55)/refResistor;
  }
  else if (refResistor < 150000){
    i = (5-vx-0.5)/refResistor;
  }
  else{
    i = (5-vx-0.475)/refResistor;
  }
  
  rx = (vx/i);
  gap = (rx-baseline)*0.01;
  
  if(rx<0||vx>4.2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Error");
    Serial.println("Error");
  }
  else if(rx<1000){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(rx, 5);
    lcd.setCursor(8,0);
    lcd.print((char)244);
    }
    else if (rx < 1000000){
      lcd.clear();
      rx=rx/1000;
      lcd.setCursor(0,0);
      lcd.print(rx, 5);
      lcd.setCursor(8,0);
      lcd.print("k");
      lcd.print((char)244);
    }
    else{
      lcd.clear();
      rx=rx/1000000;
      lcd.setCursor(0,0);
      lcd.print(rx, 5);
      lcd.setCursor(8,0);
      lcd.print("M");
      lcd.print((char)244);
    }

  /*lcd.setCursor(0,1);
  lcd.print("Status:");
  if (gap<1){
    lcd.setCursor(8,1);
    lcd.print("Good");
  }
  else if (gap<2){
    lcd.setCursor(8,1);
    lcd.print("OK  ");
  }
  else if (gap<3){
    lcd.setCursor(8,1);
    lcd.print("Poor");
  }
  else if (gap>=3){
    lcd.setCursor(8,1);
    lcd.print("Bad ");
  }
  
  Serial.print("A5 = ");
  Serial.println(vx);
  Serial.print("rx = ");
  Serial.println(rx);
  Serial.print("Baseline = ");
  Serial.println(baseline);
  Serial.print("Predicted Gap = ");
  Serial.println(gap);
  */
  
  lcd.setCursor(0,1);
  lcd.print(refResistor);
  
  lcd.setCursor(9,1);
  lcd.print(analogArray[posMin]);
  
  /*
  Serial.print("R0 = ");
  Serial.println(R2[0]);
  Serial.print("R1 = ");
  Serial.println(R2[1]);
  Serial.print("R2 = ");
  Serial.println(R2[2]);
  Serial.print("R3 = ");
  Serial.println(R2[3]);
  Serial.print("R4 = ");
  Serial.println(R2[4]);
  Serial.print("R5 = ");
  Serial.println(R2[5]);
  Serial.print("R6 = ");
  Serial.println(R2[6]);
  Serial.print("R7 = ");
  Serial.println(R2[7]);
  Serial.print("R8 = ");
  Serial.println(R2[8]);
  Serial.print("R9 = ");
  Serial.println(R2[9]);
  Serial.print("R10 = ");
  Serial.println(R2[10]);
  
  Serial.print("R0 = ");
  Serial.println(R3[0]);
  Serial.print("R1 = ");
  Serial.println(R3[1]);
  Serial.print("R2 = ");
  Serial.println(R3[2]);
  Serial.print("R3 = ");
  Serial.println(R3[3]);
  Serial.print("R4 = ");
  Serial.println(R3[4]);
  Serial.print("R5 = ");
  Serial.println(R3[5]);
  Serial.print("R6 = ");
  Serial.println(R3[6]);
  Serial.print("R7 = ");
  Serial.println(R3[7]);
  Serial.print("R8 = ");
  Serial.println(R3[8]);
  Serial.print("R9 = ");
  Serial.println(R3[9]);
  Serial.print("R10 = ");
  Serial.println(R3[10]);
  
  Serial.print("R0 = ");
  Serial.println(R2[0]);
  Serial.print("R1 = ");
  Serial.println(R2[1]);
  Serial.print("R2 = ");
  Serial.println(R2[2]);
  Serial.print("R3 = ");
  Serial.println(R2[3]);
  Serial.print("R4 = ");
  Serial.println(R2[4]);
  Serial.print("R5 = ");
  Serial.println(R2[5]);
  Serial.print("R6 = ");
  Serial.println(R2[6]);
  Serial.print("R7 = ");
  Serial.println(R2[7]);
  Serial.print("R8 = ");
  Serial.println(R2[8]);
  Serial.print("R9 = ");
  Serial.println(R2[9]);
  Serial.print("R10 = ");
  Serial.println(R2[10]);
  */
  /*
  Serial.print("currentArray0 = ");
  Serial.println(currentArray[0]);
  Serial.print("currentArray1 = ");
  Serial.println(currentArray[1]);
  Serial.print("currentArray2 = ");
  Serial.println(currentArray[2]);
  Serial.print("currentArray3 = ");
  Serial.println(currentArray[3]);
  Serial.print("currentArray4 = ");
  Serial.println(currentArray[4]);
  Serial.print("currentArray5 = ");
  Serial.println(currentArray[5]);
  Serial.print("currentArray6 = ");
  Serial.println(currentArray[6]);
  Serial.print("currentArray7 = ");
  Serial.println(currentArray[7]);
  Serial.print("currentArray8 = ");
  Serial.println(currentArray[8]);
  Serial.print("currentArray9 = ");
  Serial.println(currentArray[9]);
  Serial.print("currentArray62 = ");
  Serial.println(currentArray[62]);
  Serial.print("currentArray11 = ");
  Serial.println(currentArray[11]);
  Serial.print("currentArray12 = ");
  Serial.println(currentArray[12]);
  Serial.print("currentArray13 = ");
  Serial.println(currentArray[13]);
  Serial.print("currentArray14 = ");
  Serial.println(currentArray[14]);
  Serial.print("currentArray15 = ");
  Serial.println(currentArray[15]);
  Serial.print("currentArray16 = ");
  Serial.println(currentArray[16]);
  Serial.print("currentArray17 = ");
  Serial.println(currentArray[17]);
  Serial.print("currentArray18 = ");
  Serial.println(currentArray[18]);
  Serial.print("currentArray19 = ");
  Serial.println(currentArray[19]);
  Serial.print("currentArray20 = ");
  Serial.println(currentArray[20]);
  Serial.print("currentArray21 = ");
  Serial.println(currentArray[21]);
  Serial.print("currentArray22 = ");
  Serial.println(currentArray[22]);
  Serial.print("currentArray23 = ");
  Serial.println(currentArray[23]);
  Serial.print("currentArray24 = ");
  Serial.println(currentArray[24]);
  Serial.print("currentArray25 = ");
  Serial.println(currentArray[25]);
  Serial.print("currentArray26 = ");
  Serial.println(currentArray[26]);
  Serial.print("currentArray27 = ");
  Serial.println(currentArray[27]);
  Serial.print("currentArray28 = ");
  Serial.println(currentArray[28]);
  Serial.print("currentArray29 = ");
  Serial.println(currentArray[29]);
  Serial.print("currentArray30 = ");
  Serial.println(currentArray[30]);
  Serial.print("currentArray31 = ");
  Serial.println(currentArray[31]);
  Serial.print("currentArray32 = ");
  Serial.println(currentArray[32]);
  Serial.print("currentArray33 = ");
  Serial.println(currentArray[33]);
  Serial.print("currentArray34 = ");
  Serial.println(currentArray[34]);
  Serial.print("currentArray35 = ");
  Serial.println(currentArray[35]);
  Serial.print("currentArray36 = ");
  Serial.println(currentArray[36]);
  Serial.print("currentArray37 = ");
  Serial.println(currentArray[37]);
  Serial.print("currentArray38 = ");
  Serial.println(currentArray[38]);
  Serial.print("currentArray39 = ");
  Serial.println(currentArray[39]);
  Serial.print("currentArray40 = ");
  Serial.println(currentArray[40]);
  Serial.print("currentArray41 = ");
  Serial.println(currentArray[41]);
  Serial.print("currentArray42 = ");
  Serial.println(currentArray[42]);
  Serial.print("currentArray43 = ");
  Serial.println(currentArray[43]);
  Serial.print("currentArray44 = ");
  Serial.println(currentArray[44]);
  Serial.print("currentArray45 = ");
  Serial.println(currentArray[45]);
  Serial.print("currentArray46 = ");
  Serial.println(currentArray[46]);
  Serial.print("currentArray47 = ");
  Serial.println(currentArray[47]);
  Serial.print("currentArray48 = ");
  Serial.println(currentArray[48]);
  Serial.print("currentArray49 = ");
  Serial.println(currentArray[49]);
  Serial.print("currentArray50 = ");
  Serial.println(currentArray[50]);
  Serial.print("currentArray51 = ");
  Serial.println(currentArray[51]);
  Serial.print("currentArray52 = ");
  Serial.println(currentArray[52]);
  Serial.print("currentArray53 = ");
  Serial.println(currentArray[53]);
  Serial.print("currentArray54 = ");
  Serial.println(currentArray[54]);
  Serial.print("currentArray55 = ");
  Serial.println(currentArray[55]);
  Serial.print("currentArray56 = ");
  Serial.println(currentArray[56]);
  Serial.print("currentArray57 = ");
  Serial.println(currentArray[57]);
  Serial.print("currentArray58 = ");
  Serial.println(currentArray[58]);
  Serial.print("currentArray59 = ");
  Serial.println(currentArray[59]);
  Serial.print("currentArray60 = ");
  Serial.println(currentArray[60]);
  Serial.print("currentArray61 = ");
  Serial.println(currentArray[61]);
  Serial.print("currentArray62 = ");
  Serial.println(analogArray[62]);*/
  Serial.println(" ");
  
}


