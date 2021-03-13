#include <LedControl.h>

const int SW_pin = 2;
const int X_pin = 1;
const int Y_pin = 0;

const int DIN = 11;
const int CS =  3;
const int CLK = 13;
LedControl lc=LedControl(DIN,CLK,CS,0);

bool screen[8][8];

int x = 0;
int y = 0;

int joystickX = 0;
int joystickY = 0;
enum direction { north, east, west, south };
direction moveDir = north;

long lastMillis = 0;

void setup(){
  Serial.begin(9600);

  //Set up LCD
  lc.shutdown(0,false);       
  lc.setIntensity(0,1); //Adjust the brightness maximum is 15
  lc.clearDisplay(0);

  //Set up joystick
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);

  //Print array to LCD
  clearScreen();
  printScreen();
}

void loop(){
  long currentMillis = millis();
  
  joystickX = analogRead(X_pin);
  joystickY = analogRead(Y_pin);
  moveDir = getDirection();
  
  if(currentMillis - lastMillis > 1000){
    execute();
    
    lastMillis = currentMillis;
  }
}

void execute() {
  printScreen();
  Serial.print(moveDir);
}

void moveDot (int newX, int newY) {
  screen[x][y] = false;
  if (newX > 7)
    newX = 0;
  if (newX < 0)
    newX = 7;
  if (newY > 7)
    newY = 0;
  if (newY < 0)
    newY = 7;
  screen[newX][newY] = true;
  x = newX;
  y = newY;
}

void clearScreen() {
  for(int x=0;x<8;x++)
  {
    for(int y=0;y<8;y++)
    {
      screen[x][y] = false;
    }
  }
}

void printScreen()
{
  for(int i=0;i<8;i++)
  {
    lc.setRow(0,i,boolToDecimal(screen[i]));
  }
}

int randomApplePosition() {
  int openPositions = 0;
  for (int x = 0; x < 8; x++)
  {
    for (int y = 0; y < 8; y++)
    {
      if (!screen[x][y])
        openPositions++;
    }
  }
  int position = random(0,openPositions-1);
  int i = 0;
  for (int x = 0; x < 8; x++)
  {
    for (int y = 0; y < 8; y++)
    {
      if (!screen[x][y])
      {
        if (i == position)
          return y*8+x;
        i++;
      }
    }
  }
  return 0;
}

byte boolToDecimal(bool row[]){
  int sum=0;
  for (int i = 0; i<8; i++)
  {
    sum += row[i]*(1 << (7-i));
  }
  return (byte)sum;
}

enum direction getDirection() {
  direction dir = moveDir;

  if (joystickX <= 100)
    dir = north;
  else if (joystickX >= 900)
    dir = south;
  else if (joystickY <= 100)
    dir = east;
  else if (joystickY >= 900)
    dir = west;

  return dir;
}
