#include <LiquidCrystal.h>

double A = 560;
double B = 1000;
double C = 2200;
double D = 4700;
double E = 10000;
double F = 22000;
int pos2;
int pos3;
int pos4;
int pos5;
int vin=5;
int count = 0;
int count2 = 0;
int count3 = 0;
int count4 = 0;
int count5 = 0;
int timer = 50;
double refResistor;
double refMin;
int pos;
int x;
int y;
int z;
double SumR = 0;
double vx;
double rx;
double i;
double R[] = {A, B, C, D, E, F};
double V[63];
double I[63];
double resistorArray[63];
int digitalArray[] = {6, 7, 8, 9, 10, 13};
double analogArray[63];
double searchArray[63];
double currentArray[63];

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  Serial.begin(9600);
  pinMode(vin,INPUT);
  lcd.begin(16,2);
  
  for (count = 0; count < 6; count++) {
    pinMode(digitalArray[count],OUTPUT);
    digitalWrite(digitalArray[count],LOW);
  }
  
  
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
  
  for (x = 0; x< 63; x++)//calculates difference between analog readings and the 430 goal
  {
    V[x] = analogArray[x]*0.0048875855;
  }
  
  count = 0;
  count2 = 0;
  count3 = 0;
  count4 = 0;
  count5 = 0;
  
  for (count = 0; count < 6; count++) {
    currentArray[count]=(5-0.65-V[count])/R[count];
    for (count2 = 1+count; count2 < 6; count2++) {
      pos2 = count*5-count*(count-1)/2+5+count2-count;
      currentArray[pos2] = (5-0.65-V[pos2])/R[count2]+(5-0.65-V[pos2])/R[count];
      for (count3 = 1+count2; count3 < 6; count3++) {
        pos3 = (count*5-count*(count-1)/2+5+count2-count)*3+1-(count2-1)*(count2-2)*count2/6+(count2-1)*(count2-2)*(count2-3)/6-count*6+count*(count-1)*(count-2)/6+count3;
        currentArray[pos3] = (5-0.65-V[pos3])/R[count3]+(5-0.65-V[pos3])/R[count2]+(5-0.65-V[pos3])/R[count];
        for (count4 = 1+count3; count4 < 6; count4++) {
          pos4 = 38+count*4-(count-1)*count*3/2+(count2-1)*3-(count2-1)*(count2-2)+(count3-2)*2-(count3-2)*(count3-3)/2+count4;
          currentArray[pos4] = (5-0.65-V[pos4])/R[count4]+(5-0.65-V[pos4])/R[count3]+(5-0.65-V[pos4])/R[count2]+(5-0.65-V[pos4])/R[count];
          for (count5 = 1+count4; count5 < 6; count5++) {
            pos5 = count+(count2-1)+(count3-2)+(count4-3)+count5+52;
            currentArray[pos5] = (5-0.65-V[pos5])/R[count5]+(5-0.65-V[pos5])/R[count4]+(5-0.65-V[pos5])/R[count3]+(5-0.65-V[pos5])/R[count2]+(5-0.65-V[pos5])/R[count];
            if (count5 == 4) {
              currentArray[62] = (5-0.65-V[62])/R[62]+(5-0.65-V[62])/R[count5]+(5-0.65-V[62])/R[count4]+(5-0.65-V[62])/R[count3]+(5-0.65-V[62])/R[count2]+(5-0.65-V[62])/R[count];
            }
          }
        }
      }
    }
  }
  
  SumR=0;
  for (z = 0; z< 63; z++)
  {
    resistorArray[z] = V[z]/currentArray[z];
    SumR = resistorArray[z] + SumR;
  }
  
  rx = SumR/63;
  
  for (y = 0; y< 63; y++)
  {
    currentArray[y] = 10000*currentArray[y];
  }
  
  if(vx>4.8)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("----INFINITY----");
    Serial.println("----INFINITY----");
  }
  else
  {
    if(rx<1000)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(rx);
      lcd.setCursor(7,0);
      lcd.print((char)244);
      Serial.print(rx);
      Serial.println(" ohm");
    }
    else if (rx < 1000000)
    {
      lcd.clear();
      rx=rx/1000;
      lcd.setCursor(0,0);
      lcd.print(rx);
      lcd.setCursor(6,0);
      lcd.print("k");
      lcd.print((char)244);
      Serial.print(rx);
      Serial.println(" k ohm");
    }
    else
    {
      lcd.clear();
      rx=rx/1000000;
      lcd.setCursor(0,0);
      lcd.print(rx);
      lcd.setCursor(6,0);
      lcd.print("M");
      lcd.print((char)244);
      Serial.print(rx);
      Serial.println(" M ohm");
    }
  }
  lcd.setCursor(0,1);
  lcd.print("T.R.E.S.");
  
  Serial.print("V0 = ");
  Serial.println(V[0]);
  Serial.print("V1 = ");
  Serial.println(V[1]);
  Serial.print("V2 = ");
  Serial.println(V[2]);
  Serial.print("V3 = ");
  Serial.println(V[3]);
  Serial.print("V4 = ");
  Serial.println(V[4]);
  Serial.print("V5 = ");
  Serial.println(V[5]);
  Serial.print("V6 = ");
  Serial.println(V[6]);
  Serial.print("V7 = ");
  Serial.println(V[7]);
  Serial.print("V8 = ");
  Serial.println(V[8]);
  Serial.print("V9 = ");
  Serial.println(V[9]);
  Serial.print("V10 = ");
  Serial.println(V[10]);
  
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
  Serial.println(currentArray[62]);/*
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


