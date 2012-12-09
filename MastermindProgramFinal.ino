/*
Author: Kevin Kan
Version 2.2 pin fixes
Title: Master mind game for Arduino.*/

/*Function Codes
Serial.print() prints to serial port
Serial.print(results.value,HEX) prints the results in HEX
results.value summons the results of the romote.*/
/* pins 1-7 control rows(through resistors) board wiring 6,1,7,3,8,14,9, 8-11 control columbs board wiring 10,4,5,13 bottom KLM
left side is tape pin 12 is (left bubble facing you?) IR reciver*/

#include <IRremote.h>

int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;
int col[4];
int row[7];
//int show[7][4];
int answerKey[4];
int guess;
int ATTEMPTS = 7;
int CODE=3;
int count;//This varible controls the columbs and is the combination number.
int game;//This varible controls the rows and is the counter to number of attempts per game
int vic;
const int WIN_COND=4;


/*--------------------------------------------PROGRAM SETUP-----------------------------------------*/
void setup()
{
/*  for (int a=7; a<12; a++)
    {
      col[a]=a;
    }
  for (int b =0; b<8; b++)
    {
      row[b]=b;
    }*/
  int c;
  int r;
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  for (c = 8; c < 12; c++)
  {
    col[c]=c;
    pinMode(c, OUTPUT);
    digitalWrite(c, LOW);
    Serial.println(col[c]);
  }
  for(r = 0; r<8; r++)
  {
    row[r]=r;
    pinMode(r,OUTPUT);
    digitalWrite(r,HIGH);
    Serial.println(row[r]);
  }
  randomSeed(0); 
}

/*---------------------------------------Display on the Matrix------------------------------------------*/

void Matrix(int arrayOn[7][4])
{
  for(int a = 1; a<8; a++)
  {
     row[a]=a;
  }
  for (int b = 0; b < 4; b++)
    {
     col[b]=b+8;
    }
  for(int r = 0; r<7; r++)
  {
    for (int c = 0; c < 4; c++)
    {
      if(arrayOn[r][c]!=0)
      { 
        digitalWrite(col[c],HIGH);
        digitalWrite(row[r],LOW);
        delay(10);
        digitalWrite(col[c],LOW);
        digitalWrite(row[r],HIGH);
      }
    }
  }
}

/*--------------------------------------------MAIN PROGRAM------------------------------------------------*/
void loop()
{
  Serial.println("Welcome to Mastermind");
  Serial.println("Please choose a number from 1-6");
  game=0;
  int show[7][4];
  int lastShow[7][4];
  for (int g=0; g<4; g++)
  {
    randomSeed(0);
    answerKey[g]=random(1,6);
     Serial.print(answerKey[g]);
  }
      for(int x1=0; x1<ATTEMPTS; x1++)
    {
      for( int y1=0; y1<=CODE; y1++)
      {
        show[x1][y1]=1; 
       lastShow[x1][y1]=1; 
      }
    } 
    Matrix(show);
    for(int x=0; x<ATTEMPTS; x++)
    {
      for( int y=0; y<CODE+1; y++)
      {
        show[x][y]=0; 
       lastShow[x][y]=0; 
      }
    } 
      digitalWrite(8,HIGH);
      digitalWrite(7,LOW);
      delay(100);
      digitalWrite(8,LOW);
      digitalWrite(7,HIGH);
    /* For Kit Remote CR2025 Codes are
1=FD08F7,16582903
2=FD8877,16615543
3=FD48B7,16599223
4=FD28D7,16591063
5=,16623703
6=,16607383
repeat=FFFFFFFF
  */
  vic=0;
  while(game<ATTEMPTS && vic<4)
  {
    vic=1;
    //static byte last; // Stores last decoded result in case the remote control sends repeat code.
    count=0;
    Matrix(show);
    while(count<=CODE)
    {
      if (irrecv.decode(&results))
      { //Decode the Ir into appropriate numbers
        guess=0;
        if(results.value== 16582903)
        {
          guess=1;
              if(guess == answerKey[count])//check against answerKey
              {
                show[game][count]=1;
                vic=vic+1;
               }
              count=count+1;
              Serial.print("For your guess number: ");
              Serial.println(count);
              Serial.print("You guessed :");
              Serial.println(guess);
        }
        if((results.value) ==16615543)
        {
          guess=2;
              if(guess == answerKey[count])//check against answerKey
              {
                show[game][count]=1;
                vic=vic+1;
                }
              count=count+1;
              Serial.print("For your guess number: ");
              Serial.println(count);
              Serial.print("You guessed :");
              Serial.println(guess);
        }
        if(results.value ==16599223)
        {
          guess=3;
          if(guess == answerKey[count])//check against answerKey
            {
              show[game][count]=1;
              vic=vic+1;
              }
              count=count+1;
              Serial.print("For your guess number: ");
              Serial.println(count);
              Serial.print("You guessed :");
              Serial.println(guess);
        }
        if(results.value ==16591063)
        {
          guess=4;
          if(guess == answerKey[count])//check against answerKey
          {
            show[game][count]=1;
            vic=vic+1;
            }
            count=count+1;
            Serial.print("For your guess number: ");
            Serial.println(count);
            Serial.print("You guessed :");
            Serial.println(guess);
        }
        if(results.value ==16623703)
        {
          guess=5;
          if(guess == answerKey[count])//check against answerKey
          {
            show[game][count]=1;
            vic=vic+1;
            }
            count=count+1;
            Serial.print("For your guess number: ");
            Serial.println(count);
            Serial.print("You guessed :");
            Serial.println(guess);         
        }
        if (results.value== 16607383)
        {
          guess=6; 
          if(guess == answerKey[count])//check against answerKey
          {
            show[game][count]=1;
            vic=vic+1;
            }
            count=count+1;
            Serial.print("For your guess number: ");
            Serial.println(count);
            Serial.print("You guessed :");
            Serial.println(guess);           
        }
         irrecv.resume();
       
      } 
      if (count<CODE)
      {
        Matrix(lastShow);
      }
      else{
        Matrix(show);
        for (int t=0; t<=CODE; t++)
        {
          lastShow[game][t]=show[game][t]; 
        }
      }
      
    }
    Serial.print("You have ");
    Serial.print(vic); 
    Serial.println(" correct.");
    game=game+1;
    //Call Matrix(int show) 
  }
  if (vic >= WIN_COND)
  {
    Serial.println("You Win");

  }
  if(vic < WIN_COND)
  {
    Serial.println("You Lose");
    Serial.print("The answer was : ");
    for (int l; l<=CODE;l++)
    {
      Serial.print(answerKey[l]);
    }
    Serial.println("");
  }
}
