# nmea-variation-injector
Arduino project that manipulates NMEA 0183 sentences to insert magnetic variation data.

This Sketch reads NMEA 0183 sentences from the serial Port and sends them on the same serial port. It inserts a fixed value for the magnetic variation into every $GPRMC sentence, other sentences remain unaltered.
Future additions may include a user-defined variation value, this may be realised by setting pre-defined values over digital inputs, by using a potentiometer connected to an analog input or even by communicating with a variation database over another serial connection. It can also be altered to mess with other NMEA 0183 sentences as well. The possibilities are endless.

## Note about the usage of the `serialEvent()` function
This sketch makes use of the `serialEvent()` function, which is not supported on all Arduino boards.
  > "Please note that `serialEvent()` does not work on any modern Arduino boards. The only recognized boards to have support as of 2023/12/06 are the UNO R3, Nano, Mega 2560 Rev3 and Due.
Instead, you can use the `available()` method. Examples in this page demonstrates how to read serial data only when it is available, similarly to how `Serial.event()` is implemented." [[1]](#1)

## References
<a id="1">[1]</a> 
Arduino (2025). 
serialEvent | Arduino Documentation
https://docs.arduino.cc/language-reference/en/functions/communication/serial/serialEvent/
