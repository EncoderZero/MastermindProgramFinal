/*
Author: Kevin Kan
Version 2.3 Arrayfix
Title: Master mind game for Arduino.*/

/*Function Codes
Serial.print() prints to serial port
Serial.print(results.value,HEX) prints the results in HEX
results.value summons the results of the romote.*/
/* pins 1-7 control rows(through resistors) board wiring 6,1,7,3,8,14,9, 8-11 control columbs board wiring 10,4,5,13 bottom KLM
left side is tape pin 12 is (left bubble facing you?) IR reciver*/
/* For Kit Remote CR2025 Codes are
1=FD08F7,16582903
2=FD8877,16615543
3=FD48B7,16599223
4=FD28D7,16591063
5=,16623703
6=,16607383
repeat=FFFFFFFF
*/

#include <IRremote.h>

int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;
int col[4];
int row[7];
//int show[7][4];
int ATTEMPTS = 7;
int CODE=3;
int count;//This varible controls the columbs and is the combination number.
int game;//This varible controls the rows and is the counter to number of attempts per game
int vic;
const int WIN_COND=3;


/*--------------------------------------------PROGRAM SETUP-----------------------------------------*/
void setup()
{
  int c;
  int r;
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  for (c = 8; c < 12; c++)
  {
    col[c]=c;
    pinMode(c, OUTPUT);
    digitalWrite(c, LOW);
  }
  for(r = 0; r<8; r++)
  {
    row[r]=r;
    pinMode(r,OUTPUT);
    digitalWrite(r,HIGH);
  }
  randomSeed(0); 
}
/*---------------------------------selecition light function----------------------------------------*/
void light()
{
      digitalWrite(8,HIGH);
      digitalWrite(1,LOW);
      delay(100);
      digitalWrite(8,LOW);
      digitalWrite(7,HIGH);
}
/*---------------------------------------Display on the Matrix------------------------------------------*/

void Matrix(int arrayOn[7][4],int wait,int wait2)
{
  for(int a = 0; a<8; a++)
  {
     row[a]=a+1;
  }
  for (int b = 0; b < 4; b++)
    {
     col[b]=b+8;
    }
  for(int r = 0; r<7; r++)
  {
    for (int c = 0; c < 4; c++)
    {//Serial.print (arrayOn[r][c]);// more debugging stuff
      if(arrayOn[r][c]!=0)
      { 
        digitalWrite(col[c],HIGH);
        digitalWrite(row[r],LOW);
        delay(wait);
        digitalWrite(col[c],LOW);
        digitalWrite(row[r],HIGH);
      }
    }
  }
  delay(wait2);
}

/*--------------------------------------------MAIN PROGRAM------------------------------------------------*/
void loop()
{
  Serial.println("Welcome to Mastermind");
  Serial.println("Please choose a number from 1-6");
  game=0;
  int answerKey[4];
  int show[7][4];
  int lastShow[7][4];
  int guess[4];
  for (int g=0; g<4; g++)
  {
    randomSeed(0);
    answerKey[g]=random(1,6);
    // Serial.print(ansewrKey[g]);//uncomment previous for debugging
  }
      for(int x1=0; x1<ATTEMPTS; x1++)
    {
      for( int y1=0; y1<=CODE; y1++)
      {
        show[x1][y1]=1; 
      }
    }
    Matrix(show,5,0);
    for(int x=0; x<ATTEMPTS; x++)
    {
      for( int y=0; y<CODE+1; y++)
      {
        show[x][y]=0;  
      }
    } 
  vic=0;
  int displayMod;
  while(game<ATTEMPTS && vic<4)
  {
    vic=0;
    //static byte last; // Stores last decoded result in case the remote control sends repeat code.
    count=0;
 
    while(count<=CODE)
    {
      
      if (irrecv.decode(&results))
      { //Decode the Ir into appropriate numbers
        
        if(results.value== 16582903)
        {
          guess[count]=1;
              count=count+1;
              Serial.print("For your guess number: ");
              Serial.println(count);
              Serial.print("You guessed :");
              Serial.println('1');
              light();
        }
        if((results.value) ==16615543)
        {
          guess[count]=2;
              count=count+1;
              Serial.print("For your guess number: ");
              Serial.println(count);
              Serial.print("You guessed :");
              Serial.println('2');
              light();
        }
        if(results.value ==16599223)
        {
          guess[count]=3;
              count=count+1;
              Serial.print("For your guess number: ");
              Serial.println(count);
              Serial.print("You guessed :");
              Serial.println('3');
              light();
        }
        if(results.value ==16591063)
        {
          guess[count]=4;
  
            count=count+1;
            Serial.print("For your guess number: ");
            Serial.println(count);
            Serial.print("You guessed :");
            Serial.println('4');
            light();
        }
        if(results.value ==16623703)
        {
          guess[count]=5;
            count=count+1;
            Serial.print("For your guess number: ");
            Serial.println(count);
            Serial.print("You guessed :");
            Serial.println('5');  
            light();       
        }
        if (results.value== 16607383)
        {
          guess[count]=6; 
            count=count+1;
            Serial.print("For your guess number: ");
            Serial.println(count);
            Serial.print("You guessed :");
            Serial.println('6');      
            light();     
        }
         irrecv.resume();
       
      } 
    }
    for(int ch=0;ch<4;ch++)
    {
       if(guess[ch] == answerKey[ch])//check against answerKey
              {
                vic=vic+1;
                }
    }
    displayMod=vic;
    if (vic!=4){
      Serial.print("You have ");
      Serial.print(displayMod); 
      Serial.println(" correct.");
      for(int w=0; w<vic; w++)
      {
        show[game][w]=1;
      }
      game=game+1;
      for (int rep=0;rep<20;rep++){
        Matrix(show,1,0);//Call Matrix(int show) 
      }  
    }  
  }
  if (vic > WIN_COND)
  {
   for(int xw=0; xw<ATTEMPTS; xw++)
    {
      for( int yw=0; yw<=CODE; yw++)
      {
        show[xw][yw]=1; 
      }
    }
  for(int q=0; q<=30; q++)
    {
      Matrix(show,5,5);
    }
    Serial.println("You Win");

  }
  if(vic <= WIN_COND)
  {
    Serial.println("You Lose");
    Serial.print("The answer was : ");
    for (int l; l<=CODE;l++)
    {
      Serial.print(answerKey[l]);
    }
    Serial.println("");
     for(int z=0; z<=10; z++)
    {
      Matrix(show,5,250);
    }
  }
}
