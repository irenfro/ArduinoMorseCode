#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned long t1; //used to measure the time
unsigned long t2; //used to measure the time
unsigned long len; //difference between t2 and t1
//AlphaNumeric Characters in Morse
String morseAlpha[] = {".-", "-...", "-.-.", "-..", ".", 
                             "..-.", "--.", "....", "..", ".---", 
                             "-.-", ".-..", "--", "-.", "---", ".--.",
                             "--.-", ".-.", "...", "-", "..-", "...-", 
                             ".--", "-..-", "-.--", "--..", "E"};
String morseNumeric[] = {"-----", ".----", "..---", "...--", "....-", ".....",
                          "-....", "--...", "---..", "----.", "E"};
int flag = 0;
int flag1 = 0;
int inPin = 7;
String inp = "";
String out = "";

void setup() {
  //init the display
  lcd.begin(16, 2);
  //init the button
  pinMode(inPin, INPUT_PULLUP);
}

void loop() {
  // wait untill there is two changes in the input
  while(digitalRead(inPin) == HIGH) {/*wait*/}
  t1 = millis();
  while(digitalRead(inPin) == LOW) {/*wait*/}
  t2 = millis();
  //measurse the difference between the clicks
  len = t2 - t1;
  if(len > 50) {
    //the switch is not perfect and will bounce 
    //so we need an epsilon to make an accrurate reading
    if(len > 5000) {
      //Clear the screen and reset the values
      lcd.clear();
      out = "";
      inp = "";
    } else {
      //Translate the signal to either a . or a -
      inp += sigToAlpha();
      //print out the input to the lcd
      printLCDTop(inp);
    }   
  }
  //wait for more letters
  while((millis() - t2) < 500) {
     if(digitalRead(inPin) == LOW) {
        flag = 1;
        break;
     } 
  }
  //if no letters were found then display the alphanumeric 
  //that was entered in morse
  if(!flag) {
    lcdClearTop(); 
    convert(); 
  }
  flag = 0;
}

char sigToAlpha() {
  if(len < 600 && len > 50) {
      return '.'; 
  }
  return '-';
}

void convert() {
    //if it is the escape sequence for space or period
    if(inp == ".-.-.-"){
      out += '.';
      printLCDBottom();
      return;
    } else if(inp == "----") {
      out += " ";
      printLCDBottom();
      return;
    }
    //iterate over the alpha and numeric lists 
    //to see if the letter is in either and display the new string.
    int i = 0;
    while(morseAlpha[i] != "E") {
      if(morseAlpha[i] == inp) {
         out += char('A' + i);
         printLCDBottom();
         inp = "";
         return;
      }
      i++;
    }
    i = 0;
    while(morseNumeric[i] != "E") {
       if(morseNumeric[i] == inp) {
         out += char('0' + i);
         printLCDBottom();
         inp = "";
         return;
       }
       i++;
    }
}

//print a string to the top row
void printLCDTop(String x) {
   lcd.setCursor(0, 0);
   lcd.print(x);
}

//print a string to the bottom row
void printLCDBottom() {
  lcd.setCursor(0, 1);
  lcd.print(out);
}

//clear just the top row
void lcdClearTop() {
   lcd.home();
   for(int i = 0; i < 16; i++) {
      lcd.print(" "); 
   }
   lcd.home();
}

