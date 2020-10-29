
// define button target input values
#define BUTTON_1 55
#define BUTTON_2 131
#define BUTTON_3 225
#define BUTTON_4 337
#define BUTTON_5 443
#define BUTTON_6 511
#define BUTTON_7 611
#define BUTTON_8 764
#define BUTTON_9 938
#define BUTTON_10 1017

#define THRESHOLD 10 // threshold for reading input values

#define BUTTON_INPUT A0 // pin that reads button input

// led output pins. A = anode (long leg), C = cathode
#define LED_1_A 13
#define LED_1_C 12
#define LED_2_A 11
#define LED_2_C 10
#define LED_3_A 9
#define LED_3_C 8
#define LED_4_A 7
#define LED_4_C 6
#define LED_5_A 5
#define LED_5_C 4
#define LED_6_A 3
#define LED_6_C 2
#define LED_7_A 19 // A5 pin
#define LED_7_C 18 // A4 pin
#define LED_8_A 17 // A3 pin
#define LED_8_C 16 // A2 pin
// pins 1 & 0 doesn't work with serial communication. comment/remove all serial stuff from code before plugging these:
#define LED_9_A 1 // TX pin!
#define LED_9_C 0 // RX pin!

// led states as verbal
#define OFF 0
#define RED 1
#define GREEN 2

//int outputValue = 0;
bool acceptInput = 1; // is a new button press being accepted
int activeButton = 0; // the button that is currently being pressed
int keyUp = 0; // last active button press in this loop iteration, for output purposes

// button values in an array for easy accessing
int buttonValues[10] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8, BUTTON_9, BUTTON_10};

// same with led pin numbers
int leds[9][2] = {{LED_1_A, LED_1_C}, {LED_2_A, LED_2_C}, {LED_3_A, LED_3_C}, {LED_4_A, LED_4_C}, {LED_5_A, LED_5_C}, {LED_6_A, LED_6_C},{LED_7_A, LED_7_C},{LED_8_A, LED_8_C},{LED_9_A, LED_9_C}};

// led states variables, initialized as off. array for easy access
int ledstates[9] = {0,0,0,0,0,0,0,0,0};

void setup()
{
  //Serial.begin(9600);
  
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
  pinMode(LED_9_A, OUTPUT); // serial port pin!
  pinMode(LED_9_C, OUTPUT); // serial port pin!
}

void loop()
{
  
  // read button input value and store it to 'activeButton' variable
  GetButtonInput(analogRead(BUTTON_INPUT));
  /*
  Serial.print("value: ");
  Serial.print(buttonRead);
  Serial.print("\t\t");
  Serial.print("button: ");
  */
  
  //Serial.print(activeButton);
  
// ^ INPUT CODE ABOVE ^ 
// -------------------------
// OUTPUT CODE BELOW:

  // buttons 1-9 cycle thru individual leds' states:
  if (keyUp > 0) {
    RotateLedState(keyUp);
  }
  // button 10 shuts off all leds:
  if (activeButton == 10) {
    for(int i=0; i < 9; i++) {
      ledstates[i] = 0;
    }
  }
  WriteAllLedOutputs();
  /*
  Serial.print("\t\t");
  Serial.print("led states: ");
  for (int i=0; i < 9; i++) {
    Serial.print("led");
    Serial.print(i+1);
    Serial.print(" ");
    Serial.print(ledstates[i]);
    Serial.print(" ");
  }
  
  Serial.println("");
  */
  delay(1);
} // void loop end


// rotate a led state between 0, 1, 2 (off, red, green)
void RotateLedState(int led_number) {
  if (ledstates[led_number-1] < 2) {
    ledstates[led_number-1]++;
  } else {
    ledstates[led_number-1] = 0;
  }
}

// set a led state
void SetLedState(int led_number, int state) {
  if (state >= 0 && state <= 2) {
    ledstates[led_number-1] = state;
  }
  
}

// write all the led outputs according to their states
void WriteAllLedOutputs() {
  for (int led_index = 0; led_index < 9; led_index++) {
    if (ledstates[led_index] == 0) {
      // set both anode and cathode of the led in led_index to LOW, turning off the led
      digitalWrite(leds[led_index][0], LOW);
      digitalWrite(leds[led_index][1], LOW);
    } else if (ledstates[led_index] == 1) {
      // set the anode to LOW, cathode to HIGH of the led in led_index, lighting the led red
      digitalWrite(leds[led_index][0], LOW);
      digitalWrite(leds[led_index][1], HIGH);
    } else if (ledstates[led_index] == 2) {
      // set the anode to HIGH, cathode to LOW of the led in led_index, lighting the led green
      digitalWrite(leds[led_index][0], HIGH);
      digitalWrite(leds[led_index][1], LOW);
    }
  }
}

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

// read the first button press only. 
void GetButtonInput(int buttonRead) {
  keyUp = 0; // reset keyUp value
  if (buttonRead > 0 && acceptInput)
  {
    activeButton = GetButtonID(buttonRead); // store the active button press number in 'activeButton'
    acceptInput = 0; // stop accepting button presses while another button is already being pressed and read
  } else if (buttonRead == 0 && !acceptInput) { // if no button press was detected, :
    //Serial.print("keyup ");
    keyUp = activeButton; // store latest button press for this loop iteration
    activeButton = 0; // reset 
    acceptInput = 1; // start accepting button presses again
  }
}
