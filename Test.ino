#include <Arduino.h>
#include "MHZ19.h"
#define ESP32


#define RX_PIN 16                                          // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN 17                                          // Tx pin which the MHZ19 Rx pin is attached to
#define BAUDRATE 9600                                      // Device to MH-Z19 Serial baudrate (should not be changed)

MHZ19 myMHZ19;                                             // Constructor for library
HardwareSerial mySerial(2);                                // On ESP32 we do not require the SoftwareSerial library, since we have 2 USARTS available

unsigned long getDataTimer = 0;

void setup()
{
    Serial.begin(9600);                                     // Device to serial monitor feedback
    
    //mySerial.begin(BAUDRATE);                               // (Uno example) device to MH-Z19 serial start
    //myMHZ19.begin(mySerial);                                // *Serial(Stream) reference must be passed to library begin().

    Serial2.begin(BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN);
    myMHZ19.begin(Serial2);

    Serial.println("Go to Calibration");

    //myMHZ19.autoCalibration();                              // Turn auto calibration ON (OFF autoCalibration(false))

    //Serial.println("OUT Calibration");

    myMHZ19.autoCalibration(false);     // make sure auto calibration is off
    Serial.print("ABC Status: "); myMHZ19.getABC() ? Serial.println("ON") :  Serial.println("OFF");  // now print it's status
    
    Serial.println("Waiting 20 minutes to stabalise...");
   /* if you don't need to wait (it's already been this amount of time), remove the 2 lines */
    getDataTimer = 12e5;   //  20 minutes in milliseconds
    delay(getDataTimer);    //  wait this duration

    Serial.println("Calibrating..");
    myMHZ19.calibrate();    // Take a reading which be used as the zero point for 400 ppm 
}

void loop()
{
    if (millis() - getDataTimer >= 2000)
    {
        int CO2;

        /* note: getCO2() default is command "CO2 Unlimited". This returns the correct CO2 reading even
        if below background CO2 levels or above range (useful to validate sensor). You can use the
        usual documented command with getCO2(false) */

        CO2 = myMHZ19.getCO2();                             // Request CO2 (as ppm)

        Serial.print("CO2 (ppm): ");
        Serial.println(CO2);

        int8_t Temp;
        Temp = myMHZ19.getTemperature();                     // Request Temperature (as Celsius)
        Serial.print("Temperature (C): ");
        Serial.println(Temp);

        getDataTimer = millis();
    }
}
