#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int digitalArray[] = {6, 7, 8, 9, 10, 13};

double A = 561.1;
double B = 999.9;
double C = 2186;
double D = 4697;
double E = 10006;
double F = 21990;
double R[] = {A, B, C, D, E, F};
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
int pos2;
int pos3;
int pos4;
int pos5;
int p;
int x;
int y;
int z;
int pos;

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
double V[63];
double I[63];
double resistorArray[63];
double R2[63];
double R3[63];
double analogArray[63];
double searchArray[63];
double currentArray[63];


void setup()
{
  Serial.begin(9600);
  pinMode(vin,INPUT);
  pinMode(b1,INPUT);
  pinMode(b2,INPUT);
  lcd.begin(16,2);
  
  for (count = 0; count < 6; count++) {
    pinMode(digitalArray[count],OUTPUT);
    digitalWrite(digitalArray[count],LOW);
  }
    
  for (count = 0; count < 6; count++) {
    resistorArray[count]=R[count];
    for (count2 = 1+count; count2 < 6; count2++) {
      resistorArray[count*5-count*(count-1)/2+5+count2-count] = ((R[count]*R[count2])/(R[count]+R[count2]));
      for (count3 = 1+count2; count3 < 6; count3++) {
        pos3 = count*5-count*(count-1)/2+5+count2-count;
        resistorArray[(count*5-count*(count-1)/2+5+count2-count)*3+1-(count2-1)*(count2-2)*count2/6+(count2-1)*(count2-2)*(count2-3)/6-count*6+count*(count-1)*(count-2)/6+count3] = (resistorArray[pos3]*R[count3])/(resistorArray[pos3]+R[count3]);
        for (count4 = 1+count3; count4 < 6; count4++) {
          pos4 = (count*5-count*(count-1)/2+5+count2-count)*3+1-(count2-1)*(count2-2)*count2/6+(count2-1)*(count2-2)*(count2-3)/6-count*6+count*(count-1)*(count-2)/6+count3;
          resistorArray[38+count*4-(count-1)*count*3/2+(count2-1)*3-(count2-1)*(count2-2)+(count3-2)*2-(count3-2)*(count3-3)/2+count4] = (resistorArray[pos4]*R[count4])/(resistorArray[pos4]+R[count4]);
          for (count5 = 1+count4; count5 < 6; count5++) {
            pos5 = 38+count*4-(count-1)*count*3/2+(count2-1)*3-(count2-1)*(count2-2)+(count3-2)*2-(count3-2)*(count3-3)/2+count4;
            resistorArray[count+(count2-1)+(count3-2)+(count4-3)+count5+52] = (resistorArray[pos5]*R[count5])/(resistorArray[pos5]+R[count5]);
            if (count5 == 4) {
              resistorArray[62] = (resistorArray[56]*R[5])/(resistorArray[56]+R[5]);
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
    
    for (count = 0; count < 6; count++){
      digitalWrite(digitalArray[count],HIGH);              
      delay(timer);
      analogArray[count]=analogRead(count);//a, b, c, d, e, f
      for (count2 = count+1; count2 < 6; count2++){
        digitalWrite(digitalArray[count2],HIGH);
        delay(timer);
        analogArray[count*5-count*(count-1)/2+5+count2-count]=analogRead(vin);//ab, ac, ad, ae, af, bc, bd, be, bf, cd, ce, cf, de, df, ef
        for (count3 = count2+1; count3 < 6; count3++){
          digitalWrite(digitalArray[count3],HIGH);
          delay(timer);                                   
          analogArray[(count*5-count*(count-1)/2+5+count2-count)*3+1-(count2-1)*(count2-2)*count2/6+(count2-1)*(count2-2)*(count2-3)/6-count*6+count*(count-1)*(count-2)/6+count3]=analogRead(vin);//abc, abd, abe, abf, acd, ace, acf, ade, adf, aef, bcd, bce, bcf, bde, bdf, bef, cde, cdf, cef, def
          for (count4 = count3+1; count4 < 6; count4++){
            digitalWrite(digitalArray[count4],HIGH);
            delay(timer);
            analogArray[38+count*4-(count-1)*count*3/2+(count2-1)*3-(count2-1)*(count2-2)+(count3-2)*2-(count3-2)*(count3-3)/2+count4]=analogRead(vin);//abcd, abce, abcf, abde, abdf, abef, acde, acdf, acef, adef, bcde, bcdf, bcef, bdef, cdef
            for (count5 = count4+1; count5 < 6; count5++){
              digitalWrite(digitalArray[count5],HIGH);
              delay(timer);
              analogArray[count+(count2-1)+(count3-2)+(count4-3)+count5+52]=analogRead(vin);//abcde, abcdf, abcef, abdef, acdef, bcdef
              if (count5 == 4){
                digitalWrite(digitalArray[5],HIGH);
                delay(timer);
                analogArray[62]=analogRead(vin);//abcdef
                digitalWrite(digitalArray[count5],LOW);
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
    
    for (x = 0; x< 63; x++){
      searchArray[x] = analogArray[x] - 460;
      searchArray[x] = abs(searchArray[x]);
    }
    refMin = searchArray[0];
    for(y=0; y<63; y++){
      refMin=min(refMin, searchArray[y]);
    }
    
    for (z = 0; z< 63; z++){
      if (refMin == searchArray[z]) {
        pos = z;
      break;}
    }
    refResistor = resistorArray[pos];
    vx = analogArray[pos]*0.0048875855;
    i = (5-vx-0.7)/refResistor;
    rx2[p] = (vx/i);
  }
  
  for (z = 10; z< 20; z++){
      SumR = rx2[z] + SumR;
    }
    
  baseline = SumR/10; 
}

void loop()
{
  for (count = 0; count < 6; count++){
    digitalWrite(digitalArray[count],HIGH);              
    delay(timer);
    analogArray[count]=analogRead(count);//a, b, c, d, e, f
    for (count2 = count+1; count2 < 6; count2++){
      digitalWrite(digitalArray[count2],HIGH);
      delay(timer);
      analogArray[count*5-count*(count-1)/2+5+count2-count]=analogRead(vin);//ab, ac, ad, ae, af, bc, bd, be, bf, cd, ce, cf, de, df, ef
      for (count3 = count2+1; count3 < 6; count3++){
        digitalWrite(digitalArray[count3],HIGH);
        delay(timer);                                   
        analogArray[(count*5-count*(count-1)/2+5+count2-count)*3+1-(count2-1)*(count2-2)*count2/6+(count2-1)*(count2-2)*(count2-3)/6-count*6+count*(count-1)*(count-2)/6+count3]=analogRead(vin);//abc, abd, abe, abf, acd, ace, acf, ade, adf, aef, bcd, bce, bcf, bde, bdf, bef, cde, cdf, cef, def
        for (count4 = count3+1; count4 < 6; count4++){
          digitalWrite(digitalArray[count4],HIGH);
          delay(timer);
          analogArray[38+count*4-(count-1)*count*3/2+(count2-1)*3-(count2-1)*(count2-2)+(count3-2)*2-(count3-2)*(count3-3)/2+count4]=analogRead(vin);//abcd, abce, abcf, abde, abdf, abef, acde, acdf, acef, adef, bcde, bcdf, bcef, bdef, cdef
          for (count5 = count4+1; count5 < 6; count5++){
            digitalWrite(digitalArray[count5],HIGH);
            delay(timer);
            analogArray[count+(count2-1)+(count3-2)+(count4-3)+count5+52]=analogRead(vin);//abcde, abcdf, abcef, abdef, acdef, bcdef
            if (count5 == 4){
              digitalWrite(digitalArray[5],HIGH);
              delay(timer);
              analogArray[62]=analogRead(vin);//abcdef
              digitalWrite(digitalArray[count5],LOW);
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
  
  for (x = 0; x< 63; x++){
    searchArray[x] = analogArray[x] - 460;
    searchArray[x] = abs(searchArray[x]);
  }
  
  refMin = searchArray[0];
  for(y=0; y<63; y++){
    refMin=min(refMin, searchArray[y]);
  }
  
  for (z = 0; z< 63; z++){
    if (refMin == searchArray[z]) {
      pos = z;
      break;}
  }

  refResistor = resistorArray[pos];
  vx = analogArray[pos]*0.0048875855;
  i = (5-vx-0.7)/refResistor;
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

  lcd.setCursor(0,1);
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
  
  /*
  lcd.setCursor(0,1);
  lcd.print(baseline);
  */
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


