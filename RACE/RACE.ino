#include <Adafruit_NeoPixel.h>

//#define DEBUG
#define NUM_LEDS 300
#define DATA_PIN 2
#define BUZZER_PIN 3
#define COLOR_ORDER RGB
#define LED_TYPE    WS2812B
#define PLAYER_1 13
#define PLAYER_2 12
#define PLAYER_3 11
#define PLAYER_4 10

#define COLOR1    track.Color(255,0,0)
#define COLOR2    track.Color(0,255,0)
#define COLOR3    track.Color(0,0,255)

int win_music[] = {
  2637, 2637, 0, 2637, 
  0, 2093, 2637, 0,
  3136    
};
    
int TBEEP=4; 

float speed1=0;
float speed2=0;
float speed3=0;
float speed4=0;
float dist1=0;
float dist2=0;
float dist3=0;
float dist4=0;

byte loop1=0;
byte loop2=0;
byte loop3=0;
byte loop4=0;

byte leader=0;
byte loop_max=5;

int NPIXELS=300;
float ACEL=0.2;
float FRICTION=0.01;
float kf=1.5;

byte flag_sw1=0;
byte flag_sw2=0;
byte flag_sw3=0;
byte flag_sw4=0;
byte draworder=0;
 
unsigned long timestamp=0;

Adafruit_NeoPixel track = Adafruit_NeoPixel(NPIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

int tdelay = 5; 

void setup() {
  track.begin(); 
  pinMode(PLAYER_1 , INPUT_PULLUP);
  pinMode(PLAYER_2 , INPUT_PULLUP);
  pinMode(PLAYER_3 , INPUT_PULLUP);
  pinMode(PLAYER_4 , INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT); 
  start_race();
}

void start_race() {

      //Restart variables
      loop1=0;
      loop2=0;
      loop3=0;
      loop4=0;
      dist1=0;
      dist2=0;
      dist3=0;
      dist4=0;
      speed1=0;
      speed2=0;
      speed3=0;
      speed4=0;
      timestamp=0;
      ///////////////////
      
      for(int i=0;i<NPIXELS;i++){
          track.setPixelColor(i, track.Color(0,0,0));
          };
      track.show();     
      delay(2000);
      track.setPixelColor(9, track.Color(255,0,0));
      track.setPixelColor(8, track.Color(255,0,0));
      track.setPixelColor(7, track.Color(255,0,0));
      track.show();
      tone(BUZZER_PIN, 440);
      delay(500);
      noTone(BUZZER_PIN);
      delay(250);
                     
      track.setPixelColor(9, track.Color(0,0,0));
      track.setPixelColor(8, track.Color(255,0,0));
      track.setPixelColor(7, track.Color(255,0,0));
      track.show();
      tone(BUZZER_PIN, 440);
      delay(500);
      noTone(BUZZER_PIN);
      delay(250);    
                 
      track.setPixelColor(9, track.Color(0,0,0));
      track.setPixelColor(8, track.Color(0,0,0));
      track.setPixelColor(7, track.Color(255,0,0));
      track.show();
      tone(BUZZER_PIN, 440);
      delay(500);
      noTone(BUZZER_PIN);
      delay(250);
      
      track.setPixelColor(9, track.Color(0,255,0));
      track.setPixelColor(8, track.Color(0,255,0));
      track.setPixelColor(7, track.Color(0,255,0));
      track.show();
      tone(BUZZER_PIN, 600);
      delay(1000);
      noTone(BUZZER_PIN);
      
      track.setPixelColor(9, track.Color(0,0,0));
      track.setPixelColor(8, track.Color(0,0,0));
      track.setPixelColor(7, track.Color(0,0,0));
      track.show();                              
      timestamp=0;              
};

void winner_fx() {
  
     int msize = sizeof(win_music) / sizeof(int);
     for (int note = 0; note < msize; note++) {
     tone(BUZZER_PIN, win_music[note],200);
     delay(230);
     noTone(BUZZER_PIN);
}
};

void looper(){
//to do
 }

void checkpoint1(){
//to do
 }
 
void jump1(){
//to do
}

void checkpoint2(){
//to do
 }

void russ_mount(){
//to do
 }
 
void checkpoint3(){
//to do
 }

void track_oil(){
//to do
 }
 
void checkpoint4(){
//to do
 }

void tube(){
//to do
}


void draw_car1(void){
  for(int i=0;i<=loop1;i++){
    track.setPixelColor(((word)dist1 % NPIXELS)+i, track.Color(0,255-i*50,0));
    };                 
  }

void draw_car2(void){
  for(int i=0;i<=loop2;i++){
    track.setPixelColor(((word)dist2 % NPIXELS)+i, track.Color(255-i*50,0,0));
    };            
 }

void draw_car3(void){
  for(int i=0;i<=loop3;i++){
    track.setPixelColor(((word)dist3 % NPIXELS)+i, track.Color(0,0,255-i*50));
    };                 
  }

void draw_car4(void){
  for(int i=0;i<=loop4;i++){
    track.setPixelColor(((word)dist4 % NPIXELS)+i, track.Color(255-i*50,255-i*50,0));
    };                  
  }
  
void loop() {
    for(int i=0;i<NPIXELS;i++){
      track.setPixelColor(i, track.Color(0,0,0));
      };
    if ( (flag_sw1==1) && (digitalRead(PLAYER_1)==0) ) {
      flag_sw1=0;
      speed1+=ACEL;
      };
    if ( (flag_sw1==0) && (digitalRead(PLAYER_1)==1) ) {
      flag_sw1=1;
      };
    
    speed1-=FRICTION*speed1*kf; 
    if (speed1<0) {
      speed1=0;
      };

    if ( (flag_sw2==1) && (digitalRead(PLAYER_2)==0) ) {
      flag_sw2=0;
      speed2+=ACEL;
      };
    if ( (flag_sw2==0) && (digitalRead(PLAYER_2)==1) ) {
      flag_sw2=1;
      };
    
    speed2-=FRICTION*speed2*kf; 
    if (speed2<0) {speed2=0;};

    if ( (flag_sw3==1) && (digitalRead(PLAYER_3)==0) ) {
      flag_sw3=0;
      speed3+=ACEL;
      };
    if ( (flag_sw3==0) && (digitalRead(PLAYER_3)==1) ) {
      flag_sw3=1;
      };
    
    speed3-=FRICTION*speed3*kf; 
    if (speed3<0) {
      speed3=0;
      };

    if ( (flag_sw4==1) && (digitalRead(PLAYER_4)==0) ) {
      flag_sw4=0;
      speed4+=ACEL;
      };
    if ( (flag_sw4==0) && (digitalRead(PLAYER_4)==1) ) {
      flag_sw4=1;
      };
    
    speed4-=FRICTION*speed4*kf; 
    if (speed4<0) {
      speed4=0;
      };
    
    dist1=dist1+speed1;
    dist2=dist2+speed2;
    dist3=dist3+speed3;
    dist4=dist4+speed4;

    if (dist1>dist2||dist1>dist3||dist1||dist4) {
      leader=1;
      } 
    if (dist2>dist1||dist2>dist3||dist2||dist4) {
      leader=2;
      };
    if (dist3>dist1||dist3>dist2||dist3||dist4) {
      leader=3;
      } 
    if (dist4>dist1||dist4>dist2||dist4||dist3) {
      leader=4;
      };
      
    if (dist1>NPIXELS*loop1) {
      loop1++;
      tone(BUZZER_PIN,600);
      TBEEP=2;
      };
    if (dist2>NPIXELS*loop2) {
      loop2++;
      tone(BUZZER_PIN,700);
      TBEEP=2;
    };
    if (dist3>NPIXELS*loop3) {
      loop3++;
      tone(BUZZER_PIN,700);
      TBEEP=2;
    };
    if (dist4>NPIXELS*loop4) {
      loop4++;
      tone(BUZZER_PIN,700);
      TBEEP=2;
    };
    
    if (loop1>loop_max) {
      for(int i=0;i<NPIXELS;i++){
        track.setPixelColor(i, track.Color(0,255,0));
        }; 
        track.show();
        winner_fx();
        start_race();
    }
    if (loop2>loop_max) {
      for(int i=0;i<NPIXELS;i++){
        track.setPixelColor(i, track.Color(255,0,0));
        }; 
        track.show();
        winner_fx();
        start_race();
     }
    if (loop3>loop_max) {
      for(int i=0;i<NPIXELS;i++){
        track.setPixelColor(i, track.Color(0,0,255));
        }; 
        track.show();
        winner_fx();
        start_race();
    }
    if (loop4>loop_max) {
      for(int i=0;i<NPIXELS;i++){
        track.setPixelColor(i, track.Color(255,255,0));
        }; 
        track.show();
        winner_fx();
        start_race();
    }
    
    if ((millis() & 512)==(512*draworder)) {
      if (draworder==0) {
        draworder=1;
        }
      else {
        draworder=0;
        }   
    }; 

    if (draworder==0) {
      draw_car1();
      draw_car2();
      draw_car3();
      draw_car4();
      }
    else {
      draw_car4();
      draw_car3();
      draw_car2();
      draw_car1();
      }   
                 
    track.show(); 
    delay(tdelay);
    
    if (TBEEP>0) {
      TBEEP-=1; 
      if (TBEEP==0) {
        noTone(BUZZER_PIN);
        }; // lib conflict !!!! interruption off by neopixel
    };   
}
