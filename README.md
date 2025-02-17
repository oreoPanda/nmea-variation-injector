# nmea-variation-injector
Arduino project that manipulates NMEA 0183 sentences to insert magnetic variation data.

This Sketch reads NMEA 0183 sentences from the serial Port and sends them on the same serial port. It inserts a fixed value for the magnetic variation into every $GPRMC sentence, other sentences remain unaltered.
Future additions may include a user-defined variation value, this may be realised by setting pre-defined values over digital inputs, by using a potentiometer connected to an analog input or even by communicating with a variation database over another serial connection. It can also be altered to mess with other NMEA 0183 sentences as well. The possibilities are endless.
