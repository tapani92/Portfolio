/*
 * KNOWN ISSUES:
 * - can only press one button at a time for correct functionality
 * 
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  /*Serial.begin(9600); // for debugging
  */
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
  
  
  

  // EXAMPLES:

  /*
  // buttons 1-9 cycle thru individual leds' states on key up:
  if (keyUp > 0) {
    RotateLedState(keyUp);
  }
  */
  /*
  // button 10 shuts off all leds on key down:
  if (activeButton == 10) {
    for(int i=0; i < 9; i++) {
      //ledstates[i] = 0; // method 1
      SetLedState(i+1, OFF); // method 2
    }
  }
  */
  /*
  // light a led red when it's button is held down, green otherwise
  for (int i=1; i < 10; i++) {
    if (activeButton == i) {
      SetLedState(i, RED);
    } else {
      SetLedState(i, GREEN);
    }
  }
  */

  
  

  /*
  // debug text for led output states:
  Serial.print("\t\t");
  Serial.print("led states: ");
  for (int i=0; i < 9; i++) {
    Serial.print("led");
    Serial.print(i+1);
    Serial.print(" ");
    Serial.print(ledstates[i]);
    Serial.print(" ");
  }
  */

  XO_Game();
  
  //Serial.println(""); // debug text line change
  
  //LedSequence(); // to calibrate led pin assignments
/*  ButtonTest(); // for testing buttons and leds
  WriteAllLedOutputs(); // updates all leds states on the hardware
  delay(1);*/
} // void (main) loop end 



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
void GetButtonInput(int buttonValue) {
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
  /*
  // debug text for button input:
  Serial.print("value: ");
  Serial.print(buttonValue);
  Serial.print("\t");
  Serial.print("activeButton: ");
  Serial.print(activeButton);
  Serial.print("\t");
  Serial.print("keyUp: ");
  Serial.print(keyUp);
  Serial.print("\t");
  Serial.print("keyDown: ");
  Serial.print(keyDown);
  Serial.print("\t");
  Serial.print("acceptInput: ");
  Serial.println(acceptInput);
  
}
///////////////////////////////////////////////////////////////
/* light all leds from 1 to 9 in sequece
void LedSequence() {
  // turn off all leds first
  for (int i=1; i<10; i++) {
    SetLedState(i, OFF);
  }
  // burn a led from 1 to 9 green for 5 sec, one by one
  for (int i=1; i<10; i++) {
    SetLedState(i, GREEN);
    WriteAllLedOutputs();
    delay(50);
    SetLedState(i, OFF);
  }
  // burn a led from 1 to 9 red for 5 sec, one by one
  for (int i=1; i<10; i++) {
    SetLedState(i, RED);
    WriteAllLedOutputs();
    delay(50);
    SetLedState(i, OFF);
  }
} 
// turns a led red/green/off when a corresponding button is pressed. for testing
/void ButtonTest() {
  if (keyDown > 0) {
    RotateLedState(keyDown);
  }
}*/

}

  ////////////////////////////
 //!!!!TIC-TAC-TOE GAME!!!!//
///////////////////////////
void XO_Game ()
{

  int TTTwinner;
  int TTTplayer=0;
  int TTTplayerColour=RED;
  int board[9]={0,0,0,0,0,0,0,0,0};

while(1) {

  
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
  delay(800);
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
  delay(500);
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
  delay(800);
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
  delay(500);

  




  

//GAME CONTROL
 for (int i=0; i<9; i++) {

    while(1)
    {
     // read raw button input value and convert it to a button press 
     GetButtonInput(analogRead(BUTTON_INPUT));
    

    if(keyDown > 0 && board[keyDown] !=1 && keyDown !=10)
    {
      delay(20);
      TTTplayer=activeButton;
      board[keyDown]=1;
      delay(50);
      break;
    } else if (keyDown==10)       //If player push RESET button, the game restarts!
    {
      TTTplayer=0;
      TTTwinner=0;
      
      for(i = 0; i < 9; i++)
      {
      board[i]=0;
      }
      delay(50);
      break;
    }
    
    delay(1);
    }
    
    SetLedState(TTTplayer, TTTplayerColour);
    WriteAllLedOutputs();





  
  
  if(i>=2)                                        //If there´s been made three moves or more, program starts to check every move if there´s three straight lines
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
  delay(500);
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
  delay(500);
  }
    delay(1000);

  while(1)
    {

    if(TTTwinner == RED)   //If RED wins, the game restarts!
    {
      delay(20);
      TTTplayer=0;
      TTTwinner=0;
      
      for(i = 0; i < 9; i++)
      {
      board[i]=0;
      }
      delay(50);
      break;
    }
   
    }
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
  delay(500);
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
  delay(500);
  }
delay(1000);
while(1)
    {
    

    if(TTTwinner == GREEN)      //If GREEN wins, the game restarts!
    {
      delay(20);
      TTTplayer=0;
      TTTwinner=0;
      
      for(i = 0; i < 9; i++)
      {
      board[i]=0;
      }
      delay(50);
      break;
    }
   
    }

} else if(i==8)           //If neither GREEN or RED gets three straight lines, but there´s been made 9 moves-->DRAW!!!
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
  delay(500);
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
  delay(500);
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
  delay(500);
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
  delay(500);
  }

 while(1)
    {
     

    if(i == 8)   //If there´s been made 9 moves without winnings, the game restarts!
    {
      delay(20);
      TTTplayer=0;
      TTTwinner=0;
      
      for(i = 0; i < 9; i++)
      {
      board[i]=0;
      }
      delay(50);
      break;
    }
   
    }
     
    delay(50);

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


 }
}



 


}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
