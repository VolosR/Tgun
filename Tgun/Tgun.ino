#include "Slider-potentiometer-easyC-SOLDERED.h"
#include "Relays-SOLDERED.h"
#include <TFT_eSPI.h>
#include "gun.h"
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

#define fire 16
#define laser 14
#define chMode 0
#define background 0xBE79
#define txtColor 0x4208
#define RELAY_ADDRESS 0x30

int ammo=32;
int mode=0; //0 is semia, 1 is three round, 2 is full auto
int lastMode=3;
bool empty=false;

int deb=0;
int deb2=0;
int deb3=0;
bool sw1=0;
bool laserON=0;

CH_Relay Relay;
sliderPot slider;
int rel=0;

void setup() {

    pinMode(15,OUTPUT);
    digitalWrite(15,1);

  pinMode(1,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(fire,INPUT_PULLUP);
  pinMode(laser,INPUT_PULLUP);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_ORANGE);
  
  sprite.createSprite(320,170);
  sprite.setSwapBytes(1);

       ledcSetup(0, 10000, 8);
       ledcAttachPin(38, 0);
       ledcWrite(0, 168);

  Wire.begin(43, 44);
  Relay.begin(RELAY_ADDRESS);
  slider.begin();
  
        draw();
}

void draw()
  {
    sprite.pushImage(0,0,320,170,gun);
    //sprite.fillRect(236,46,56,56,background);
    sprite.setTextColor(txtColor, background);
    sprite.setTextDatum(4);
    sprite.drawString(String(ammo),264,73,7);

     sprite.fillRect(88+(laserON*16),54,6,6,TFT_WHITE);
     sprite.fillRect(168,(mode*14)+40,10,8,txtColor);
   
    for(int i=0;i<ammo;i++)
    sprite.fillRect(75+(i*7),130,4,16,txtColor);

    if(ammo==0)
    sprite.drawString("RELOAD!!!",180,140,4);
    sprite.pushSprite(0,0);
  }

  void klik()
  {
    if(mode==0)
    { Relay.relayControl(0, 1);
  delay(20);
  Relay.relayControl(0, 0);
  delay(20);}

  else{  
  sw1=!sw1;
  Relay.relayControl(sw1, 1);
  delay(20);
  Relay.relayControl(sw1, 0);}
  }

  void reload()
  {  
    ammo=32;
    empty=0;
  }

void loop() {

  if(lastMode!=mode) {draw(); lastMode=mode;}

  if(digitalRead(1)==0 && digitalRead(3)==1)
  mode=0;

  if(digitalRead(1)==1 && digitalRead(3)==0)
  mode=2;

  if(digitalRead(1)==1 && digitalRead(3)==1)
  mode=1;

  if(digitalRead(laser)==0)
  {if(deb2==0){deb2=1; laserON=!laserON; draw();}
  }else deb2=0;

  

  if(mode==0 && empty==false)
  if(digitalRead(fire)==0)
  {if(deb==0){deb=1; ammo--; draw(); klik(); delay(50);}
  }else deb=0;

      if(mode==1 && empty==false)
      if(digitalRead(fire)==0){
      if(deb==0)
      { deb=1;
        for(int i=0;i<3;i++)
        {if(ammo>0){ammo--;  draw(); klik(); delay(20);}}
      }}else deb=0;

      if(mode==2 && empty==false)
      if(digitalRead(fire)==0)
      {ammo--; draw(); klik(); delay(18);}

      if(empty==true)
        {
        int value=slider.getValue();

        if(rel==0 && value<12)
        {rel=1;  Relay.relayControl(0, 1); Relay.relayControl(1, 1);}               

        if(rel==1 && value>990 )
        {rel=0; Relay.relayControl(0, 0); Relay.relayControl(1, 0); reload(); draw();}

        }
   
  
 
 
 if(ammo==0) {empty=true;  draw();}

}

