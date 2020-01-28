// Rahmanta Nadhif Satriana
// 30044533
// CPSC 599.88: Tangible HCI Assignment 1
// After SOS

//GPIO PIN Numbers
const int mainButton = 2;
const int noisy = 8;
const int whiteLED = 9;
const int blueLED = 10;
const int yellowLED = 11;
const int redLED =  13;

//Reference for morse code and the alphabet
const String morse[] = {".-", "-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-",
                        "..-","...-",".--","-..-","-.--","--.."};
const String alphabet[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

//Set variables needed
int mainState = 0;    //state of button (pressed or unpressed)   
int prevMainState = 0;
int pressStartTime = 0; //start of button press
int pressEndTime = 0; // end of button press
int pressTime = 0; // end - start
int morseStartTime = 0; //start of checking morse code
int morseEndTime = 0; //end of checking morse code
int counter = 0; //the times it goes through the while loop, for flashing lights
  
String morseMessage = "";
String realMessage = "";
String currMode = "";

void setup() {
  Serial.begin(9600); //for debugging
  // initialize the electric things as output/input:
  pinMode(whiteLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(noisy, OUTPUT);
  pinMode(mainButton, INPUT);
}


//All serial.println was for testing and debugging

void loop() {
  // read the state of the pushbutton value:
  mainState = digitalRead(mainButton);
  morseEndTime = millis();

  //Check if there is a long delay in morse code, meaning one letter is registered
  if ((morseEndTime - morseStartTime) > 2000 && morseStartTime != 0) {
    morseStartTime = 0;
    bool found = false;
    // go through array to find the code
    for (int x = 0; x < 26; x++) {
      if (morseMessage == morse[x]) {
        found = true;
        realMessage += alphabet[x];
        Serial.println(realMessage);
        break;
      }
    }
    morseMessage = "";
    if (found) {
      //if there is 3 codes, then change the mode of the lights
      if (realMessage.length() == 3) {
        currMode = realMessage;
        realMessage = "";
        changeMode();
      }
    }
    //if message not found, reset everything
    else {
      realMessage = "";
      currMode = "";
      changeMode();
    }
  }

  

  //to keep the lights constantly flashing while still having system active
  if (currMode == "RAV") {
    if (counter == 1000) {
      digitalWrite(redLED, HIGH);
      digitalWrite(yellowLED, LOW);
      digitalWrite(blueLED, LOW);
      digitalWrite(whiteLED, LOW);
    }
    else if (counter == 4000) {
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, HIGH);
      digitalWrite(blueLED, LOW);
      digitalWrite(whiteLED, LOW);
    }
    else if (counter == 7000) {
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(blueLED, LOW);
      digitalWrite(whiteLED, LOW);
    }
    else if (counter == 10000) {
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(blueLED, HIGH);
      digitalWrite(whiteLED, LOW);
    }
    else if (counter == 13000) {
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(blueLED, LOW);
      digitalWrite(whiteLED, HIGH);
      counter = 0;
    }
    
  }

  else if (currMode == "BAD") {
    if (counter == 5000) {
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(blueLED, LOW);
      digitalWrite(whiteLED, LOW);
    }

    else if (counter == 10000) {
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(blueLED, LOW);
      digitalWrite(whiteLED, HIGH);
      counter = 0;
    }
    
  }
 
  //if there is a change in state...
  if (mainState != prevMainState) {
    if (mainState == HIGH) {
      pressStartTime = millis();
      tone(noisy, 600); //make noise on piezo to signify button pressing
    }

    else {
      pressEndTime = millis();
      noTone(noisy); //stop the sound
      pressTime = pressEndTime - pressStartTime;
  
      Serial.println(pressTime);  
      if (pressTime > 50) {
        //long press
        if (pressTime > 250) {
          morseMessage += "-";
        }
        else {
          morseMessage += ".";
        }
        Serial.println(morseMessage);
      }

      morseStartTime = millis();
    }
  }
  
  

  prevMainState = mainState;
  counter ++;
}

//codes for changing modes
void changeMode() {
  if (currMode == "WAT") {
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(blueLED, LOW);
    digitalWrite(whiteLED, LOW);
  }

  else if (currMode == "GOO") {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(blueLED, HIGH);
    digitalWrite(whiteLED, LOW);
  }

  else if (currMode == "COM") {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(blueLED, LOW);
    digitalWrite(whiteLED, LOW);
  }

  else if (currMode == "HOW" || currMode == "BAD") {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(blueLED, LOW);
    digitalWrite(whiteLED, HIGH);
  }

  else if (currMode == "RAV") {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(blueLED, LOW);
    digitalWrite(whiteLED, LOW);
  }

  else {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(blueLED, LOW);
    digitalWrite(whiteLED, LOW);
  }
}
