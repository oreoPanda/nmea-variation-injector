String inputString = "";      // a String to hold incoming data
String outputString = "";
int indexx;
bool stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:  
  if (stringComplete) {
    if (inputString.startsWith("$GPRMC")) {
      //scroll to the 10th comma
      indexx = 0;
      for (int i = 0; i < 10; i++) {
        indexx = inputString.indexOf(',', indexx+1);    //TODO handle the -1 as error
      }
      //we now have the index of the 10th comma stored in the variable called index
      outputString = inputString.substring(0, indexx+1);
      outputString.concat("1.0,E");
      //find the index of the 11th comma in the inputString. This should be just next to the 10th comma, unless a deviation is already given
      indexx = indexx +4;
      //append the rest of the inputString to the outputString
      outputString.concat(inputString.substring(indexx));
      //put the outputString onto the serial connection
      outputString.concat("\r\n");
      Serial.print(outputString);      
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