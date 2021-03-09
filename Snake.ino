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
  int newX = x;
  int newY = y;
  
  if (analogRead(X_pin) <= 10)
    newX--;
  if (analogRead(X_pin) >= 1000)
    newX++;
  if (analogRead(Y_pin) <= 10)
    newY--;
  if (analogRead(Y_pin) >= 1000)
    newY++;

  moveDot(newX, newY);

  printScreen();

  delay(500);
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
      screen[x][y] = 0;
    }
  }
}

void printScreen()
{
  for(int i=0;i<8;i++)
  {
    lc.setRow(0,i,boolean_to_decimal(screen[i]));
  }
}

byte boolean_to_decimal(bool bol[]){
  int somme=0;
  for (int i = 0; i<8; i++){
    somme += bol[i]*(1 << (7-i));
  }
  return (byte)somme;
}
