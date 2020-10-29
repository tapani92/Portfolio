/*
 * RISTINOLLA, MUSTIPELI & REAKTIOPELI COMBINED
 * 
 */
 

// define button target input values
#define BUTTON_1 55
#define BUTTON_2 131
#define BUTTON_3 225
#define BUTTON_4 337
#define BUTTON_5 442
#define BUTTON_6 510
#define BUTTON_7 614
#define BUTTON_8 766
#define BUTTON_9 944
#define BUTTON_10 1020

#define THRESHOLD 10 // threshold for reading input values

#define BUTTON_INPUT A0 // pin that reads button input

// led output pins. A = anode (long leg), C = cathode
// pins 1 & 0 doesn't work with serial communication. comment/remove all serial stuff from code before plugging these
#define LED_1_A 12
#define LED_1_C 13
#define LED_2_A 6 
#define LED_2_C 7 
#define LED_3_A 2 
#define LED_3_C 3 
#define LED_4_A 10
#define LED_4_C 11
#define LED_5_A 4
#define LED_5_C 5
#define LED_6_A 0 // RX pin! unplug for uploading code to Arduino!
#define LED_6_C 1 // TX pin! unplug for uploading code to Arduino!
#define LED_7_A 8
#define LED_7_C 9
#define LED_8_A 16 // A2 pin
#define LED_8_C 17 // A3 pin
#define LED_9_A 18 // A4 pin
#define LED_9_C 19 // A5 pin

// led states as verbose
#define OFF 0
#define RED 1
#define GREEN 2

bool acceptInput = 1; // is a new button press being accepted
// button values in an array for easy accessing
int buttonValues[10] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8, BUTTON_9, BUTTON_10};

// same with led pin numbers
int leds[9][2] = {{LED_1_A, LED_1_C}, {LED_2_A, LED_2_C}, {LED_3_A, LED_3_C}, {LED_4_A, LED_4_C}, {LED_5_A, LED_5_C}, {LED_6_A, LED_6_C},{LED_7_A, LED_7_C},{LED_8_A, LED_8_C},{LED_9_A, LED_9_C}};

// array of led states variables for leds 1-9, respectively. initialized as off. array for easy access
int ledstates[9] = {0,0,0,0,0,0,0,0,0};

int activeButton = 0; // the button that is currently being pressed
int keyUp = 0; // when a button is released, this will have that button's number value for one loop iteration
int keyDown = 0; // when a button is pressed, this will have that button's number value for one loop iteration

int selectedGame = 0; // current game selection
volatile unsigned long waitTime = 0; // counter variable used by Wait & WaitInterruptable, incremented by Arduinos timer interrupt service routine
volatile unsigned delayTime = 0; // counter variable used by InputDelay, incremented by Arduinos timer interrupt service routine
bool reset = false; // flag for resetting and returning to game selection menu, skips stuff when true
int resetTimer = 0; // reset button hold time counter

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// REACTION GAME VARIABLES:
bool RG_initialize = true;
int RG_score = 0;
int RG_timer = 0;
int RG_timeLimit = 1000;
int RG_randomLed = 0;
///////////////////////////////////////////////////////////////////////////

/////////////////MEMORY GAME VARIABLES/////////////
int MG_sequence[100]; // array for game sequence
int MG_arraySize = 0; //current array size
int MG_answer = 0;    //used in PLAYER INPUT SEQUENCE to keep track on anwers 
int MG_score = 0;    //keeps score about lenght of the longest correct answer sequence
/////////////////memory game variables END//////////////

///////////////////////////////////////////////////////////////////////////
// TIC TAC TOE GAME VARIABLES:
int TTTwinner=0;
int TTTplayer=0;
int TTTplayerColour=RED;
int board[9]={0,0,0,0,0,0,0,0,0};
///////////////// TTT game variables END //////////////

void setup()
{
  //Serial.begin(9600); // for debugging
  
  // ARDUINO TIMER 1 SETUP:
  noInterrupts(); // stop interrupts
  //set timer1 interrupt at 1000 Hz (1 ms)
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1 = 0;//initialize counter value to 0
  // set compare match register for 1000 hz increments.
  //compare match register = [ 16,000,000Hz/ (prescaler * desired interrupt frequency) ] - 1
  OCR1A = 1999;// 1000 Hz interval, 8 prescaler.  (must be <65536 for timer1)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  interrupts(); // enable interrupts
  // END OF TIMER SETUP
  
  randomSeed(analogRead(A1)); //random seed read from the floating A1 pin

  
  pinMode(BUTTON_INPUT, INPUT); // button input read pin
  pinMode(LED_1_A, OUTPUT);
  pinMode(LED_1_C, OUTPUT);
  pinMode(LED_2_A, OUTPUT);
  pinMode(LED_2_C, OUTPUT);
  pinMode(LED_3_A, OUTPUT);
  pinMode(LED_3_C, OUTPUT);
  pinMode(LED_4_A, OUTPUT);
  pinMode(LED_4_C, OUTPUT);
  pinMode(LED_5_A, OUTPUT);
  pinMode(LED_5_C, OUTPUT);
  pinMode(LED_6_A, OUTPUT);
  pinMode(LED_6_C, OUTPUT);
  pinMode(LED_7_A, OUTPUT);
  pinMode(LED_7_C, OUTPUT);
  pinMode(LED_8_A, OUTPUT);
  pinMode(LED_8_C, OUTPUT);
  pinMode(LED_9_A, OUTPUT);
  pinMode(LED_9_C, OUTPUT);
 
}

void loop()
{

  // reset game variables
  if (reset) {
    // reset memory game:
    MG_answer = 0;
    MG_arraySize = 0;
    MG_score = 0;
    // reset reaction game:
    RG_randomLed = 0;
    RG_score = 0;
    RG_timer = 0;
    RG_timeLimit = 1000;
    RG_initialize = true; // for triggering RG starting animation
    // reset tic tac toe game:
    XO_Initialize();
    reset = false; // reset reset flag
  }
  
  selectedGame = MenuAnim(); // game selection with animation
  
  while(selectedGame == 1) {
    MG_Main(); // MEMORY GAME MAIN LOOP
  }
  while(selectedGame == 2) {
    RG_Main(); // REACTION GAME MAIN LOOP
  }
  while(selectedGame == 3) {
    XO_Game(); // TIC TAC TOE GAME MAIN LOOP
  }
  
  RG_ResetLeds();
  
  
} // void (main) loop end 

int MenuAnim() {
  int infinitySequence[6] = {7, 4, 5, 6, 9, 8};
  int green = 0;
  int ledDelay = 25;
  int selection = 0;
  RG_ResetLeds();
  while(!reset && (activeButton != 1 && activeButton != 2 && activeButton != 3)) {
    for (int i = 1; i<4; i++) {
      SetLedState(i, green);
    }
    // flip upper row led state off/green
    if (green == 0) {
      green = 2;
    } else {
      green = 0;
    }
    SetLedState(7, RED);
    SetLedState(8, OFF);
    WriteAllLedOutputs();
    WaitInterruptable(ledDelay*2); // wait while reading all button inputs
    for(int i = 1; i < 7; i++) {
      SetLedState(infinitySequence[i], RED);
      SetLedState(infinitySequence[i-1], OFF);
      WriteAllLedOutputs();
      WaitInterruptable(ledDelay*2);
    }
    GetButtonInput(); // read button input
  }
  selection = activeButton;
  RG_ResetLeds();
  for(int endIterations = 0; endIterations < 10; endIterations++) {
    SetLedState(selection, green);
    // flip selected led state off/green
    if (green == 0) {
      green = 2;
    } else {
      green = 0;
    }
    SetLedState(7, RED);
    SetLedState(8, OFF);
    WriteAllLedOutputs();
    Wait(ledDelay);
    for(int i = 1; i < 7; i++) {
      SetLedState(infinitySequence[i], RED);
      SetLedState(infinitySequence[i-1], OFF);
      WriteAllLedOutputs();
      Wait(ledDelay);
    }
  }
  SetLedState(selection, GREEN);
  Wait(600);
  RG_ResetLeds();
  return selection;
}

///////////////////////////////////////////////////////////////
// REACTION GAME FUNCTIONS: ///////////////////////////////////
///////////////////////////////////////////////////////////////

// initialize the reaction game
void RG_Initialize() {
  RG_randomLed = 0;
  RG_score = 0;
  RG_timer = 0;
  RG_timeLimit = 1000;
  RG_initialize = false;
  // flash all leds green
  for (int i=1; i<10; i++) {
    SetLedState(i, GREEN);
  }
  WriteAllLedOutputs();
  Wait(400);
  RG_ResetLeds();
  WriteAllLedOutputs();
  Wait(2000);
}
// reaction game main loop:
void RG_Main() {
  // initialize the reaction game if triggered to do so
  if (RG_initialize) {
    RG_Initialize(); 
  }

  // if no led is active, activate a random led
  if (RG_randomLed == 0) {
    RG_ResetLeds();
    RG_LightRandomLed();
  }
  
  // if the timer exceeds the time limit while a random led is active
  if (RG_timer >= RG_timeLimit) {
    // and if a GREEN led was lit: game over
    if (ledstates[RG_randomLed-1] == GREEN) {
      RG_Fail();
    }
    // otherwise just randomize a new led
    RG_ResetLeds();
    RG_LightRandomLed();
    Wait(random(250, 500));
    RG_timer = 0;
  }
  
  GetButtonInput(); // read button input value and update global input variables, check for reset button
  RG_timer += 2; // increment RG_timer, add the milliseconds spent by GetButtonInput's input reading
  
  // while a random led is active:
  // if any of the main game buttons (1-9) are pressed
  if (activeButton > 0 && activeButton < 10) {
    // if the pressed key matches the currently active led
    if (activeButton == RG_randomLed) {
      // and if the led is green
      if (ledstates[RG_randomLed-1] == GREEN) {
        RG_Score();
      } else {
        RG_Fail();
      }
    } else {
      RG_Fail();
    }
  }
  
  WriteAllLedOutputs(); // updates all leds states on the hardware
  
/*
Serial.print("randomLed: ");
Serial.print(RG_randomLed);
Serial.print("\t\tlastButton: ");
Serial.print(RG_lastButton);
Serial.print("\t\tScore: ");
Serial.print(RG_score);
Serial.print("\t\tTimer: ");
Serial.print(RG_timer);
Serial.println("");
*/

} // end RG_Main()


// reset the timer and leds
void RG_ResetLeds() {
  for(int i=1; i < 10; i++) {
      SetLedState(i, OFF); // turn all led states off
  }
  WriteAllLedOutputs();
}
// light a random (unlit) led
void RG_LightRandomLed() {
  RG_randomLed = 0;
  while(true) {
    // randomize until we get an unlit led
    RG_randomLed = random(1, 10);
    if(ledstates[RG_randomLed-1] == 0) {
      // if score is high enough, led may turn RED/GREEN
      if (RG_score > 3) {
        SetLedState(RG_randomLed, random(1,3));
      } else {
        SetLedState(RG_randomLed, GREEN); // otherwise only GREEN
      }
      break; // exit loop
    }
  }
}

// score in the reaction game
void RG_Score() {
  // first some fancy led flashing!
  for(int i=0; i<3; i++) {
    SetLedState(RG_randomLed, GREEN);
    WriteAllLedOutputs();
    //delay(200);
    Wait(200);
    SetLedState(RG_randomLed, OFF);
    WriteAllLedOutputs();
    //delay(200);
    Wait(200);
  }
  //delay(random(500, 1501)); // random delay before resuming game
  Wait(random(250, 1501));
  // adjust variables:
  RG_score++;
  RG_timer = 0;
  // tighten the reaction time window for user input, cap it at 100 ms
  if (RG_timeLimit > 500) {
    RG_timeLimit -= 50;
  } else if (RG_timeLimit > 100) {
    RG_timeLimit -= 25;
  }
  RG_randomLed = 0;
}

void RG_Fail() {
  // first some fancy led flashing!
  for(int i=0; i<11; i++) {
    SetLedState(RG_randomLed, RED);
    WriteAllLedOutputs();
    //delay(100);
    Wait(100);
    SetLedState(RG_randomLed, OFF);
    WriteAllLedOutputs();
    //delay(100);
    Wait(100);
  }
  RG_GameOver(RG_score);
  RG_Initialize();
}

// display score level and reset the game
void RG_GameOver(int score) {
  if (score > 0) {
    int scoreTens = score / 10;
    int scoreOnes = score % 10;
  
    // display the 10 digit score, in sequence:
    if (scoreTens > 0) {
      RG_ResetLeds();
      for (int i=1; i <= scoreTens && i < 10; i++) {
        SetLedState(i, GREEN);
        WriteAllLedOutputs();
        Wait(250);
      }
      WaitInterruptable(400);
    }
    // display the one digit score, in sequence:
    RG_ResetLeds();
    for (int i=1; i <= scoreOnes && i < 10; i++) {
      SetLedState(i, RED);
      WriteAllLedOutputs();
      Wait(250);
    }
    Wait(400);
    // flash score till a button is pressed
    do {
      RG_ResetLeds();
      for (int i=1; i <= scoreOnes && i < 10; i++) {
        SetLedState(i, RED);
        WriteAllLedOutputs();
      }
      WaitInterruptable(400);
      RG_ResetLeds();
      WaitInterruptable(50);
      for (int i=1; i <= scoreTens && i < 10; i++) {
        SetLedState(i, GREEN);
        WriteAllLedOutputs();
      }
      WaitInterruptable(400);
      RG_ResetLeds();
      WaitInterruptable(50);
      // read button input and kill loop if a button is pressed
      GetButtonInput();
    } while(activeButton == 0);
  }
  RG_ResetLeds();
}

//////////////////////////////////////////////////////////////////////////
///////MEMORY GAME FUNCTIONS//////////////////////////////////////////////

//FLASH ONE LED
//Flash chosen LED  chosen colour
void MG_FlashOneLed(int LED_NUMBER, int COLOUR, int TIME)
{  
    SetLedState(LED_NUMBER, COLOUR);
    WriteAllLedOutputs();
    Wait(TIME);

    SetLedState(LED_NUMBER, OFF);
    WriteAllLedOutputs();      
}




//FLASH ALL LEDS
//Flash all LEDs chosen times with chosen colour
//can be also used to turn all LEDs off by setting colour to "OFF" and xtimes & time to 1
void MG_FlashAllLeds(int COLOUR, int XTIMES, int TIME)
{
for (int j = 0; j < XTIMES; j++) //indicated by flashing all LEDs green
  {
        for (int i = 1; i <= 9; i++)
        {
          SetLedState(i, COLOUR);
        }
        WriteAllLedOutputs();
        Wait(TIME);
        
        for (int i = 1; i <= 9; i++)
        {
          SetLedState(i, OFF);
        }
        WriteAllLedOutputs();
        Wait(TIME);
  }
}

void MG_Main()
{

////////////MEMORY GAME////////////
 //Serial.println("uusi kierros");
 
//RANDOM NUMBER
 MG_arraySize++; //increases current array size by one
 MG_sequence[MG_arraySize] = random(9) + 1;  // creates random number between 1-9 and adds it as last number in "sequence" -array.
  // Serial.println(MG_sequence[MG_arraySize]);

//BEGINING OF NEW ROUND
  MG_FlashAllLeds(GREEN, 3, 200); // beginning of a new round is indicated by flashing all LEDs green

//LED SEQUENCE
 for (int j = 1; j <= MG_arraySize; j++)
 { 
   MG_FlashOneLed(MG_sequence[j], RED, 500);
   Wait(500);
 }
//led sequence END


//PLAYER INPUT SEQUENCE
//Serial.println("odottaa vastauksia");
MG_FlashAllLeds(GREEN, 1, 50); //indicated by a quick flash of green with all LEDs.
for (int j = 1; j <= MG_arraySize; j++)
  {
    while (!reset) //waits a player input
    {
      GetButtonInput(); // read input
      if (keyDown > 0 && keyDown != 10) 
      {
        MG_answer = activeButton;
        break;
      }
    }

      //WRONG ANSWER
      if (MG_answer != MG_sequence[j]) 
      {
          MG_FlashOneLed(MG_answer, RED, 50);
          Wait(200);
          MG_FlashAllLeds(RED, 3, 200); //indicated by flashing all LEDs red
          
          /*MG_FlashOneLed (MG_answer, RED, 200); //tästä en oo ihan varma onko parempi ilman
          delay(200);
          MG_FlashOneLed (MG_answer, RED, 200);
          delay(200);
          MG_FlashOneLed (MG_answer, RED, 1000);*/
          Wait(1000);
          MG_FlashOneLed(MG_sequence[j], GREEN, 200);
          Wait(200);
          MG_FlashOneLed(MG_sequence[j], GREEN, 200);
          Wait(200);
          MG_FlashOneLed(MG_sequence[j], GREEN, 200);
          Wait(2000);

          
          RG_GameOver(MG_score); // game over animation & score display
          MG_answer = 0;
          MG_arraySize = 0;
          MG_score = 0;
          //selectedGame = 0;
                 
        //wrong answer END       
      } else {
     
        //CORRECT ANSWER
        //Serial.println("oikein");
        MG_FlashOneLed(MG_sequence[j], GREEN, 50);        
        Wait(200);
        MG_answer = 0;
                  
        if (j == MG_score +1)
        {
          MG_score++;
        }
      }
       //correct answer END   
  }
 //player input sequence END
 
 
//returs back to the begining of MEMORY GAME -main loop
/////////////////memory game END//////////////

} // MG_Main end


///////memory game functions END///////////
///////////////////////////////////////////////////////////////////////

  ////////////////////////////
 //!!!!RISTINOLLA GAME!!!!//
///////////////////////////

void XO_Initialize() {
  
  TTTplayer=0;
  TTTwinner=0;
  TTTplayerColour=RED;
  for(int i = 0; i < 9; i++)
  {
    board[i]=0;
  }
}

void XO_Game()
{

  while(!reset) {
  
    
  //START ANIMATION
    SetLedState(1,RED);
    SetLedState(2,OFF);
    SetLedState(3,RED);
    SetLedState(4,OFF);
    SetLedState(5,RED);
    SetLedState(6,OFF);
    SetLedState(7,RED);
    SetLedState(8,OFF);
    SetLedState(9,RED);
    WriteAllLedOutputs();
    Wait(800);
    SetLedState(1,OFF);
    SetLedState(2,OFF);
    SetLedState(3,OFF);
    SetLedState(4,OFF);
    SetLedState(5,OFF);
    SetLedState(6,OFF);
    SetLedState(7,OFF);
    SetLedState(8,OFF);
    SetLedState(9,OFF);
    WriteAllLedOutputs();
    Wait(500);
    SetLedState(1,OFF);
    SetLedState(2,GREEN);
    SetLedState(3,OFF);
    SetLedState(4,GREEN);
    SetLedState(5,OFF);
    SetLedState(6,GREEN);
    SetLedState(7,OFF);
    SetLedState(8,GREEN);
    SetLedState(9,OFF);
    WriteAllLedOutputs();
    Wait(800);
      SetLedState(1,OFF);
    SetLedState(2,OFF);
    SetLedState(3,OFF);
    SetLedState(4,OFF);
    SetLedState(5,OFF);
    SetLedState(6,OFF);
    SetLedState(7,OFF);
    SetLedState(8,OFF);
    SetLedState(9,OFF);
    WriteAllLedOutputs();
    Wait(500);
  
  //GAME CONTROL
   for (int i=0; i<9; i++) {
  
      while(!reset)
      {
       // read raw button input value and convert it to a button press 
       GetButtonInput();
        // on keydown, if the corresponding led is not "taken" and if the key was not button 10:
        if(keyDown > 0 && board[keyDown-1] !=1 && keyDown !=10)
        {
          // mark player selection on the screen
          TTTplayer=activeButton;
          board[activeButton-1] = 1; // mark that led as "taken"
          break;
        } else if (activeButton==10)       //If player push RESET button, the game restarts!
        {
          XO_Initialize(); // reset variables
          selectedGame = 0; // back to game selection menu
          reset = true;
          break; // break while
        }
      } // end while
      
      SetLedState(TTTplayer, TTTplayerColour);
      WriteAllLedOutputs();
  
  
  
  
  
    
    //If there´s been made three moves or more, program starts to check every move if there´s three straight lines
    if(i>=2) 
    {
      
      
        if(ledstates[0]==RED && ledstates[1]==RED && ledstates[2]==RED)             //RED winnings
      {
        TTTwinner=RED;
      }
      
      if(ledstates[3]==RED && ledstates[4]==RED && ledstates[5]==RED)
      {
        TTTwinner=RED;
      }
      
      if(ledstates[6]==RED && ledstates[7]==RED && ledstates[8]==RED)
      {
        TTTwinner=RED;
      }
      
      if(ledstates[0]==RED && ledstates[3]==RED && ledstates[6]==RED)
      {
        TTTwinner=RED;
      }
      
      if(ledstates[1]==RED && ledstates[4]==RED && ledstates[7]==RED)
      {
        TTTwinner=RED;
      }
      
      if(ledstates[2]==RED && ledstates[5]==RED && ledstates[8]==RED)
      {
        TTTwinner=RED;
      }
      
      if(ledstates[0]==RED && ledstates[4]==RED && ledstates[8]==RED)
      {
        TTTwinner=RED;
      }
      
      if(ledstates[6]==RED && ledstates[4]==RED && ledstates[2]==RED)
      {
        TTTwinner=RED;
      }
      
      
      
      
      if(ledstates[0]==GREEN && ledstates[1]==GREEN && ledstates[2]==GREEN)     //GREEN winnings
      {
        TTTwinner=GREEN;
      }
      
      if(ledstates[3]==GREEN && ledstates[4]==GREEN && ledstates[5]==GREEN)
      {
        TTTwinner=GREEN;
      }
      
      if(ledstates[6]==GREEN && ledstates[7]==GREEN && ledstates[8]==GREEN)
      {
        TTTwinner=GREEN;
      }
      
      if(ledstates[0]==GREEN && ledstates[4]==GREEN && ledstates[8]==GREEN)
      {
        TTTwinner=GREEN;
      }
      
      if(ledstates[6]==GREEN && ledstates[4]==GREEN && ledstates[2]==GREEN)
      {
        TTTwinner=GREEN;
        
      }
      
      if(ledstates[0]==GREEN && ledstates[3]==GREEN && ledstates[6]==GREEN)
      {
        TTTwinner=GREEN;
      }
      
      if(ledstates[1]==GREEN && ledstates[4]==GREEN && ledstates[7]==GREEN)
      {
        TTTwinner=GREEN;
      }
      
      if(ledstates[2]==GREEN && ledstates[5]==GREEN && ledstates[8]==GREEN)
      {
        TTTwinner=GREEN;
        
      }
  
    
  
    if(TTTwinner==RED)
     {
      for(int j=0;j<4;j++)      //If RED wins, LED animation starts by ceating four times LED X!
      {
        SetLedState(1,RED);
        SetLedState(2,OFF);
        SetLedState(3,RED);
        SetLedState(4,OFF);
        SetLedState(5,RED);
        SetLedState(6,OFF);
        SetLedState(7,RED);
        SetLedState(8,OFF);
        SetLedState(9,RED);
        WriteAllLedOutputs();
        Wait(500);
        SetLedState(1,OFF);
        SetLedState(2,OFF);
        SetLedState(3,OFF);
        SetLedState(4,OFF);
        SetLedState(5,OFF);
        SetLedState(6,OFF);
        SetLedState(7,OFF);
        SetLedState(8,OFF);
        SetLedState(9,OFF);
        WriteAllLedOutputs();
        Wait(500);
      }
      Wait(1000);
      
      // reset variables
      XO_Initialize();
      break; // break for loop

  }
  
  if(TTTwinner==GREEN)       //If GREEN wins, LED animation starts by creating four times LED circle!
  {
    for(int k=0;k<4;k++)    //Led lights appear 4 times!
    {
    SetLedState(1,OFF);
    SetLedState(2,GREEN);
    SetLedState(3,OFF);
    SetLedState(4,GREEN);
    SetLedState(5,OFF);
    SetLedState(6,GREEN);
    SetLedState(7,OFF);
    SetLedState(8,GREEN);
    SetLedState(9,OFF);
    WriteAllLedOutputs();
    Wait(500);
    SetLedState(1,OFF);
    SetLedState(2,OFF);
    SetLedState(3,OFF);
    SetLedState(4,OFF);
    SetLedState(5,OFF);
    SetLedState(6,OFF);
    SetLedState(7,OFF);
    SetLedState(8,OFF);
    SetLedState(9,OFF);
    WriteAllLedOutputs();
    Wait(500);
    }
  Wait(1000);

        // reset variables
        XO_Initialize();
        break; // break for loop

  
  }
  if(i==8 && TTTwinner == 0)           //If neither GREEN or RED gets three straight lines, but there´s been made 9 moves-->DRAW!!!
  {
    for(int x=0;x<3;x++)    //LED lights appear 3 times!
    {
    SetLedState(1,GREEN);
    SetLedState(2,GREEN);
    SetLedState(3,GREEN);
    SetLedState(4,GREEN);
    SetLedState(5,GREEN);
    SetLedState(6,GREEN);
    SetLedState(7,GREEN);
    SetLedState(8,GREEN);
    SetLedState(9,GREEN);
    WriteAllLedOutputs();
    Wait(500);
    SetLedState(1,OFF);
    SetLedState(2,OFF);
    SetLedState(3,OFF);
    SetLedState(4,OFF);
    SetLedState(5,OFF);
    SetLedState(6,OFF);
    SetLedState(7,OFF);
    SetLedState(8,OFF);
    SetLedState(9,OFF);
    WriteAllLedOutputs();
    Wait(500);
    SetLedState(1,RED);
    SetLedState(2,RED);
    SetLedState(3,RED);
    SetLedState(4,RED);
    SetLedState(5,RED);
    SetLedState(6,RED);
    SetLedState(7,RED);
    SetLedState(8,RED);
    SetLedState(9,RED);
    WriteAllLedOutputs();
    Wait(500);
    SetLedState(1,OFF);
    SetLedState(2,OFF);
    SetLedState(3,OFF);
    SetLedState(4,OFF);
    SetLedState(5,OFF);
    SetLedState(6,OFF);
    SetLedState(7,OFF);
    SetLedState(8,OFF);
    SetLedState(9,OFF);
    WriteAllLedOutputs();
    Wait(500);
    }
  
    // reset variables
    XO_Initialize();
    break; // break for loop
  
  }
  }
   
   
  /*do                                                       //CPU TTTplayer!!!!!
  {
    if(i<=2)
    {
      int CPUmove=random(4);
      i=(firstMoves[CPUMove]);
  
      if(ledstates[i]=0)
      {
        delay(1000);
        ledstates[i]=2;
        Serial.Print(firstMoves[CPUMove]);
        Serial.Println();
      }
    }
  }
  */
  
   if (TTTplayerColour == RED) {         //If RED has made a move, then next TTTplayer will be GREEN
          TTTplayerColour = GREEN;
        } else {
          TTTplayerColour = RED;
        }
  
   } // end TTT main for loop
  } // end TTT main while loop


} // end XO_Game



// RISTINOLLA FUNCTIONS END
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
// I/O FUNCTIONS: /////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// rotate a led state between 0, 1, 2 (off, red, green)
void RotateLedState(int led_number) {
  if (ledstates[led_number-1] < 2) {
    ledstates[led_number-1]++;
  } else {
    ledstates[led_number-1] = 0;
  }
}
///////////////////////////////////////////////////////////////
// set a led state. led_number (1-9), state (0/OFF, 1/RED, 2/GREEN)
void SetLedState(int led_number, int state) {
  if (state >= 0 && state <= 2) {
    ledstates[led_number-1] = state;
  }
}
///////////////////////////////////////////////////////////////
// write all the led outputs according to their states
void WriteAllLedOutputs() {
  for (int led_index = 0; led_index < 9; led_index++) {
    if (ledstates[led_index] == 0) {
      // set both anode and cathode of the led in led_index to LOW, turning off the led
      digitalWrite(leds[led_index][0], LOW);
      digitalWrite(leds[led_index][1], LOW);
    } else if (ledstates[led_index] == 1) {
      // set the anode to HIGH, cathode to LOW of the led in led_index, lighting the led red
      digitalWrite(leds[led_index][0], HIGH);
      digitalWrite(leds[led_index][1], LOW);
    } else if (ledstates[led_index] == 2) {
      // set the anode to LOW, cathode to HIGH of the led in led_index, lighting the led green
      digitalWrite(leds[led_index][0], LOW);
      digitalWrite(leds[led_index][1], HIGH);
    }
  }
}
///////////////////////////////////////////////////////////////
// read input voltage value, return a corresponding button number as integer
int GetButtonID(int inputValue){
  for (int button = 1; button <= 10; button++) {
    if (inputValue > buttonValues[button-1] - THRESHOLD && inputValue < buttonValues[button-1] + THRESHOLD)
    {
      return button;
    }
  }
  return -1; // if no valid result (ie. if two or more buttons we're pressed simultaneously)
}
///////////////////////////////////////////////////////////////
// handles button input. updates 'activeButton', 'keyUp' & 'keyDown'. accepts one button press at a time (acceptInput) 
void GetButtonInput() {
  int buttonValue = analogRead(BUTTON_INPUT);
  InputDelay(); // buffer delay to get a more stable read value
  buttonValue = analogRead(BUTTON_INPUT);
  InputDelay(); // another buffer in case of continuos reading
  keyUp = 0; // reset keyUp & keyDown
  keyDown = 0; 
  // read raw input value
  if (acceptInput && buttonValue > 0)
  {
    activeButton = GetButtonID(buttonValue); // convert the input value to it's corresponding button number and store it to 'activeButton'
    keyDown = activeButton; // store latest button press for this loop iteration
    acceptInput = 0; // stop accepting button presses while another button is already being pressed and read
    // if a button press is no longer detected: 
  } else if (!acceptInput && buttonValue <= 0) {
    keyUp = activeButton; // store latest button release for this loop iteration
    activeButton = 0; // no button is active 
    acceptInput = 1; // start accepting button presses again
  }
  // check for reset button:
  if(activeButton == 10) {
    resetTimer += 2; // increment by 2 ms
  } else {
    resetTimer = 0;
  }
  // trigger reset if button is held down for 1 sec
  if(resetTimer > 1000) {
    selectedGame = 0;
    resetTimer = 0;
    reset = true;
  }
  
}


// a small ~1 ms delay for buffering button input read
void InputDelay() {
  noInterrupts();
  delayTime = 0; // this is being incremented by ISR
  interrupts();
  while(1) {
    if (delayTime >= 1) {
      break;
    }
  }
}
// wait while reading button input, interruptable by reset button long press only
void Wait(unsigned long millisecondsToWait) {
  noInterrupts();
  waitTime = 0; // this is being incremented by ISR
  interrupts();
  while(waitTime < millisecondsToWait*2 && !reset) {
    GetButtonInput();
    if (millisecondsToWait < 1) {
      break;
    }
    millisecondsToWait--; // subtract the input buffer delay time spent in GetButtonInput
  }
}


// wait while reading button input, interruptable by any button press
void WaitInterruptable(unsigned long millisecondsToWait) {
  noInterrupts();
  waitTime = 0; // this is being incremented by ISR
  interrupts();
  while(waitTime < millisecondsToWait*2 && !reset) {
    GetButtonInput();
    if (millisecondsToWait < 1) {
      break;
    }
    millisecondsToWait--; // subtract the input buffer delay time spent in GetButtonInput
    if (activeButton != 0) {
      break;
    }
  }
}
///////////////////////////////////////////////////////////////
// TIMER INTERRUPT ROUTINE
ISR(TIMER1_COMPA_vect) {
  // incrementing on a 1000 Hz rate (1 ms)
  waitTime++;
  delayTime++;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
