#include <FastLED.h>

//Pins
#define PIN_LED_DATA 2

#define PIN_BUZZER 3

#define PIN_PLAYER1 10
#define PIN_PLAYER2 11
#define PIN_PLAYER3 12
#define PIN_PLAYER4 13

//Track
#define NUM_LEDS 300
#define NUM_LAPS 5

//Physics
#define ACEL 0.2
#define DRAG 0.02

//Player colors
#define C_PLAYER1 CRGB(255, 0, 0)
#define C_PLAYER2 CRGB(0, 255, 0)
#define C_PLAYER3 CRGB(0, 0, 255)
#define C_PLAYER4 CRGB(255, 255, 0)

#define BEEP_FREQUENCY 700
#define BEEP_DURATION 5

//Global Vars
CRGB leds[NUM_LEDS];

unsigned short win_music[] = {
    2637, 2637, 0, 2637,
    0, 2093, 2637, 0,
    3136 //Add missing note plz
};

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

unsigned short beeping = 0;

void drawPlayer1()
{
    for (unsigned short i = 0; i <= p1_pos / NUM_LEDS; i++)
    {
        leds[((unsigned int)p1_pos + i) % NUM_LEDS] = C_PLAYER1;
    }
}
void drawPlayer2()
{
    for (unsigned short i = 0; i <= p2_pos / NUM_LEDS; i++)
    {
        leds[((unsigned int)p2_pos + i) % NUM_LEDS] = C_PLAYER2;
    }
}
void drawPlayer3()
{
    for (unsigned short i = 0; i <= p3_pos / NUM_LEDS; i++)
    {
        leds[((unsigned int)p3_pos + i) % NUM_LEDS] = C_PLAYER3;
    }
}
void drawPlayer4()
{
    for (unsigned short i = 0; i <= p4_pos / NUM_LEDS; i++)
    {
        leds[((unsigned int)p4_pos + i) % NUM_LEDS] = C_PLAYER4;
    }
}

void drawPlayers()
{
    //change draw order in order to see players in same pos
    short currentMills = millis() % 1000; // 0 - 999
    if (currentMills < 250)
    {
        drawPlayer1();
        drawPlayer2();
        drawPlayer3();
        drawPlayer4();
    }
    else if (currentMills < 500)
    {
        drawPlayer2();
        drawPlayer3();
        drawPlayer4();
        drawPlayer1();
    }
    else if (currentMills < 750)
    {
        drawPlayer3();
        drawPlayer4();
        drawPlayer1();
        drawPlayer2();
    }
    else
    {
        drawPlayer4();
        drawPlayer1();
        drawPlayer2();
        drawPlayer3();
    }
}

void checkButtons()
{
    //Check if debounce is going to be needed
    bool p1_btn = digitalRead(PIN_PLAYER1);
    bool p2_btn = digitalRead(PIN_PLAYER2);
    bool p3_btn = digitalRead(PIN_PLAYER3);
    bool p4_btn = digitalRead(PIN_PLAYER4);

    if (p1_btn_last == true && p1_btn == false)
    {
        p1_speed += ACEL;
    }
    if (p2_btn_last == true && p2_btn == false)
    {
        p2_speed += ACEL;
    }
    if (p3_btn_last == true && p3_btn == false)
    {
        p3_speed += ACEL;
    }
    if (p4_btn_last == true && p4_btn == false)
    {
        p4_speed += ACEL;
    }

    p1_btn_last = p1_btn;
    p2_btn_last = p2_btn;
    p3_btn_last = p3_btn;
    p4_btn_last = p4_btn;
}

void applyDrag()
{
    p1_speed -= p1_speed * DRAG;
    p2_speed -= p2_speed * DRAG;
    p3_speed -= p3_speed * DRAG;
    p4_speed -= p4_speed * DRAG;
}

void calculatePositions()
{
    p1_pos += p1_speed;
    p2_pos += p2_speed;
    p3_pos += p3_speed;
    p4_pos += p4_speed;
}

bool inGame()
{
    int max_dist = NUM_LAPS * NUM_LEDS;
    if (p1_pos >= max_dist || p2_pos >= max_dist || p3_pos >= max_dist || p4_pos >= max_dist)
    {
        return false;
    }
    return true;
}

void beep()
{
    tone(PIN_BUZZER, BEEP_FREQUENCY);
    beeping = BEEP_DURATION;
}

void startRace()
{
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

    FastLED.clear();
    leds[7] = CRGB::Red;
    leds[8] = CRGB::Red;
    leds[9] = CRGB::Red;
    FastLED.show();
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
    tone(PIN_BUZZER, 600, 750);
    delay(750);
}

void winnerSound()
{
    unsigned short msize = sizeof(win_music) / sizeof(unsigned short);
    for (unsigned short note = 0; note < msize; note++)
    {
        tone(PIN_BUZZER, win_music[note], 220);
        delay(250);
    }
    delay(250);
}

void showWinner()
{
    unsigned int max_dist = NUM_LAPS * NUM_LEDS;
    if (p1_pos >= max_dist)
    {
        for (unsigned int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = C_PLAYER1;
        }
        FastLED.show();
        winnerSound();
    }
    if (p2_pos >= max_dist)
    {
        for (unsigned int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = C_PLAYER2;
        }
        FastLED.show();
        winnerSound();
    }
    if (p3_pos >= max_dist)
    {
        for (unsigned int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = C_PLAYER3;
        }
        FastLED.show();
        winnerSound();
    }
    if (p4_pos >= max_dist)
    {
        for (unsigned int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = C_PLAYER4;
        }
        FastLED.show();
        winnerSound();
    }
}

void setup()
{
    //init leds  Neopixel???
    FastLED.addLeds<NEOPIXEL, PIN_LED_DATA>(leds, NUM_LEDS);

    pinMode(PIN_PLAYER1, INPUT_PULLUP);
    pinMode(PIN_PLAYER2, INPUT_PULLUP);
    pinMode(PIN_PLAYER3, INPUT_PULLUP);
    pinMode(PIN_PLAYER4, INPUT_PULLUP);
    pinMode(PIN_BUZZER, OUTPUT);
}

void loop()
{
    startRace();
    while (inGame())
    {
        checkButtons();
        applyDrag();
        calculatePositions();
        FastLED.clear();
        drawPlayers();
        if (beeping > 0)
        {
            beeping--;
            if (beeping == 0)
            {
                noTone(PIN_BUZZER);
            }
        }
        else
        {
            FastLED.show();
        }

        //small delay;
        delay(5);
    }
    noTone(PIN_BUZZER); //in case player won while beeping
    showWinner();

    //wait for all buttons to be pressed to reset the startRace
    int pos = 0;
    while (!digitalRead(PIN_PLAYER1) && !digitalRead(PIN_PLAYER2) && !digitalRead(PIN_PLAYER3) && !digitalRead(PIN_PLAYER4))
    {
        delay(20);
        FastLED.clear();
        leds[pos++ % NUM_LEDS] = CRGB(millis() % 256, micros() % 256, (pos * analogRead(1)) % 256);
        FastLED.show();
    }
    FastLED.clear();
    tone(PIN_BUZZER, 500, 250);
    leds[0] = C_PLAYER1;
    FastLED.show();
    delay(100);
    leds[1] = C_PLAYER2;
    FastLED.show();
    delay(100);
    leds[2] = C_PLAYER3;
    FastLED.show();
    delay(100);
    leds[3] = C_PLAYER4;
    FastLED.show();
    delay(200);
    tone(PIN_BUZZER, 500, 250);
    delay(1000);
}