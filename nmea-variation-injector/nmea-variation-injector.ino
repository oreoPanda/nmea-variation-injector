String inputString = "";      // a String to hold incoming data
String checksum = "";
int indexx;
bool stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve sufficient bytes for the strings:
  checksum.reserve(10);
  inputString.reserve(300);
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
void calcChecksum(const String& input, String& result) {
  byte checksum = byte(input.charAt(0));

  for (int i = 1; i < input.length(); i++) {
    checksum ^= byte(input.charAt(i));
  }

  char hexString[3];
  sprintf(hexString, "%02X", checksum);

  result = String(hexString);
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
    if (inputString.startsWith("$GPRMC")) {
      //scroll to the 10th comma
      indexx = 0;
      for (int i = 0; i < 10; i++) {
        indexx = inputString.indexOf(',', indexx+1);    //TODO handle the -1 as error
      }
      //we now have the index of the 10th comma stored in the variable called index. Remove everything after the 10th comma:
      inputString.remove(indexx+1);
      //add magnetic variation
      inputString.concat("1.0,E*");
      //add checksum
      calcChecksum(inputString, checksum);
      inputString.concat(checksum);
      inputString.concat("\r\n");
      Serial.print(inputString);
    }
    else {
      inputString.concat("\r\n");
      Serial.print(inputString);
    }
    
    // clear the string:
    inputString = "";
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
  while (Serial.available()) {    
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:    
    if (inChar == '\n') {      
      stringComplete = true;
      inputString.trim();
    }
  }
}