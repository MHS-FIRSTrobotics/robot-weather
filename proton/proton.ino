// This #include statement was automatically added by the Particle IDE.
#include "SparkFun_Photon_Weather_Shield_Library/SparkFun_Photon_Weather_Shield_Library.h"

int ledStatus;
Weather weatherSensor;
double *tempfs[10];
double *humiditys[10];
double *barometers[10];
double *pressures[10];
int currentIndex = 0;

double tempf;
double humidity;
double barometer;
double pressure;



void setup() {
    ledStatus = D7;
    weatherSensor.begin();
    pinMode(ledStatus, OUTPUT);
    weatherSensor.setModeAltimeter();//Set to altimeter Mode

    //These are additional MPL3115A2 functions that MUST be called for the sensor to work.
    weatherSensor.setOversampleRate(7); // Set Oversample rate
    //Call with a rate from 0 to 7. See page 33 for table of ratios.
    //Sets the over sample rate. Datasheet calls for 128 but you can set it
    //from 1 to 128 samples. The higher the oversample rate the greater
    //the time between data samples.


    weatherSensor.enableEventFlags(); //Necessary register calls to enble temp, baro and alt
}

void loop() {
    digitalWrite(ledStatus, HIGH);
    // Measure Relative Humidity from the HTU21D or Si7021
    humiditys[currentIndex] = (double) weatherSensor.getRH();
    //Serial.println("Temp: ");
    tempfs[currentIndex] = (double) weatherSensor.getTempF();
    //Serial.println(tempf);
     //Measure the Barometer temperature in F from the MPL3115A2
    barometers[currentIndex] = (double) weatherSensor.readBaroTempF();
     //Measure Pressure from the MPL3115A2
    pressures[currentIndex] = (double) weatherSensor.readPressure();

    currentIndex = ++currentIndex % 10;

    tempf = avg(tempf);
    humidity = avg(humiditys);
    pressure = avg(pressures);
    barometer = avg(barometers);

    Particle.variable("temperature", &tempf, DOUBLE);
    Particle.variable("humidity", &humidity, DOUBLE);
    Particle.variable("pressure", &pressure, DOUBLE);
    Particle.variable("barometer", &barometer, DOUBLE);

    delay(1000);
    digitalWrite(ledStatus, LOW);
    delay(500);
}

double avg(double *arr {
    int sum = 0;
    int length = sizeof(arr) / sizeof(double);
    for (int i = 0; i < length; i++) {
        sum += arr[i];
    }

    return sum / length;
}