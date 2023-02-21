const byte hallSensor = 2;
const int throttlePin = A0;
const int pwmPin = 5;

int throttleVal = 0;
unsigned long firstTime;
volatile unsigned long delta;
volatile boolean firstPulse = true;
float speedMs;
float speedKmh;
int power = 0;          // initial pwm value
float wheelSize = 0.79; // wheel perimeter in meter
int maxPower = 168;     // 24v from 36v source (66%)
float maxSpeed = 10.0;  // km/h

void magnetDetected()
{
    // if 0, magnet is in front of hallsensor
    if (digitalRead(hallSensor) == 0 && firstPulse == true)
    {
        firstTime = millis();
        firstPulse = false;
    }
    else if (digitalRead(hallSensor) == 0 && firstPulse == false)
    {
        delta = millis() - firstTime;
        firstPulse = true;
        Serial.print("Delta: ");
        Serial.print(delta);
        Serial.println(" ms");
        speedMs = (1 / (delta / 1000.0)) * wheelSize;
        speedKmh = speedMs * 3.6;
        Serial.print("Speed: ");
        Serial.print(speedKmh);
        Serial.println(" km/h");
    }
}

void setup()
{
    Serial.begin(115200);
    attachInterrupt(0, magnetDetected, CHANGE);
}

void loop()
{
    throttleVal = analogRead(throttlePin);
    power = map(throttleVal, 172, 870, 0, maxPower);

    if (speedKmh < maxSpeed)
    {
        analogWrite(pwmPin, power);
    } else
    {
        analogWrite(pwmPin, 0);
    }
    
    Serial.print ("Throttle: ");
    Serial.println (throttleVal);
    Serial.print ("Power: ");
    Serial.println (power);
}
