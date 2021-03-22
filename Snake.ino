#include <LedControl.h>

const int SW_pin = 2;
const int X_pin = 1;
const int Y_pin = 0;

const int DIN = 11;
const int CS =  3;
const int CLK = 13;
LedControl lc=LedControl(DIN,CLK,CS,0);

byte screen[8][8];
byte floorValue = 1;

int joystickX = 0;
int joystickY = 0;
enum direction { none, north, east, west, south };
direction moveDir = south;

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

  screen[1][1] = 1+floorValue;
  screen[2][1] = 2+floorValue;
  screen[3][1] = 3+floorValue;

  screen[5][5] = 0;
  
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
  moveDot(moveDir);
  printScreen();
}

void moveDot (direction Direction) {
  int x = 0;
  int y = 0;
  byte highestValue = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (screen[i][j] > floorValue)
      {
        if (screen[i][j] > highestValue)
        {
          highestValue = screen[i][j];
          x = i;
          y = j;
        }
      }
    }
  }
  
  if (Direction == north)
    x--;
  else if (Direction == east)
    y--;
  else if (Direction == south)
    x++;
  else if (Direction == west)
    y++;

  x = x%8;
  y = y%8;
  
  if (x < 0)
    x = x + 8;
  if (y < 0)
    y = y + 8;

  Serial.print(x);
  Serial.print(':');
  Serial.print(y);
  Serial.print(' ');
  Serial.println(screen[x][y]);
  
  if (screen[x][y] == 0) {
    screen[x][y] = highestValue + 1;
    int positionApple = randomApplePosition();
    x = positionApple - positionApple/8 * 8;
    y = positionApple%8;
    screen[x][y] = 0;
  }
  else
  {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (screen[i][j] > floorValue)
          screen[i][j]--;
      }
    }
    screen[x][y] = highestValue;
  }
}

void clearScreen() {
  for(int x=0;x<8;x++)
  {
    for(int y=0;y<8;y++)
    {
      screen[x][y] = floorValue;
    }
  }
}

void printScreen()
{
  for(int i=0;i<8;i++)
  {
    lc.setRow(0,i,rowToByte(screen[i]));
  }
}

int randomApplePosition() {
  int openPositions = 0;
  for (int x = 0; x < 8; x++)
  {
    for (int y = 0; y < 8; y++)
    {
      if (screen[x][y] == floorValue)
        openPositions++;
    }
  }
  int position = random(0,openPositions-1);
  int i = 0;
  for (int x = 0; x < 8; x++)
  {
    for (int y = 0; y < 8; y++)
    {
      if (screen[x][y] == floorValue)
      {
        if (i == position)
          return y*8+x;
        i++;
      }
    }
  }
  return 0;
}

byte rowToByte(byte row[]){
  byte sum=0;
  for (int i = 0; i<8; i++)
  {
    sum += (row[i] != floorValue)*(1 << (7-i));
  }
  return sum;
}

enum direction getDirection() {
  direction dir = moveDir;

  if (joystickX <= 100 && moveDir != south)
    dir = north;
  else if (joystickX >= 900 && moveDir != north)
    dir = south;
  else if (joystickY <= 100 && moveDir != west)
    dir = east;
  else if (joystickY >= 900 && moveDir != east)
    dir = west;
  return dir;
}
