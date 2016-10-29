// (c) by Paolo CRAVERO IK1ZYW 2016. All rights reserved.
//
// No responsibility is taken for any use of this code,
// which is provided as an academic guidance to fellow builders.
// -------------------------------------------------------------

#include <Wire.h>
#include <Button.h>
#include <EEPROM.h> // used to store DST status


// **** CONFIGURATION ****

// **** DEFINITIONS ****
#define BACKSPACE 10
#define DECPOINT 12
#define SUM 11
#define CECA 13
#define TOTI 14

#define DSTADDRESS 1 // EEPROM position for DST status

int DSTdays[100] = {
0x25, // 2016-03-27 to 2016-10-30
0x14, // 2017-03-26 to 2017-10-29
0x03, // 2018-03-25 to 2018-10-28
0x62, // 2019-03-31 to 2019-10-27
0x40, // 2020-03-29 to 2020-10-25
0x36, // 2021-03-28 to 2021-10-31
0x25, // 2022-03-27 to 2022-10-30
0x14, // 2023-03-26 to 2023-10-29
0x62, // 2024-03-31 to 2024-10-27
0x51, // 2025-03-30 to 2025-10-26
0x40, // 2026-03-29 to 2026-10-25
0x36, // 2027-03-28 to 2027-10-31
0x14, // 2028-03-26 to 2028-10-29
0x03, // 2029-03-25 to 2029-10-28
0x62, // 2030-03-31 to 2030-10-27
0x51, // 2031-03-30 to 2031-10-26
0x36, // 2032-03-28 to 2032-10-31
0x25, // 2033-03-27 to 2033-10-30
0x14, // 2034-03-26 to 2034-10-29
0x03, // 2035-03-25 to 2035-10-28
0x51, // 2036-03-30 to 2036-10-26
0x40, // 2037-03-29 to 2037-10-25
0x36, // 2038-03-28 to 2038-10-31
0x25, // 2039-03-27 to 2039-10-30
0x03, // 2040-03-25 to 2040-10-28
0x62, // 2041-03-31 to 2041-10-27
0x51, // 2042-03-30 to 2042-10-26
0x40, // 2043-03-29 to 2043-10-25
0x25, // 2044-03-27 to 2044-10-30
0x14, // 2045-03-26 to 2045-10-29
0x03, // 2046-03-25 to 2046-10-28
0x62, // 2047-03-31 to 2047-10-27
0x40, // 2048-03-29 to 2048-10-25
0x36, // 2049-03-28 to 2049-10-31
0x25, // 2050-03-27 to 2050-10-30
0x14, // 2051-03-26 to 2051-10-29
0x62, // 2052-03-31 to 2052-10-27
0x51, // 2053-03-30 to 2053-10-26
0x40, // 2054-03-29 to 2054-10-25
0x36, // 2055-03-28 to 2055-10-31
0x14, // 2056-03-26 to 2056-10-29
0x03, // 2057-03-25 to 2057-10-28
0x62, // 2058-03-31 to 2058-10-27
0x51, // 2059-03-30 to 2059-10-26
0x36, // 2060-03-28 to 2060-10-31
0x25, // 2061-03-27 to 2061-10-30
0x14, // 2062-03-26 to 2062-10-29
0x03, // 2063-03-25 to 2063-10-28
0x51, // 2064-03-30 to 2064-10-26
0x40, // 2065-03-29 to 2065-10-25
0x36, // 2066-03-28 to 2066-10-31
0x25, // 2067-03-27 to 2067-10-30
0x03, // 2068-03-25 to 2068-10-28
0x62, // 2069-03-31 to 2069-10-27
0x51, // 2070-03-30 to 2070-10-26
0x40, // 2071-03-29 to 2071-10-25
0x25, // 2072-03-27 to 2072-10-30
0x14, // 2073-03-26 to 2073-10-29
0x03, // 2074-03-25 to 2074-10-28
0x62, // 2075-03-31 to 2075-10-27
0x40, // 2076-03-29 to 2076-10-25
0x36, // 2077-03-28 to 2077-10-31
0x25, // 2078-03-27 to 2078-10-30
0x14, // 2079-03-26 to 2079-10-29
0x62, // 2080-03-31 to 2080-10-27
0x51, // 2081-03-30 to 2081-10-26
0x40, // 2082-03-29 to 2082-10-25
0x36, // 2083-03-28 to 2083-10-31
0x14, // 2084-03-26 to 2084-10-29
0x03, // 2085-03-25 to 2085-10-28
0x62, // 2086-03-31 to 2086-10-27
0x51, // 2087-03-30 to 2087-10-26
0x36, // 2088-03-28 to 2088-10-31
0x25, // 2089-03-27 to 2089-10-30
0x14, // 2090-03-26 to 2090-10-29
0x03, // 2091-03-25 to 2091-10-28
0x51, // 2092-03-30 to 2092-10-26
0x40, // 2093-03-29 to 2093-10-25
0x36, // 2094-03-28 to 2094-10-31
0x25, // 2095-03-27 to 2095-10-30
0x03, // 2096-03-25 to 2096-10-28
0x62, // 2097-03-31 to 2097-10-27
0x51, // 2098-03-30 to 2098-10-26
0x40, // 2099-03-29 to 2099-10-25
0x36, // 2100-03-28 to 2100-10-31
0x25, // 2101-03-27 to 2101-10-30
0x14, // 2102-03-26 to 2102-10-29
0x03, // 2103-03-25 to 2103-10-28
0x51, // 2104-03-30 to 2104-10-26
0x40, // 2105-03-29 to 2105-10-25
0x36, // 2106-03-28 to 2106-10-31
0x25, // 2107-03-27 to 2107-10-30
0x03, // 2108-03-25 to 2108-10-28
0x62, // 2109-03-31 to 2109-10-27
0x51, // 2110-03-30 to 2110-10-26
0x40, // 2111-03-29 to 2111-10-25
0x25, // 2112-03-27 to 2112-10-30
0x14, // 2113-03-26 to 2113-10-29
0x03, // 2114-03-25 to 2114-10-28
0x62, // 2115-03-31 to 2115-10-27
};



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

byte seconds;
byte minutes = 0x00;
byte hours;
byte weekday;
byte month_day;
byte month_nr;
byte year_nr;

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

  int oldDST;
  int curDST;
  int intHours;

  
  Serial.begin(9600);
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
//    Wire.write((byte)0x80 | 0x15); //3° byte ORE da 0x00 a 0x24
//    Wire.write((byte)0x06); //4° byte GIORNO della settimana da 0x01 a 0x07
//    Wire.write((byte)0x29); //5° byte GIORNO del mese da 0x00 a 0x31
//    Wire.write((byte)0x10); //6° byte MESE da 0x00 a 0x12
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

  readRTC();
    if ( hours > 3 ) { // this check is done only if we are past the changeover time of 2/3 AM
      oldDST = EEPROM.read(DSTADDRESS);
      curDST = IsDst(month_day, month_nr, year_nr);
      if ( curDST != oldDST ) {
        if ( curDST == 0 ) { // we're in solar time, move backwards 1h
          Serial.println("Reverting back to solar time (-1h).");
          intHours = bcdToDec(hours);
          intHours = intHours - 1;
          hours = decToBcd(intHours);
        } else { // we're in DST, move forward 1h
          Serial.println("Switching to DST! (+1h)");
          intHours = bcdToDec(hours);
          intHours = intHours + 1;
          hours = decToBcd(intHours);
          
        }

          Wire.beginTransmission(0x68);
          //il primo byte stabilisce il registro
          //iniziale da scivere
          Wire.write((byte)0x00);
          //specifico il tempo e la data
          Wire.write(seconds); //1° byte SECONDI da 0x00 a 0x59 -- skipping ahead of two seconds
          Wire.write(minutes); //2° byte MINUTI da 0x00 a 0x59
          Wire.write((byte)0x80 | hours); //3° byte ORE da 0x00 a 0x24
          Wire.write((byte)0x02); //4° byte GIORNO della settimana da 0x01 a 0x07
          Wire.write(month_day); //5° byte GIORNO del mese da 0x00 a 0x31
          Wire.write(month_nr); //6° byte MESE da 0x00 a 0x12
          Wire.write(year_nr); //7° byte ANNO 0x00 a 0x99
          Wire.endTransmission();
      } // endif curDST != oldDST

      Serial.print("DST values old/cur: ");
      Serial.print(oldDST, DEC);
      Serial.print("/");
      Serial.println(curDST, DEC);
      
      EEPROM.update(DSTADDRESS, curDST);
    } // end if hour>3
    
  delay(3000);

  // start interrupt at the end of startup sequence
  oldSecondElapsed = secondElapsed = 58;
  attachInterrupt(digitalPinToInterrupt(oneSecondInterruptPin), oneSecondISR, FALLING);  

}


// check if we are in DST timeframe
// Returns 0 in solar time, returns 1 in DST
int IsDst(int day, int month, int year)
{

    int myday = bcdToDec(day);
    int mymonth = bcdToDec(month);
    int myyear = bcdToDec(year);
    int changeOverDays = DSTdays[myyear-16];
    int changeOverToSolar = (changeOverDays & 0x0F) + 25;
    int changeOverToDST = (changeOverDays >> 4) + 25;

    Serial.println(changeOverDays, HEX);
    Serial.println(changeOverToSolar, DEC);
    Serial.println(changeOverToDST, DEC);

    Serial.print("day: ");
    Serial.print(myday, DEC);
    Serial.print(" month: ");
    Serial.print(mymonth, DEC);
    Serial.print(" year: ");
    Serial.println(myyear, DEC);
    
    if (mymonth < 3 || mymonth > 10)  return 0; 
    if (mymonth > 3 && mymonth < 10)  return 1; 
    
    if ((mymonth == 3) && ( myday >= changeOverToDST )) return 1;
    if ((mymonth == 10) && ( myday < changeOverToSolar )) return 1;

    return 0; 
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

void readRTC() {
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
      weekday = Wire.read();
      month_day = Wire.read();
      month_nr = Wire.read();
      year_nr = Wire.read();
      //control = Wire.read();

}

void oneSecondISR() {
  oldSecondElapsed = secondElapsed;
  secondElapsed = secondElapsed + 1;
  }

void loop() {

  int digit;
  int j;
  static boolean firstPass = 1;

//  static byte new_minutes;
//  static byte new_hours;
//  static byte new_month_day;
//  static byte new_month_nr;
//  static byte new_year_nr;

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

            // display hhmm.ddmm20yy
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

            // display hhmm
            case 2:
              printBCD(hours);
              delay(interBlockPause);
              printKey(DECPOINT);
              delay(interBlockPause);
              printBCD(minutes);
              break;

            // display ddmm20yy.hhmm
            case 3:
              printBCD(month_day);
              delay(interBlockPause);
              printBCD(month_nr);
              delay(interBlockPause);
              printBCD(0x20); // year, we're good until 2099 :)
              printBCD(year_nr);              
              delay(interBlockPause);
              printKey(DECPOINT);
              delay(interBlockPause);
              printBCD(hours);
              delay(interBlockPause);
              printBCD(minutes);
              delay(interBlockPause);
              break;

            // display hhmmss ++
            case 4:
              printBCD(hours);
              printBCD(minutes);
              printBCD(seconds);              
              printKey(SUM);
              printKey(1);
              printKey(SUM);
              break;

            // display hhmmss.ddmmyy ++
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
            case 3: break; // nothing do to
              
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
      case 3:
        backspace(13);
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

      readRTC();
      
      firstPass = 1;

      secondElapsed = seconds; // reinit interrupt variable

      mode = bcdToDec(minutes) % 6;
      if (hours < 1) { mode = 0; }

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


