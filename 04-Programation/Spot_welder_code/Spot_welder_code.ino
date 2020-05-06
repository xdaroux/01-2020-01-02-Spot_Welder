#define TRANSFO_RELAY 10
#define FAN_RELAY 9
#define PEDAL_SW 2


#define SOLDER_TIME 50

#define TIME_FAN_ON 60000
volatile unsigned long fan_TimerOlder = millis()+TIME_FAN_ON;

//Mes relais son configurer a l'inverse so 
#define RELAY_ON 0
#define RELAY_OFF 1

void setup() {
  pinMode(PEDAL_SW, INPUT);
  pinMode(FAN_RELAY,OUTPUT);
  pinMode(TRANSFO_RELAY,OUTPUT);

  digitalWrite(TRANSFO_RELAY,RELAY_OFF); // Etre sur que le transo est a OFF par default 

}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(PEDAL_SW))
  {
    fan_TimerOlder = millis();
    digitalWrite(TRANSFO_RELAY,RELAY_ON);
    delay(SOLDER_TIME);
    digitalWrite(TRANSFO_RELAY,RELAY_OFF);
    delay(500); // delay tempon
  }

  if(millis()-fan_TimerOlder < TIME_FAN_ON)
  {
    digitalWrite(FAN_RELAY,RELAY_ON);
  }
  else
  {
    digitalWrite(FAN_RELAY,RELAY_OFF);
  }
  digitalWrite(FAN_RELAY,RELAY_ON); 
}

/*void spot_welder_ON()
{

  
  
}*/
