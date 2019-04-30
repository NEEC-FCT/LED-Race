#include <FastLED.h>

//Pins
#define PIN_LED_DATA 2

#define PIN_BUZZER 3

#define PIN_PLAYER1 9
#define PIN_PLAYER2 10
#define PIN_PLAYER3 11
#define PIN_PLAYER4 12

//Track
#define NUM_LEDS 300
#define NUM_LAPS 5

//Physics
#define ACEL 0.6
#define DRAG 0.04

//Player colors
#define C_PLAYER1 CRGB(0, 255, 0)
#define C_PLAYER2 CRGB(255, 0, 0)
#define C_PLAYER3 CRGB(0, 0, 255)
#define C_PLAYER4 CRGB(255, 255, 0)

#define BEEP_FREQUENCY 700
#define BEEP_DURATION 3

#define NOTE_G6  1568
#define NOTE_C7  2093
#define NOTE_E7  2637
#define NOTE_G7  3136

//Global Vars
CRGB leds[NUM_LEDS];

//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};
 
void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
}

float p1_pos;
float p2_pos;
float p3_pos;
float p4_pos;
float p1_speed;
float p2_speed;
float p3_speed;
float p4_speed;

bool p1_btn_last = false;
bool p2_btn_last = false;
bool p3_btn_last = false;
bool p4_btn_last = false;

short beeping = 0;

void drawPlayer1() {
  for (unsigned short i = 0; i <= p1_pos / NUM_LEDS; i++) {
    leds[((unsigned int)p1_pos - i) % NUM_LEDS] = C_PLAYER1;
  }
}
void drawPlayer2() {
  for (unsigned short i = 0; i <= p2_pos / NUM_LEDS; i++) {
    leds[((unsigned int)p2_pos - i) % NUM_LEDS] = C_PLAYER2;
  }
}
void drawPlayer3() {
  for (unsigned short i = 0; i <= p3_pos / NUM_LEDS; i++) {
    leds[((unsigned int)p3_pos - i) % NUM_LEDS] = C_PLAYER3;
  }
}
void drawPlayer4() {
  for (unsigned short i = 0; i <= p4_pos / NUM_LEDS; i++) {
    leds[((unsigned int)p4_pos - i) % NUM_LEDS] = C_PLAYER4;
  }
}

void drawPlayers() {
  //change draw order in order to see players in same pos
  short currentMills = millis() % 1000; // 0 - 999
  if (currentMills < 250) {
    drawPlayer1();
    drawPlayer2();
    drawPlayer3();
    drawPlayer4();
  } else if (currentMills < 500) {
    drawPlayer2();
    drawPlayer3();
    drawPlayer4();
    drawPlayer1();
  } else if (currentMills < 750) {
    drawPlayer3();
    drawPlayer4();
    drawPlayer1();
    drawPlayer2();
  } else {
    drawPlayer4();
    drawPlayer1();
    drawPlayer2();
    drawPlayer3();
  }
}

void checkButtons() {
  //Check if debounce is going to be needed
  bool p1_btn = digitalRead(PIN_PLAYER1);
  bool p2_btn = digitalRead(PIN_PLAYER2);
  bool p3_btn = digitalRead(PIN_PLAYER3);
  bool p4_btn = digitalRead(PIN_PLAYER4);

  if (p1_btn_last == true && p1_btn == false) {
    p1_speed += ACEL;
  }
  if (p2_btn_last == true && p2_btn == false) {
    p2_speed += ACEL;
  }
  if (p3_btn_last == true && p3_btn == false) {
    p3_speed += ACEL;
  }
  if (p4_btn_last == true && p4_btn == false) {
    p4_speed += ACEL;
  }

  p1_btn_last = p1_btn;
  p2_btn_last = p2_btn;
  p3_btn_last = p3_btn;
  p4_btn_last = p4_btn;
}

void applyDrag() {
  p1_speed -= p1_speed * DRAG;
  p2_speed -= p2_speed * DRAG;
  p3_speed -= p3_speed * DRAG;
  p4_speed -= p4_speed * DRAG;
}

void calculatePositions() {
  int temp = (int)p1_pos % NUM_LEDS;
  p1_pos += p1_speed;
  if ((int)p1_pos%NUM_LEDS < temp)
    beep();
  temp = (int)p2_pos % NUM_LEDS;
  p2_pos += p2_speed;
  if ((int)p2_pos%NUM_LEDS < temp)
    beep();
  temp = (int)p3_pos % NUM_LEDS;
  p3_pos += p3_speed;
  if ((int)p3_pos%NUM_LEDS < temp)
    beep();
  temp = (int)p4_pos % NUM_LEDS;
  p4_pos += p4_speed;
  if ((int)p4_pos%NUM_LEDS < temp)
    beep();
}

bool inGame() {
  int max_dist = NUM_LAPS * NUM_LEDS;
  if (p1_pos >= max_dist || p2_pos >= max_dist || p3_pos >= max_dist || p4_pos >= max_dist) {
    return false;
  }
  return true;
}

void beep() {
  tone(PIN_BUZZER, BEEP_FREQUENCY);
  beeping = BEEP_DURATION;
}

void startRace() {
  p1_pos = 0;
  p1_speed = 0;
  p1_btn_last = true;
  p2_pos = 0;
  p2_speed = 0;
  p2_btn_last = true;
  p3_pos = 0;
  p3_speed = 0;
  p3_btn_last = true;
  p4_pos = 0;
  p4_speed = 0;
  p4_btn_last = true;
  beeping = 0;

  FastLED.clear();
  leds[7] = CRGB::Red;
  leds[8] = CRGB::Red;
  leds[9] = CRGB::Red;
  FastLED.show();
  delay(500);
  
  tone(PIN_BUZZER, 440, 500);
  delay(750);

  leds[9] = CRGB::Black;
  FastLED.show();
  tone(PIN_BUZZER, 440, 500);
  delay(750);

  leds[8] = CRGB::Black;
  FastLED.show();
  tone(PIN_BUZZER, 440, 500);
  delay(750);

  leds[7] = CRGB::Green;
  leds[8] = CRGB::Green;
  leds[9] = CRGB::Green;
  FastLED.show();
  tone(PIN_BUZZER, 600, 333);
  delay(333);
}

void winnerSound() {
 int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
      int noteDuration = 1000 / tempo[thisNote];
 
      buzz(PIN_BUZZER, melody[thisNote], noteDuration);
      //30% to separate the notes
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
 
      // stop the tone playing aka noTone:
      buzz(PIN_BUZZER, 0, noteDuration);
    }
}

void showWinner() {
  unsigned int max_dist = NUM_LAPS * NUM_LEDS;
  if (p1_pos >= max_dist) {
    FastLED.clear();
    for (unsigned int i = 0; i < NUM_LEDS/15; i++) {
      leds[i] = C_PLAYER1;
    }
    FastLED.show();
    winnerSound();
  }
  if (p2_pos >= max_dist) {
    FastLED.clear();
    for (unsigned int i = 0; i < NUM_LEDS/15; i++) {
      leds[i] = C_PLAYER2;
    }
    FastLED.show();
    winnerSound();
  }
  if (p3_pos >= max_dist) {
    FastLED.clear();
    for (unsigned int i = 0; i < NUM_LEDS/15; i++) {
      leds[i] = C_PLAYER3;
    }
    FastLED.show();
    winnerSound();
  }
  if (p4_pos >= max_dist) {
    FastLED.clear();
    for (unsigned int i = 0; i < NUM_LEDS/15; i++) {
      leds[i] = C_PLAYER4;
    }
    FastLED.show();
    winnerSound();
  }
}

void standby(){
  //wait for all buttons to be pressed to reset the startRace
  int pos = 0;
  while (digitalRead(PIN_PLAYER1) && digitalRead(PIN_PLAYER2) && digitalRead(PIN_PLAYER3) && digitalRead(PIN_PLAYER4)) {
    delay(100);
    FastLED.clear();
    leds[pos++ % NUM_LEDS] = CRGB(millis() % 256, micros() % 256, (pos * analogRead(1)) % 256);
    FastLED.show();
  }
  FastLED.clear();
}

void setup() {
  FastLED.addLeds<NEOPIXEL, PIN_LED_DATA>(leds, NUM_LEDS);
  pinMode(PIN_PLAYER1, INPUT_PULLUP);
  pinMode(PIN_PLAYER2, INPUT_PULLUP);
  pinMode(PIN_PLAYER3, INPUT_PULLUP);
  pinMode(PIN_PLAYER4, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
}

void loop() {
  standby();
  startRace();
  while (inGame()) {
    checkButtons();
    applyDrag();
    calculatePositions();
    FastLED.clear();
    drawPlayers();
    if (beeping == 0) {
        noTone(PIN_BUZZER);
    }
    if (beeping > 0) {
      beeping--;
    } else {
      FastLED.show();
    }

    //small delay;
    delay(5);
  }
  noTone(PIN_BUZZER); //in case player won while beeping
  showWinner();
}
