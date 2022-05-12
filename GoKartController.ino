const byte hallSensor = 2;
const int buttonPin = 3;
const int pwmPin = 5;

int buttonState = 0; 
unsigned long firstTime;
volatile unsigned long delta;
volatile boolean firstPulse = true;
float speedMs;
float speedKmh;
int power = 0; // initial pwm value
float wheelSize = 0.79; // wheel perimeter in meter
int maxPower = 168; // 24v from 36v source (66%)
float maxSpeed = 10.0; // km/h

void magnetDetected ()
{
  // if 0, magnet is in front of hallsensor
  if (digitalRead (hallSensor) == 0 && firstPulse == true) {
      firstTime = millis (); 
      firstPulse = false;
    }
  else if (digitalRead (hallSensor) == 0 && firstPulse == false) {
      delta = millis () - firstTime;  
      firstPulse = true;
      Serial.print ("Delta: ");
      Serial.print (delta);
      Serial.println (" ms");
      speedMs =  ( 1 / ( delta / 1000.0 ) ) * wheelSize ;
      speedKmh = speedMs * 3.6;
      Serial.print ("Speed: ");
      Serial.print ( speedKmh );
      Serial.println (" km/h"); 
    }
}


void setup () {
  Serial.begin (115200);
  attachInterrupt (0, magnetDetected, CHANGE);
  pinMode(buttonPin, INPUT);
}

void loop () {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    Serial.println ("power on");
      if ( speedKmh < 10.0 ) {
        if ( power < maxPower ) {
          power++;
        }
      } else {
        if ( power > 0 ){
          power--;
        }
      }
      Serial.print ("Power: ");
      Serial.println (power);
    } else {
      power = 0;
      speedKmh = 0;
    }
    analogWrite(pwmPin, power);
    delay (5);
  }
