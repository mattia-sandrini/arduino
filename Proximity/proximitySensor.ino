const int trigPin = 48;
const int echoPin = 49;
const int ledPin = 5;

long duration;
float distance;
const int MAX_DISTANCE = 30;
const int MIN_DISTANCE = 6;

const int BUFFER_SIZE = 2;
float buffer[BUFFER_SIZE];
short index = 0;

int fadeValue;
int sum;


int freq_notes[] = {262, 294, 330, 349, 392, 440, 494};
//float freq_notes[] = {0.262, 0.294, 0.330, 0.349, 0.392, 0.440, 0.494};
int melody[] = {0, 1, 2, 3, 4, 5, 6, 1, 1, 5, 4, 3};   // notes


/* Python code for generating the sin lookup table 
import math
n = 40 # 91
s=""
for i in range(40):
    s += str(int(round(255*math.sin((i/(40-1)) * math.pi/2)))) + ', '
    if i%10 == 0 and i!=0:
        s += '\n'
print(s)
*/

// 91 values for a sin lookup table - 8 bit
const int SIN_TABLE_LENGTH = 40; // 91
uint8_t sinTable[] = { 
   0, 10, 21, 31, 41, 51, 61, 71, 81, 90, 100, 
  109, 119, 127, 136, 145, 153, 161, 169, 177, 184, 
  191, 198, 204, 210, 216, 221, 226, 230, 235, 238, 
  242, 245, 248, 250, 252, 253, 254, 255, 255
 };
float tmpCalcDelay = 1.0/(SIN_TABLE_LENGTH*4); // Temporary calculation for the delay


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  for (int i=0; i<BUFFER_SIZE; i++) {
    buffer[i] = 0;
  }

  pinMode(LED_BUILTIN, OUTPUT);
  for (int i=0; i<4; i++) {
    pinMode(50+i, OUTPUT);
  }
  for (int i=0; i<3; i++) {
    pinMode(10+i, OUTPUT);
  }
  
  Serial.begin(9600);
}

float readProximityValue() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  
  buffer[index] = duration*0.034/2;  // distance in cm
  index = (index+1) % BUFFER_SIZE;
  sum = 0;
  for (int i=0; i<BUFFER_SIZE; i++) {
    sum += buffer[i];
  }
  return sum / BUFFER_SIZE;
}

void loop() {
  distance = readProximityValue();
  if (distance > MAX_DISTANCE+3)
    distance = MAX_DISTANCE+3;
  if (distance < 3)
    distance = 3;
  fadeValue = 255 - (255 * (distance-3)/MAX_DISTANCE);
  analogWrite(ledPin, fadeValue);

  float f;
  int i = (int)((fadeValue/255.0) * 6.0 + 0.5f);
  //Serial.println(i);
  f = freq_notes[i];
  //Serial.print("f: ");
  //Serial.println(f);
  
  /*long delay_us = tmpCalcDelay/f * 1000000L;
  //Serial.print("delay_us: ");
  //Serial.println(delay_us);

  for (int k=0; k<40; k++) {
    for (int j=0; j<SIN_TABLE_LENGTH; j++) {
      PORTB = 64.0 + 64.0 * sinTable[j]/255.0;
      //Serial.println(PORTB);
      delayMicroseconds(delay_us);
    }
    for (int j=SIN_TABLE_LENGTH-1; j>=0; j--) {
      PORTB = 64.0 + 64.0 * sinTable[j]/255.0;
      //Serial.println(PORTB);
      delayMicroseconds(delay_us);
    }
    for (int j=0; j<SIN_TABLE_LENGTH; j++) {
      PORTB = 64.0 - 64.0 * sinTable[j]/255.0;
      //Serial.println(PORTB);
      delayMicroseconds(delay_us);
    }
    for (int j=SIN_TABLE_LENGTH-1; j>=0; j--) {
      PORTB = 64.0 - 64.0 * sinTable[j]/255.0;
      //Serial.println(PORTB);
      delayMicroseconds(delay_us);
    }
  }*/
  
  for (int j=0; j<f; j++) {
    for (int t=0; t<10000/f; t++) {
      PORTB = 128 * t / (10000/f); //64+64*sin(2*3.14*t/f);
      //PORTB = 128 + 128*sin(2*3.14*t/(10000/f));
      delayMicroseconds(10);
    }
  }


  
  /*float f;
  distance = readProximityValue();
  if (distance > MAX_DISTANCE+MIN_DISTANCE)
    distance = MAX_DISTANCE+MIN_DISTANCE;
  if (distance < MIN_DISTANCE)
    distance = MIN_DISTANCE;
  f = 200 + (1000.0 * (distance-MIN_DISTANCE)/MAX_DISTANCE);
    
  
  for (int j=0; j<f; j++) {
    int tmp = 10000/f /2.0;
    for (int t=0; t<tmp; t++) {
      PORTB = 128 * t / (10000/f); //64+64*sin(2*3.14*t/f);
      //PORTB = 128 + 128*sin(2*3.14*t/(10000/f));
      delayMicroseconds(10);
    }
    for (int t=0; t<tmp; t++) {
      PORTB = 128 * (1.0 - t / (10000/f)); //64+64*sin(2*3.14*t/f);
      //PORTB = 128 + 128*sin(2*3.14*t/(10000/f));
      delayMicroseconds(10);
    }
  }*/
  

  //Serial.print("Distance: ");
  //Serial.println(distance);
}
