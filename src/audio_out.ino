/*
  Audio Output

  Output some beeps
*/

#define AUDIO_PIN 2

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(AUDIO_PIN, OUTPUT);
  analogWrite(AUDIO_PIN, 0);
}

#define FREQ 400
#define PERIOD (1000000/FREQ)

// Beep for 0.02 seconds, every 0.2 seconds
#define BEEP_PERIOD 200
#define BEEP_DUTY 10

// the loop function runs over and over again forever
void loop() {
  unsigned long us = micros();
  if (us % (BEEP_PERIOD*1000) < (BEEP_PERIOD * 1000 * BEEP_DUTY / 100)) {
    analogWrite(AUDIO_PIN, (us % PERIOD < PERIOD / 2) * 255); // Start with square wave. We can figure out volume modulation, frequency, etc later.
  } else {
    digitalWrite(AUDIO_PIN, 0);
  }
}
