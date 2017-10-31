#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4 // not used / nicht genutzt bei diesem Display
#define DRAW_DELAY 118
#define D_NUM 47
Adafruit_SSD1306 display(OLED_RESET);

const int MONITOR_WIDTH = 128;
const int MONITOR_HEIGHT = 32;
const int buttonPin = 2; 
int lastButtonState = 0; 
int currentPositionX = 0;
int currentPositionY = 0;
int currentDirection = 0;

int counter = 0;
bool gameRunning = false;

void setup()   {        
  Serial.begin(9600);  
  // initialize with the I2C addr 0x3C / mit I2C-Adresse 0x3c initialisieren
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // random start seed / zufälligen Startwert für Random-Funtionen initialisieren
  randomSeed(analogRead(0));

  drawIntro();
  currentPositionX = 20;
  currentPositionY = MONITOR_HEIGHT / 2;
  currentDirection = 1;
  counter = 0;

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  display.clearDisplay();
}

void loop() {
  if (gameRunning) {
    display.clearDisplay();
    counter++;
    if (counter > 100) {
      refreshDirection();
      refreshPosition();
      counter = 0;
    }
  }
}

void drawField() {
  display.drawLine(0, 0, MONITOR_WIDTH, 0, WHITE);
  display.drawLine(0, MONITOR_HEIGHT - 1, MONITOR_WIDTH, MONITOR_HEIGHT - 1, WHITE);
  display.drawLine(0, 0, 0, MONITOR_HEIGHT - 1, WHITE);
  display.drawLine(MONITOR_WIDTH - 1, 0, MONITOR_WIDTH - 1, MONITOR_HEIGHT - 1, WHITE);
}

void refreshPosition() {
  drawField();
  currentPositionX+=2;
  display.drawPixel(currentPositionX, currentPositionY, WHITE);
  display.drawPixel(currentPositionX+1, currentPositionY, WHITE);
  display.drawPixel(currentPositionX+2, currentPositionY, WHITE);
  display.drawPixel(currentPositionX+3, currentPositionY, WHITE);
  display.drawPixel(currentPositionX, currentPositionY-1, WHITE);

  display.display();
}

void refreshDirection() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState == 1 && lastButtonState==0) {
      currentDirection++;
      if (currentDirection > 3) {
        currentDirection = 0;
      }
  }
  lastButtonState = buttonState;
}

void drawIntro() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(12,6);
  display.println("S P A C E");
  display.setTextSize(1);
  display.setCursor(22,22);
  display.println("Press Button...");
  display.display();
  while(digitalRead(buttonPin) == 0) {
    // do nothing...
  }
  gameRunning = true;
}

void gameOver() {
  gameRunning = false;
  display.setCursor(12,10);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println("GAME OVER");
  // draw some random pixel / zufaellige Pixel anzeigen
  for(int i=0;i<100;i++){
    display.drawPixel(random(MONITOR_WIDTH),random(MONITOR_HEIGHT), WHITE);
    display.display();
  }
  setup();
}




