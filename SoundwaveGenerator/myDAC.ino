/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

int freq_notes[] = {262, 294, 330, 349, 392, 440, 494};
int melody[] = {0, 1, 2, 3, 4, 5, 6, 1, 1, 5, 4, 3};   // notes

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i=0; i<4; i++) {
    pinMode(50+i, OUTPUT);
  }
  for (int i=0; i<2; i++) {
    pinMode(10+i, OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
  //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(100);                       // wait for a second
  //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //delay(2000);                       // wait for a second
  /*int f=0;
  int speedF = 5;
  for (int tf=0; tf<500; tf+=speedF) {
    f = 100 + 1000+500*sin(2*3.14*tf);
    
    for (int t=0; t<100000/f; t++) {
      PORTB = 64 * t / (100000/f); //64+64*sin(2*3.14*t/f);
      delayMicroseconds(10);
    }
  }*/
  int f;
  for (int i=0; i<12; i++) {
    f = freq_notes[melody[i]];

    for (int j=0; j<f; j++) {
      for (int t=0; t<10000/f; t++) {
        PORTB = 64 * t / (10000/f); //64+64*sin(2*3.14*t/f);
        delayMicroseconds(10);
      }
    }
  }
  
}
