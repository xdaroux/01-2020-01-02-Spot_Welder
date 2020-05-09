#include <LiquidCrystal.h>

//LCD define 
#define ROW 16
#define COLONE 2
//pin
#define RS 12
#define E 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2
/*
 * R/W GND
 * VSS GND
 * VDD 5V
 * V0  Potentiometer wiper GND to  5V
 * A   Resistor to 5V
 * K   GND 
  */


//ENCODER define 
#define OUTA 6
#define OUTB 7
#define SW_ENCODEUR 13

#define DELAI_ENCODEUR 500

#define CW 1
#define CCW -1
enum ETAT {ETAT0,ETAT1,ETAT2,ETAT3};



//SpotWelder  
#define DEBUG_SPOTWELDER 1

#define DEFAULT_TIME 50
#define SOLDER_TIME 50
#define TIME_FAN_ON 60000

//Mes relais son configurer a l'inverse
#define RELAY_ON 0
#define RELAY_OFF 1

//pin
#define TRANSFO_RELAY 10
#define FAN_RELAY 9
#define PEDAL_SW 8




//LCD Global Variable 
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);


//ENCODER GLOBAL Variable 


//SpotWelder
volatile unsigned long fan_TimerOlder = millis()+TIME_FAN_ON;
int Etat_fan = 0;

//Power Supply 
unsigned int Voltage = 0;


void setup() {
  //LCD 
  lcd.begin(ROW,COLONE); // dimension de l'ecran lcd ROW COLON
  lcd.setCursor(0,1);
  lcd.print("TIME ON : ");
  lcd.setCursor(14,1);
  lcd.print("ms");
  


  
  //ENCODER 
  pinMode (OUTA,INPUT_PULLUP);
  pinMode (OUTB,INPUT_PULLUP);
  pinMode (SW_ENCODEUR,INPUT_PULLUP);



  //SpotWelder
  pinMode(PEDAL_SW, INPUT);
  pinMode(FAN_RELAY,OUTPUT);
  pinMode(TRANSFO_RELAY,OUTPUT);
  digitalWrite(TRANSFO_RELAY,RELAY_OFF); // Etre sur que le transo est a OFF par default 
  lcd.setCursor(8,0);
  lcd.print("FAN: ");
  lcd.print("OFF");
   
  //Power Supply 
  lcd.setCursor(2,0);
  lcd.print(" Volt");
  
  Serial.begin(9600);
  

}

void loop() {
  
  int outa;
  int outb;
  int etat_encodeur = ETAT0;
  int timeRelayOn = DEFAULT_TIME;
  int tmp = 0;

  while(1)
  { 
    //**********************************************   ENCODER   *******************************//
      outa = digitalRead(OUTA);
      outb =  digitalRead(OUTB);
      tmp =  lire_encodeur(&etat_encodeur,outa,outb);
      if(timeRelayOn == 0 && tmp < 0)
      {
        // On veut pas de valeur negative 
      }
      else
      {
        timeRelayOn += tmp;
      }
      if(digitalRead(SW_ENCODEUR) == 0)
      {
        Etat_fan = !Etat_fan;
        Serial.print("SW : ");
        Serial.println(Etat_fan);
        delay(250); //tempon pour la sw
      }


      //*****************************************    LCD   **********************************//
      lcd.setCursor(10,1);
      if(timeRelayOn >= 100)
      {
        
      }
      else if(timeRelayOn >= 10)
      {
        lcd.print(" ");
      }
      else
      {
        lcd.print("  ");
      }
      lcd.print(timeRelayOn);

      
      //*****************************************    Spot Welder   **********************************//
       if(digitalRead(PEDAL_SW))
      {
        if(DEBUG_SPOTWELDER)
        {
          Serial.println("PEDAL ON");
        }
        fan_TimerOlder = millis();
        digitalWrite(TRANSFO_RELAY,RELAY_ON);
        delay(timeRelayOn);
        digitalWrite(TRANSFO_RELAY,RELAY_OFF);
        delay(500); // delay tempon
      }
    
      if(Etat_fan == 1)
      {
        digitalWrite(FAN_RELAY,RELAY_ON);
        lcd.setCursor(13,0);
        lcd.print(" ON");

      }
      else if(millis()-fan_TimerOlder < TIME_FAN_ON)
      {
        digitalWrite(FAN_RELAY,RELAY_ON);
        
        
        tmp = (TIME_FAN_ON-(millis()-fan_TimerOlder))/1000; 
        lcd.setCursor(13,0);
        if(tmp < 10)
        {
          lcd.print(" ");
        }
        else
        {
        
        }
        lcd.print(tmp);
        lcd.print("s");
      }
      else
      {
        digitalWrite(FAN_RELAY,RELAY_OFF);
        lcd.setCursor(13,0);
        lcd.print("OFF");
      }
      //************************************* Power Supply  *****************************//
      Voltage = analogRead(A0);
      Voltage = map(Voltage,0,1023,0,40);
      lcd.setCursor(0,0);
      if(Voltage < 10)
      {
        lcd.print(" ");
      }
      
      lcd.print(Voltage);
      
   }
}

int lire_encodeur(int* p_etat_encodeur,int outa, int outb)
{
  int etat;
  int etat_avant = *p_etat_encodeur;
  int val = 0;
  if(outa == 1 && outb == 1)
  {
    etat = ETAT0; /// Wait 
  }
  else if (outa == 0 && outb == 1)
  {
    etat = ETAT1; ///CW
    if (etat_avant == ETAT0)
    val++;
  }
  else if (outa == 0 && outb == 0)
  {
    etat = ETAT2; /// noting
  }
  else if (outa == 1 && outb == 0)
  {
    etat = ETAT3; ///CCW
    if (etat_avant == ETAT0)
    val--;
  }

  delay(1);
  *p_etat_encodeur = etat;

  return val;
  
}
