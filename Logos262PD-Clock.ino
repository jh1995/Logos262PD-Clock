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

int outputs[6] = {5,4,3,7,8,9}; // C, B, A, C', B', A' outputs

int latch = 6; // latches LSD to MSD

//int oneSecondInterruptPin = 2; // pin for timekeeping, D2
#define oneSecondInterruptPin 2


#define setHoursPin A0 // pull to ground to set hours
#define setMinutesPin A1 // pull to ground to set minutes

Button button1(setHoursPin, PULLUP); // Connect your button between pin 2 and GND
Button button2(setMinutesPin, PULLUP); // Connect your button between pin 3 and GND

//bool blinker = 0;  // blinking control variable
//bool dpBlink = 0; // decimal point blinking control; set to 1 to make decimal points blink
//bool digitBlink = 0; // digit blinking control; set to 1 to make digits blink
//bool allOff = 2; // used for PWM brightness control

bool secondElapsed = 0;

int keyPressDuration = 50; // how long to keep the key pressed in milliseconds
int keyInterkeyDelay = 0; // for future use, how long to way between each keypress, for some visual effect; it sums up with keyPressDuration; in milliseconds
int interBlockPause = 300; // visual effect of pausing when typing informational blocks

unsigned long displayedTime = 100000;
unsigned long displayedDate = 20160914;
// sample values

//unsigned int intensity = 0; // PWM-like intensity control
//int intensitySteps = 10; // how many intensity levels we can do, 1 to 255
//int sensorValue = 0;  // variable to store the value coming from the sensor

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
{0,0,0,0,1,1}, // 14 substract
{0,1,1,0,1,1}  // 15 multiply
}; 


void setup() {

  //Serial.begin(9600);
//  Wire.begin();

  //pinMode(sensorPin, INPUT_PULLUP);
//  pinMode(setHoursPin, INPUT_PULLUP);
//  pinMode(setMinutesPin, INPUT_PULLUP);
  pinMode(oneSecondInterruptPin, INPUT_PULLUP); // DS1307 square wave output is open-drain.
    
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
//    Wire.write((byte)0x00); //2° byte MINUTI da 0x00 a 0x59
//    Wire.write((byte)0x80 | 0x08); //3° byte ORE da 0x00 a 0x24
//    Wire.write((byte)0x02); //4° byte GIORNO della settimana da 0x01 a 0x07
//    Wire.write((byte)0x06); //5° byte GIORNO del mese da 0x00 a 0x31
//    Wire.write((byte)0x09); //6° byte MESE da 0x00 a 0x12
//    Wire.write((byte)0x16); //7° byte ANNO 0x00 a 0x99
//    Wire.endTransmission();
//  
//    delay(50);
//  
//    // set square wave output at 1 Hz
//    Wire.beginTransmission(0x68);
//    Wire.write((byte)0x07);
//    Wire.write((byte)0x10); // control byte to set and enable 1 Hz SQW output
//    Wire.endTransmission();
//  }



  // start interrupt at the end of startup sequence
  attachInterrupt(digitalPinToInterrupt(oneSecondInterruptPin), oneSecondISR, FALLING);  

  delay(6000);
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
  secondElapsed = 1;
}

void loop() {

  int digit;
  int j;
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


  year_nr = 0x16;
  month_nr = 0x09;
  month_day = 0x14;
  hours = 0x23;
  //minutes = 0x00;
 
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
  
  
  delay(1000);
  delay(2000);


  minutes = increaseBCD(minutes,0, 59);
  
  backspace(2);

  printBCD(minutes);
  delay(interBlockPause);

  delay(1000);
  delay(2000);

  minutes = increaseBCD(minutes,0, 59);
  backspace(13); // eat it all up, the decimal point counts as well!
  
//
//  // things to do every second.
//  if (secondElapsed == 1) {
//    
//    secondElapsed = 0; // reset the ISR
//
//    // read the amount of incoming light to control display intensity via PWM
//    // LDR value increases with darkness, so the ADC output is closer to ground
//    // at maximum light intensity and highest in complete darkness.
//    // v20160603: - just do it once a second, not at every loop to speed up PWM
//    //            - higher brightness decreased. Previous values: 12, 8, 6, 3, 2, 1.
//    sensorValue = analogRead(sensorPin);
//    if (sensorValue > 700) {
//      intensitySteps = 12;
//    } else if (sensorValue > 600) {
//      intensitySteps = 8;
//    } else if (sensorValue > 450) {
//      intensitySteps = 6;    
//    } else if (sensorValue > 300) {
//      intensitySteps = 4;
//    } else if (sensorValue > 100) {
//      intensitySteps = 3;
//    } else {
//      intensitySteps = 2;
//    }
//
//
//    
//    //  ** UPDATE THE CLOCK VARIABLES
//    Wire.beginTransmission(0x68);
//    Wire.write((byte)0x00);
//    Wire.endTransmission();
//
//    if ( inSetMode == 0 ) { // if we are operating normally, read an update, otherwise don't mess with values
//      //richiedo 7 byte dal dispositivo con
//      //indirizzo 0x68
//    //  Wire.requestFrom(0x68, 7, 1); // just date and time
//      Wire.requestFrom(0x68, 7); // time, date and control byte
//      //recupero i 7 byte relativi ai
//      //corrispondenti registri
//      seconds = Wire.read();
//      minutes = Wire.read();
//      hours = Wire.read();
//      giorno_sett = Wire.read();
//      month_day = Wire.read();
//      month_nr = Wire.read();
//      year_nr = Wire.read();
//      control = Wire.read();
//    } else {
//      Wire.requestFrom(0x68, 1); // read just seconds fr blinking
//      // maybe not the best way for blinking, should take advantage of the interrput *****
//      seconds = Wire.read();
//    }
//
////    Serial.print(month_day, HEX);
////    Serial.print("/");
////    Serial.print(month_nr, HEX);
////    Serial.print("/");
////    Serial.print(year_nr, HEX);
////        Serial.print(" ");
////    Serial.print(hours, HEX);
////        Serial.print(":");
////    Serial.print(minutes, HEX);
////        Serial.print(":");
////    Serial.println(seconds, HEX);
// 
//    // blinking control
//    blinker = seconds % 2; // blink stuff once a second (0.5 Hz)
//  } // end if secondelapsed
//  
//    // TODO: define what to print on the display:
//    // HH:MM
//    // moving seconds ... randomised, 5 positions
//    // DD.MM
//    // YYYY
//
//
//    // are we setting date/time and so on?
//    if ( inSetMode > 0 ) {
//      
//      switch (inSetMode) {
//        case 1:
//          delay(100);
//          new_hours = hours;
//          new_minutes = minutes;
//          new_month_day = month_day;
//          new_month_nr = month_nr;
//          new_year_nr = year_nr;
//          inSetMode = 2;
//        case 2:
//          lowDigit = 0xDD;
//          highDigit = new_hours;
//          mainDP0 = 1;
//          mainDP1 = 1;
//          mainDP2 = 0;
//          mainDP3 = 0;
//          break;
//        case 3:
//          lowDigit = new_minutes;
//          highDigit = 0xDD;
//          mainDP0 = 0;
//          mainDP1 = 0;
//          mainDP2 = 1;
//          mainDP3 = 1;
//          break;
//        case 4: // update month first, so that we can count the right number of days in the month. Leap years not counted.
//          lowDigit = new_month_nr;
//          highDigit = 0xDD;
//          mainDP0 = 0;
//          mainDP1 = 1;
//          mainDP2 = 1;
//          mainDP3 = 1;
//          break;
//        case 5:
//          lowDigit = 0xDD;
//          highDigit = new_month_day;
//          mainDP0 = 1;
//          mainDP1 = 1;
//          mainDP2 = 1;
//          mainDP3 = 0;
//          break;
//        case 6:
//          lowDigit = new_year_nr;
//          highDigit = 0x20;
//          mainDP0 = 0;
//          mainDP1 = 0;
//          mainDP2 = 0;
//          mainDP3 = 0;
//          break;
//        default:  // just in case, exit set mode
//          inSetMode = 0;
//          digitBlink = 0;
//      } // end switch
//
//      if ( button1.uniquePress() ) {
//        inSetMode++;
//        delay(100);        
//        if ( inSetMode > 6 ) { // we're done with setting
//          inSetMode = 0;
//          digitBlink = 0;
//          
//          //attivo la comunicazione con il DS1307
//          //l'indirizzo dell'RTC è 0x68
//          Wire.beginTransmission(0x68);
//          //il primo byte stabilisce il registro
//          //iniziale da scivere
//          Wire.write((byte)0x00);
//          //specifico il tempo e la data
//          Wire.write((byte)0x00); //1° byte SECONDI da 0x00 a 0x59
//          Wire.write(new_minutes); //2° byte MINUTI da 0x00 a 0x59
//          Wire.write((byte)0x80 | new_hours); //3° byte ORE da 0x00 a 0x24
//          Wire.write((byte)0x02); //4° byte GIORNO della settimana da 0x01 a 0x07
//          Wire.write(new_month_day); //5° byte GIORNO del mese da 0x00 a 0x31
//          Wire.write(new_month_nr); //6° byte MESE da 0x00 a 0x12
//          Wire.write(new_year_nr); //7° byte ANNO 0x00 a 0x99
//          Wire.endTransmission();
//            
//        }
//
//      } // end if button1 is pressed
//
//      // let's update some digit!!
//      if ( button2.uniquePress() ) {
//        switch (inSetMode) {
//          case 1: break;
//          case 2: new_hours = increaseBCD ( new_hours, 0, 23 ); break;
//          case 3: new_minutes = increaseBCD ( new_minutes, 0, 59 ); break;
//          case 4: new_month_nr = increaseBCD ( new_month_nr, 1, 12 ); break;
//          case 5: new_month_day = increaseBCD ( new_month_day, 1, 31 ); break;
//          case 6: new_year_nr = increaseBCD ( new_year_nr, 16, 50 ); break; // up to 2050 A.D.
//        }
//        delay(100);
//        
//      }
//
//      // not in SET mode
//    } else {
//
//	// **** TODO TOTEST **** add here the drift correction routine that adds 2 seconds every day at 3 AM
//		if ( (hours == 3) && (minutes == 0) && (seconds == 0) ) { // is it 3:00:00 AM?
//          //attivo la comunicazione con il DS1307
//          //l'indirizzo dell'RTC è 0x68
//          Wire.beginTransmission(0x68);
//          //il primo byte stabilisce il registro
//          //iniziale da scivere
//          Wire.write((byte)0x00);
//          //specifico il tempo e la data
//          Wire.write((byte)0x02); //1° byte SECONDI da 0x00 a 0x59 -- skipping ahead of two seconds
//          Wire.write((byte)0x00); //2° byte MINUTI da 0x00 a 0x59
//          Wire.write((byte)0x80 | hours); //3° byte ORE da 0x00 a 0x24
//          Wire.write((byte)0x02); //4° byte GIORNO della settimana da 0x01 a 0x07
//          Wire.write(month_day); //5° byte GIORNO del mese da 0x00 a 0x31
//          Wire.write(month_nr); //6° byte MESE da 0x00 a 0x12
//          Wire.write(year_nr); //7° byte ANNO 0x00 a 0x99
//          Wire.endTransmission();
//		}
//
//	    new_year_nr = 0x03 & seconds;
//      new_year_nr = new_year_nr % 4; // recycling a variable
//      lowDigit = minutes;
//      highDigit = hours;
//  
//  
//      // enter SET mode
//      if (button1.isPressed()) {
//  //      digitBlink = 1; // blink digits when setting
//        inSetMode = 1;
//        lowDigit = 0xDD;
//        highDigit = 0xDD;
//      }
//      // show D.DM.M 
//      if (button2.isPressed()) {
//        lowDigit = month_nr;
//        highDigit = month_day;
//        mainDP0 = 0;
//        mainDP1 = 1;
//        mainDP2 = 0;
//        mainDP3 = 1;
//      }
//      
//    } // end if/else inSetMode
//

} 

