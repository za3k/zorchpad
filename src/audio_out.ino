/*
  Audio Output

  Output some beeps
*/

#define AUDIO_PIN 2

// the setup function runs once when you press reset or power the board
void setup() {
  // Chip select is always on
  pinMode(AUDIO_PIN, OUTPUT);
  analogWrite(AUDIO_PIN, 0);
}

#define FREQ 100
#define PERIOD (1000000/FREQ)

#define BEEP_PERIOD 2000
#define BEEP_DUTY 5
void loop() {
  unsigned long us = micros();
  if ((us % (BEEP_PERIOD*1000)) < (BEEP_PERIOD * 1000 * BEEP_DUTY / 100)) { // Beep every 1/2 second
    analogWrite(AUDIO_PIN, (us % PERIOD < PERIOD / 2) * 255); // Square wave
  } else digitalWrite(AUDIO_PIN, 0);
}
