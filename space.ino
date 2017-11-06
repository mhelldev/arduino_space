#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4 // not used / nicht genutzt bei diesem Display
#define DRAW_DELAY 118
#define D_NUM 47
#define SPEED 100
Adafruit_SSD1306 display(OLED_RESET);

const int buttonPin = 2; 
int lastButtonState = 0; 
int currentPositionX = 0;
int currentPositionY = 0;
int currentDirection = 0;

int counter = 0;
bool gameRunning = false;
int starsX[10];
int starsY[10];
int enemyX[1];
int enemyY[1];
int wall[128];

void setup()   {        
  Serial.begin(9600);  
  // initialize with the I2C addr 0x3C / mit I2C-Adresse 0x3c initialisieren
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // random start seed / zufälligen Startwert für Random-Funtionen initialisieren
  randomSeed(analogRead(0));
  currentPositionX = 20;
  currentPositionY = display.height() / 2;
  currentDirection = 1;
  counter = 0;
  initializeStars();
  initializeEnemies();
  initializeWall();
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  display.clearDisplay();
  drawIntro();
}

void initializeStars() {
  for (int i=0; i < 10; i++) {
    starsX[i] = random(display.width());
    starsY[i] = random(display.height());
  }
}

void initializeEnemies() {
  for (int i=0; i < 1; i++) {
    enemyX[i] = random(display.width(), display.width()*2);
    enemyY[i] = random(display.height());
  }
}

void initializeWall() {
  wall[0] = display.height() / 2;
  for (int i = 1; i < 128; i++) {
    int rnd = random(-1,2);
    wall[i] = wall[i-1] + rnd;
    if (wall[i] < 0) {
      wall[i] = 0;
    }
    if (wall[i] > display.height() / 3) {
      wall[i] = display.height() / 3;
    }
  }
}

void loop() {
  if (gameRunning) {
    display.clearDisplay();
    counter++;
    if (counter > SPEED) {
      refreshDirection();
      refreshObjects();
      checkWallCollision();
      counter = 0;
    }
  }
}

void checkWallCollision() {
  if (currentPositionY <= wall[currentPositionX] 
    || currentPositionY >= wall[currentPositionX] + 20) {
    gameOver();
  }
}

void drawField() {
  display.drawLine(0, 0, display.width(), 0, WHITE);
  display.drawLine(0, display.height() - 1, display.width(), display.height() - 1, WHITE);
  display.drawLine(0, 0, 0, display.height() - 1, WHITE);
  display.drawLine(display.width() - 1, 0, display.width() - 1, display.height() - 1, WHITE);
}

int refreshCounter = 0;

void refreshObjects() {
  refreshCounter++;
  drawField();
  //currentPositionX+=2;
  display.drawPixel(currentPositionX, currentPositionY, WHITE);
  display.drawPixel(currentPositionX+1, currentPositionY, WHITE);
  display.drawPixel(currentPositionX+2, currentPositionY, WHITE);
  display.drawPixel(currentPositionX+3, currentPositionY, WHITE);
  display.drawPixel(currentPositionX, currentPositionY-1, WHITE);
  
  if (refreshCounter > 5) {
    refreshCounter = 0;
    currentPositionY = currentPositionY + 1;
  }
  
  refreshStars();
  //refreshEnemies();
  refreshWall();

  display.display();
}

void refreshWall() {
  for (int i=0; i < 127; i++) {
    wall[i] = wall[i+1];
    display.drawLine(i, 0, i, wall[i], WHITE);
    display.drawLine(i, wall[i] + 20, i, display.height(), WHITE);
  }
  int rnd = random(-1,2);
  wall[127] = wall[126] + rnd;
  if (wall[127] < 0) {
    wall[127] = 0;
  }
  if (wall[127] > display.height() / 3) {
    wall[127] = display.height() / 3;
  }
}

void refreshStars() {
  for (int i=0; i < 10; i++) {
    display.drawPixel(starsX[i], starsY[i], WHITE);
    starsX[i] -= 2;
    if (starsX[i] < 0) {
      starsX[i] = display.width();
      starsY[i] = random(display.height());
    }
  }
}

void refreshEnemies() {
   for (int i=0; i < 1; i++) {
    display.drawPixel(enemyX[i], enemyY[i], WHITE);
    display.drawPixel(enemyX[i] + 1, enemyY[i], WHITE);
    display.drawPixel(enemyX[i] + 2, enemyY[i], WHITE);
    display.drawPixel(enemyX[i] + 3, enemyY[i], WHITE);
    display.drawPixel(enemyX[i] + 4, enemyY[i], WHITE);
    display.drawPixel(enemyX[i] + 3, enemyY[i] - 1, WHITE);
    display.drawPixel(enemyX[i] + 3, enemyY[i] - 2, WHITE);
    enemyX[i] -= 1;
    if (enemyX[i] < 0) {
      enemyX[i] = random(display.width(), display.width()*2);
      enemyY[i] = random(display.height());
    }
  }
}

void refreshDirection() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState == 1) {
      currentPositionY = currentPositionY - 1;
  }
}

void drawIntro() {
  while(digitalRead(buttonPin) == 0) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(12,6);
    display.println("S P A C E");
    display.setTextSize(1);
    display.setCursor(22,22);
    display.println("Press Button...");
    refreshStars();
    display.display(); 
  }
  gameRunning = true;
}

void playIntroMelody() {
  
}

void gameOver() {
  gameRunning = false;
  display.setCursor(12,10);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println("GAME OVER");
  // draw some random pixel / zufaellige Pixel anzeigen
  for(int i=0;i<100;i++){
    display.drawPixel(random(display.width()),random(display.height()), WHITE);
    display.display();
  }
  setup();
}




