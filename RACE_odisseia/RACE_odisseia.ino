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

//Ghost
#define GBASESPD 1.4
#define GSPDINC 0.1

//Player colors
#define C_PLAYER1 CRGB(0, 255, 0)
#define C_PLAYER2 CRGB(255, 0, 0)
#define C_PLAYER3 CRGB(0, 0, 255)
#define C_PLAYER4 CRGB(255, 255, 0)
#define C_PLAYERG CRGB(255,255,255)

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
float pg_pos;
float p1_speed;
float p2_speed;
float p3_speed;
float p4_speed;
float pg_speed;

bool p1_btn_last = false;
bool p2_btn_last = false;
bool p3_btn_last = false;
bool p4_btn_last = false;

bool p1_finish = false;
bool p2_finish = false;
bool p3_finish = false;
bool p4_finish = false;

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

void drawPlayerG() {
  for (unsigned short i = 0; i <= pg_pos / NUM_LEDS; i++) {
    leds[((unsigned int)pg_pos - i) % NUM_LEDS] = C_PLAYERG;
  }
}
void drawPlayers() {
  //change draw order in order to see players in same pos
  short currentMills = millis() % 1000; // 0 - 999
  drawPlayerG();
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
  int max_dist = NUM_LEDS * NUM_LAPS;
  int temp = (int)p1_pos % NUM_LEDS;
  p1_pos += p1_speed;
  if ((int)p1_pos % NUM_LEDS < temp)
    beep();
  if (p1_pos > max_dist)
    p1_finish = true;
  temp = (int)p2_pos % NUM_LEDS;
  p2_pos += p2_speed;
  if ((int)p2_pos % NUM_LEDS < temp)
    beep();
  if (p2_pos > max_dist)
    p2_finish = true;
  temp = (int)p3_pos % NUM_LEDS;
  p3_pos += p3_speed;
  if ((int)p3_pos % NUM_LEDS < temp)
    beep();
  if (p3_pos > max_dist)
    p3_finish = true;
  temp = (int)p4_pos % NUM_LEDS;
  p4_pos += p4_speed;
  if ((int)p4_pos % NUM_LEDS < temp)
    beep();
  if (p4_pos > max_dist)
    p4_finish = true;
  temp = (int)pg_pos % NUM_LEDS;
  pg_pos += pg_speed;
  if ((int)pg_pos % NUM_LEDS < temp)
    beep();
}

bool inGame() {
  int max_dist = NUM_LAPS * NUM_LEDS;
  if (pg_pos >= max_dist) {
    return false;
  }
  return true;
}

void beep() {
  tone(PIN_BUZZER, BEEP_FREQUENCY);
  beeping = BEEP_DURATION;
}

void startRace(float spd) {
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
  pg_pos = 0;
  pg_speed = spd;
  beeping = 0;

  p1_finish = false;
  p2_finish = false;
  p3_finish = false;
  p4_finish = false;

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
  FastLED.clear();
  int num = 0;
  if (p1_finish) {
    leds[0] = C_PLAYER1;
    leds[1] = C_PLAYER1;
    leds[2] = C_PLAYER1;
    num++;
  }

  if (p2_finish) {
    leds[3] = C_PLAYER2;
    leds[4] = C_PLAYER2;
    leds[5] = C_PLAYER2;
    num++;
  }

  if (p3_finish) {
    leds[6] = C_PLAYER3;
    leds[7] = C_PLAYER3;
    leds[8] = C_PLAYER3;
    num++;
  }

  if (p4_finish) {
    leds[9] = C_PLAYER4;
    leds[10] = C_PLAYER4;
    leds[11] = C_PLAYER4;
    num++;
  }


  if (!p1_finish && !p2_finish && !p3_finish && !p4_finish) {
    for (unsigned int i = 0; i < NUM_LEDS / 7; i++) {
      leds[i] = C_PLAYERG;
    }
  }
  FastLED.show();
  Serial.print(num);
  Serial.println(" jogadores venceram o fantasma!");
  winnerSound();

}

float standby() {
  //wait for all buttons to be pressed to reset the startRace
  int pos = 0;
  int dificulty = -1;
  Serial.println("Enviar a dificuldade de 0-5: ");
  while (Serial.available()) {
    Serial.read();
  }
  while (true) {
    delay(100);
    FastLED.clear();
    leds[pos++ % NUM_LEDS] = CRGB(millis() % 256, micros() % 256, (pos * analogRead(1)) % 256);
    FastLED.show();
    if (Serial.available()) {
      dificulty = Serial.parseInt();
      if (dificulty >= 0 && dificulty < 6)
        return GBASESPD + (GSPDINC * dificulty);
    }
  }
  FastLED.clear();
  return GBASESPD + (GSPDINC * dificulty);
}

void setup() {
  FastLED.addLeds<NEOPIXEL, PIN_LED_DATA>(leds, NUM_LEDS);
  pinMode(PIN_PLAYER1, INPUT_PULLUP);
  pinMode(PIN_PLAYER2, INPUT_PULLUP);
  pinMode(PIN_PLAYER3, INPUT_PULLUP);
  pinMode(PIN_PLAYER4, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  Serial.begin(57600);
}

void loop() {
  float spd;
  spd = standby();
  startRace(spd);
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
