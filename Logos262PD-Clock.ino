// (c) by Paolo CRAVERO IK1ZYW 2016. All rights reserved.
//
// No responsibility is taken for any use of this code,
// which is provided as an academic guidance to fellow builders.
// -------------------------------------------------------------

#include <Wire.h>
#include <Button.h>


// **** CONFIGURATION ****

// **** DEFINITIONS ****
#define BACKSPACE 10
#define DECPOINT 12
#define SUM 11
#define CECA 13
#define TOTI 14

int outputs[6] = {5,4,3,7,8,9}; // C, B, A, C', B', A' outputs

int latch = 6; // latches LSD to MSD

//int oneSecondInterruptPin = 2; // pin for timekeeping, D2
#define oneSecondInterruptPin 2
#define bypassPin A0 // logical level 1, do clock; logical level 0, do calculator


#define setHoursPin A1 // pull to ground to set hours
#define setMinutesPin A2 // pull to ground to set minutes

Button button1(setHoursPin, PULLUP); // Connect your button between pin 2 and GND
Button button2(setMinutesPin, PULLUP); // Connect your button between pin 3 and GND

int secondElapsed = 0;
int oldSecondElapsed = 0;

int mode = 0;

int keyPressDuration = 50; // how long to keep the key pressed in milliseconds
int keyInterkeyDelay = 0; // for future use, how long to way between each keypress, for some visual effect; it sums up with keyPressDuration; in milliseconds
int interBlockPause = 300; // visual effect of pausing when typing informational blocks

//unsigned long currentTime;
//unsigned long displayedTime = 100000;
//unsigned long displayedDate = 20160914;

byte emuKey[16][6] ={ // LSB to MSB
{1,0,0,0,0,1}, // 0
{0,1,1,0,1,0}, // 1
{0,1,1,0,0,1}, // 2
{1,0,1,0,1,0}, // 3
{1,0,0,0,0,0}, // 4
{0,1,1,0,0,0}, // 5
{1,0,1,0,0,0}, // 6 
{1,0,0,0,1,1}, // 7
{0,1,1,0,1,1}, // 8
{1,0,1,0,1,1}, // 9
{0,0,1,0,0,1}, // 10 backspace
{0,0,0,0,1,1}, // 11 sum
{1,0,0,0,1,0}, // 12 decimal point
{1,1,1,1,0,0}, // 13 CE/CA
{0,0,0,0,1,0}, // 14 Totalizer 1
{0,1,1,0,1,1}  // 15 multiply
}; 


void setup() {

//  Serial.begin(9600);
  Wire.begin();

  //pinMode(sensorPin, INPUT_PULLUP);
//  pinMode(setHoursPin, INPUT_PULLUP);
//  pinMode(setMinutesPin, INPUT_PULLUP);
  pinMode(oneSecondInterruptPin, INPUT); // DS3231 module should already include pullups
  pinMode(bypassPin, INPUT_PULLUP); // clock function bypass pin
    
  for(int a = 0; a < 6; a++){pinMode(outputs[a], OUTPUT);} //set outputs
  pinMode(latch, OUTPUT); //set outputs
  digitalWrite(latch, HIGH); // 
//
//  if ( button1.isPressed() ) {
//    //attivo la comunicazione con il DS1307
//    //l'indirizzo dell'RTC è 0x68
//    Wire.beginTransmission(0x68);
//    //il primo byte stabilisce il registro
//    //iniziale da scivere
//    Wire.write((byte)0x00);
//    //specifico il tempo e la data
//    Wire.write((byte)0x00); //1° byte SECONDI da 0x00 a 0x59
//    Wire.write((byte)0x53); //2° byte MINUTI da 0x00 a 0x59
//    Wire.write((byte)0x80 | 0x23); //3° byte ORE da 0x00 a 0x24
//    Wire.write((byte)0x05); //4° byte GIORNO della settimana da 0x01 a 0x07
//    Wire.write((byte)0x16); //5° byte GIORNO del mese da 0x00 a 0x31
//    Wire.write((byte)0x09); //6° byte MESE da 0x00 a 0x12
//    Wire.write((byte)0x16); //7° byte ANNO 0x00 a 0x99
//    Wire.endTransmission();
//  
//    delay(50);
//  
//    // set square wave output at 1 Hz
    Wire.beginTransmission(0x68);
    Wire.write((byte)0x0E);
    Wire.write((byte)0x00); // control byte to set and enable 1 Hz SQW output &B0000_0000
    Wire.endTransmission();  // 
//  }


  delay(3000);

  // start interrupt at the end of startup sequence
  oldSecondElapsed = secondElapsed = 58;
  attachInterrupt(digitalPinToInterrupt(oneSecondInterruptPin), oneSecondISR, FALLING);  

}

int decToBcd(int val)
{
  return ( ((val/10)*16) + (val%10) );
}

int bcdToDec(int val)
{
  return ( val / 16 * 10 + val % 16 );
}

// increase a BCD value converting it to decimal and back
byte increaseBCD ( byte myBCD, int lowLimit, int highLimit ) {
  int myDecimal = bcdToDec( myBCD );  // get the decimal value

  myDecimal++;

  if (myDecimal > highLimit) {
    return decToBcd(lowLimit);
  } else {
    return decToBcd(myDecimal);
  }
}

// print as many backspaces as needed
void backspace(int howManyBackspaces) {
  for (int i=0; i<howManyBackspaces; i++) {
    printKey(BACKSPACE); // 10 is the position in emuKey table for the backspace character
  }
}

void printKey(int myBCD) {

  // choose right MUX line
  digitalWrite(outputs[0], emuKey[myBCD][0]);
  digitalWrite(outputs[1], emuKey[myBCD][1]);
  digitalWrite(outputs[2], emuKey[myBCD][2]);
  digitalWrite(outputs[3], emuKey[myBCD][3]);
  digitalWrite(outputs[4], emuKey[myBCD][4]);
  digitalWrite(outputs[5], emuKey[myBCD][5]);

  // "press" the key
  digitalWrite(latch, LOW); // connect together row and column
  delay(keyPressDuration);
  digitalWrite(latch, HIGH); // 
  delay(keyPressDuration); // this delay is needed as a guard time before next keypress
}


// BCD is a two digit number in BCD format
void printBCD(int myBCD) {
  int myDigitHigh;
  int myDigitLow;
 
  // get lower digit
  myDigitLow = myBCD & 0x0F;

  // get higher digit
  myDigitHigh = myBCD >> 4;
  
  printKey(myDigitHigh); // print higher digit
  printKey(myDigitLow); // print lower digit

}


void oneSecondISR() {
  oldSecondElapsed = secondElapsed;
  secondElapsed = secondElapsed + 1;
  }

void loop() {

  int digit;
  int j;
  static boolean firstPass = 1;
  static byte seconds;
  static byte minutes = 0x00;
  static byte hours;
  static byte giorno_sett;
  static byte month_day;
  static byte month_nr;
  static byte year_nr;
  static byte new_minutes;
  static byte new_hours;
  static byte new_month_day;
  static byte new_month_nr;
  static byte new_year_nr;

  static byte control;
  static byte inSetMode; // are we setting the time? any value > 0 defines what we are setting

  if ( digitalRead(bypassPin) == 1 ) { // no magnet on the bypass switch/sensor, pull up resistor kicks in, do the clock stuff
 
  // for 59 seconds show the time
  if (secondElapsed < 59) {
    if ( secondElapsed > oldSecondElapsed ) { // has one second passed?
        oldSecondElapsed = secondElapsed;
        if (firstPass == 1) {
          switch (mode) {
            case 0:
              // display yyyymmdd.hhmm
              printBCD(0x20); // year, we're good until 2099 :)
              printBCD(year_nr);
              delay(interBlockPause);
              printBCD(month_nr);
              delay(interBlockPause);
              printBCD(month_day);
              delay(interBlockPause);
              printKey(DECPOINT);
              delay(interBlockPause);
              printBCD(hours);
              delay(interBlockPause);
              printBCD(minutes);
              delay(interBlockPause);
              break;
    
            case 1:
              printBCD(hours);
              delay(interBlockPause);
              printBCD(minutes);
              delay(interBlockPause);
              printKey(DECPOINT);
              delay(interBlockPause);
              printBCD(month_day);
              delay(interBlockPause);
              printBCD(month_nr);
              delay(interBlockPause);
              printBCD(0x20); // year, we're good until 2099 :)
              printBCD(year_nr);
              break;
              
            case 2:
              printBCD(hours);
              delay(interBlockPause);
              printKey(DECPOINT);
              delay(interBlockPause);
              printBCD(minutes);
              break;
              
            case 3:
              printBCD(year_nr);
              printBCD(month_nr);
              printBCD(month_day);
              printKey(DECPOINT);
              printBCD(hours);
              printBCD(minutes);
              printBCD(seconds);              
              printKey(SUM);
              printKey(0);
              printKey(DECPOINT);
              printKey(0);
              printKey(0);
              printKey(0);
              printKey(0);
              printKey(0);                                                  
              printKey(1);
              printKey(SUM);
              break;
                
            case 4:
              printBCD(hours);
              printBCD(minutes);
              printBCD(seconds);              
              printKey(SUM);
              printKey(1);
              printKey(SUM);
              break;
              
            case 5:
              printBCD(hours);
              printBCD(minutes);
              printBCD(seconds);              
              printKey(DECPOINT);
              printBCD(month_day);
              printBCD(month_nr);
              printBCD(year_nr);
              printKey(SUM);
              printKey(1);
              printKey(SUM);
              break;
    
          }
    
    
          firstPass = 0;
        } else {
          switch (mode) {
            case 0: break; // nothing to do
            case 1: break; // nothing to do
            case 2: break; // nothing to do
            case 3:
              printKey(SUM);
              break;
              
            case 4:
              printKey(SUM);
              break;
              
            case 5:
              printKey(SUM);
              break;
            
          }
        }
    } // end if one second has passed
  } // end if seconds < 59

  // clear the screen
  if (secondElapsed == 59) {
    switch (mode) {
      case 0:
        backspace(13);
        break;
      case 1:
        backspace(13);
        break;
      case 2:
        backspace(5);
        break;
      default:
        printKey(TOTI);
        printKey(TOTI);
        break;
    } // end switch
  } // end if seconds == 58

  // prepare for the next round
  if (secondElapsed > 59) {

      delay(1000);
      
      secondElapsed = 0; // reset the ISR

      //richiedo 7 byte dal dispositivo con
      //indirizzo 0x68
      Wire.beginTransmission(0x68);   // Initialize the Tx buffer
      Wire.write(0x00);            // Put slave register address in Tx buffer
      Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive

      //  Wire.requestFrom(0x68, 7, 1); // just date and time
      Wire.requestFrom(0x68, 7); // time, date and control byte
      //recupero i 7 byte relativi ai
      //corrispondenti registri
      seconds = Wire.read();
      minutes = Wire.read();
      hours = Wire.read();
      giorno_sett = Wire.read();
      month_day = Wire.read();
      month_nr = Wire.read();
      year_nr = Wire.read();
      //control = Wire.read();

      firstPass = 1;

      secondElapsed = seconds; // reinit interrupt variable

      mode = bcdToDec(minutes) % 6;

//    Serial.print(month_day, HEX);
//    Serial.print("/");
//    Serial.print(month_nr, HEX);
//    Serial.print("/");
//    Serial.print(year_nr, HEX);
//        Serial.print(" ");
//    Serial.print(hours, HEX);
//        Serial.print(":");
//    Serial.print(minutes, HEX);
//        Serial.print(":");
//    Serial.println(seconds, HEX);


    } // end if minute has elapsed

  } // end if control the bypass pin

} 


