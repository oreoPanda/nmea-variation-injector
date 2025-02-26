const char* magVar;// = "1.0,E";
const char* dollar;// = "$";
const char* star;// = "*";

//#define DEBUG

void setup() {
  // initialize serial:
  Serial.begin(9600);

  magVar = "1.0,E";
  dollar = "$";
  star = "*";
}

void loop() {
  // create a (way too large) buffer and read from serial until newline char
  char buffer[300];
  int len = Serial.readBytesUntil('\n', buffer, 300);

  //if reading from serial went fine, len will be gt 0 and show the number of chars
  if(len>0){
    // copy buffer into char* that fits and 0-terminate it
    char inChars[len];
    strncpy(inChars, buffer, len);
    inChars[len] = '\0';

#ifdef DEBUG
    Serial.print("Input raw: ");
    Serial.println(inChars);
#endif

    //check for GPRMC data
    if (len >= 6 && (strncmp("$GPRMC",inChars,6) == 0)) {
      //remove leading $ and trailing data that will be replaced (,,*<CS>)
      char gprmcPayload[len-7];
      strncpy(gprmcPayload, inChars + 1, len-7);
      gprmcPayload[len-7] = '\0';

  #ifdef DEBUG
      Serial.print("Input payload: ");
      Serial.println(gprmcPayload);
  #endif

      //add new constant magnetic variation
      char* gprmcPayloadWithMagVar;
      gprmcPayloadWithMagVar = malloc(strlen(gprmcPayload)+5+1); //+5 for magVar, +1 for \0
      strcpy(gprmcPayloadWithMagVar, gprmcPayload);
      strcat(gprmcPayloadWithMagVar, magVar);

  #ifdef DEBUG
      Serial.print("New payload: ");
      Serial.println(gprmcPayloadWithMagVar);
  #endif

      //calculate new checksum
      byte cs = byte(gprmcPayloadWithMagVar[0]);
      for(int i=1; i<strlen(gprmcPayloadWithMagVar); i++){
        cs ^= byte(gprmcPayloadWithMagVar[i]);
      }

      //convert checksum to hex string
      char hexString[3];
      sprintf(hexString, "%02X", cs);
      
      //calculate new output: $ + payload with magnetic variation + * + new checksum
      char* outPayload;
      outPayload = malloc(strlen(gprmcPayloadWithMagVar)+1+1+2+1); //+1 for $, +1 for *, +2 for CS +1 for \0
      strcpy(outPayload, dollar);
      strcat(outPayload, gprmcPayloadWithMagVar);
      strcat(outPayload, star);
      strcat(outPayload, hexString);

  #ifdef DEBUG
      Serial.print("Finished output data: ");
  #endif
      
      //send modified data
      Serial.println(outPayload);

      //free up memory allocated with malloc
      free(gprmcPayloadWithMagVar);
      free(outPayload);
      
    }else{
      //send unmodified input data
      Serial.println(inChars);
    }

  }
}
