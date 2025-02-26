#include <string.h>

char inputString[100] = "\0";   // string to hold an NMEA sentence
char checkstr[3] = "\0";        // string to hold the checksum
int recv_index = 0;
bool stringComplete = false;  // whether the string is complete

//#define DEBUG

void setup() {
  // initialize serial:
  Serial.begin(9600);
}


/*
 * void calcChecksum(const String &data, String &result)
 *
 * This function evaluates the received NMEA 0183 sentence and sends it over the
 * serial port. If the NMEA 0183 sentence is a $GPRMC sentence, it Inserts a
 * fixed variation value into the sentence and recalculates the checksum. The
 * old checksum is replaced by the new one and the new sentence is then sent over
 * the serial port. Other nmea sentences remain untouched.
 * Note: This function assumes that the variation is not already set in the
 * received $GPRMC sentence and is not safe to use if the value is already set.
 *
 * @param args
 * const String& input: The part of the NMEA 0183 sentence of which the checksum shall be calculated.
 * String& result:      The String to write the checksum into.
 *
 * @param returns
 * nothing
 *
 */
void calcChecksum(const char *input, char *result) {
  byte checksum = byte(input[1]);
  //Serial.print(input.charAt(1));

  for (unsigned int i = 2; i < strlen(input)-1; i++) {
    //Serial.print(input.charAt(i));
    checksum ^= byte(input[i]);
  }
  
  //Serial.println(" ");

  sprintf(result, "%02X", checksum);

}

/*
 * void loop()
 *
 * This function evaluates the received NMEA 0183 sentence and sends it over the
 * serial port. If the NMEA 0183 sentence is a $GPRMC sentence, it Inserts a
 * fixed variation value into the sentence and recalculates the checksum. The
 * old checksum is replaced by the new one and the new sentence is then sent over
 * the serial port. Other nmea sentences remain untouched.
 * Note: This function assumes that the variation is not already set in the
 * received $GPRMC sentence and is not safe to use if the value is already set.
 *
 * @param args
 * none
 *
 * @param returns
 * nothing
 *
 */
void loop() {
  if (stringComplete) {
    if (strstr(inputString, "$GPRMC")) {
      //scroll to the 10th comma
      char *p = inputString;
      for (int i = 0; i < 10; i++) {
        if (!(p = strchr(p+1, ','))) {
          break;    //stop scrolling if there are less than 10 commas
        }
      }
      if (p) {        //only if 10th comma found
        //set the character after the 10th comma as end of the string
        *(p+1) = '\0';
        //add magnetic variation
        strcat(inputString, "1.0,E*");    //WEIRD: Leave out this line and everything works.... WHY??
        //add checksum
        calcChecksum(inputString, checkstr);
        strcat(inputString, checkstr);  //WEIRD: Leave out this line and shortdata looks ok, but data not
        strcat(inputString, "\r\n");
      }
      Serial.write(inputString);
    }
    else {    
      //inputString.concat("\r\n");
      Serial.write(inputString);
    }
    
    // clear the string:
#ifdef DEBUG
    Serial.println("Clearing...");
#endif
    inputString[0] = '\0';
    stringComplete = false;
  }
}

/*
 * void serialEvent()
 *
 * Whenever a new byte is available on the serial port, this function reads
 * the byte from the serial port and appends it to the global variable inputString.
 * If the byte is a newline character ('\n' / decimal 10 / hexadecimal 0x10),
 * it means that the end of an NMEA 0183 sentence was read and the global
 * variable string_complete is set to true.
 *
 * @param args
 * none
 *
 * @param returns
 * nothing
 */
void serialEvent() {
  while (Serial.available() && !stringComplete) {
    if (recv_index >= 100) {
      break;
    }
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString[recv_index++] = inChar;
    // if the incoming character is a newline, set stringComplete
    if (inChar == '\n') {
      inputString[recv_index] = '\0';
      stringComplete = true;
      recv_index = 0;
      //inputString.trim();
      break;
    }
  }
}