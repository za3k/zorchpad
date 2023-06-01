/*
  Audio Output

  Output some beeps
*/

#define AUDIO_PIN 2
#define DEFAULT_FREQ 100

// the setup function runs once when you press reset or power the board
void setup() {
  // Chip select is always on
  pinMode(AUDIO_PIN, OUTPUT);
  analogWrite(AUDIO_PIN, 0);
}

void tone(int durationMs, int freq) {
  unsigned long periodMicroseconds = 1000000/freq;
  unsigned long startMicroseconds = micros();
  unsigned long us;
  do {
    us = micros();
    analogWrite(AUDIO_PIN, (us % periodMicroseconds < periodMicroseconds / 2) * 255); // Square wave 
  } while (us < startMicroseconds + durationMs*1000); 
}
void tone(int durationMs) { tone(durationMs, DEFAULT_FREQ); }

// The standard word (PARIS) is 50 units long. If a dit was 1200ms long, you would transmit
// at 1 wpm.
#define WPM 20
// Farnsworth spacing. To learn morse, it's better to add gaps between characters instead of slowing down the whole thing. Make this as low as you need.
#define WORD_WPM 5

#define DIT (1200/WPM)
#define SPACE_DIT (1200/WORD_WPM)

#define dit tone(DIT); delay(DIT);
#define di dit
#define dah tone(DIT*3); delay(DIT);
// Called after dit or dah, for a total of 3 units
#define letter_space delay(SPACE_DIT*3-DIT);
// Called after dit or dah, for a total of 7 units
#define word_space delay(SPACE_DIT*7-DIT);

void sayMorse(char letter) {
  // Codes from https://morsecode.world/international/morse.html
  switch (letter) {
    case 'a': case 'A': dit dah; break;
    case 'b': case 'B': dah dit dit dit; break;
    case 'c': case 'C': dah dit dah dit; break;
    case 'd': case 'D': dah dit dit; break;
    case 'e': case 'E': dit; break;
    case 'f': case 'F': dit dit dah dit; break;
    case 'g': case 'G': dah dah dit; break;
    case 'h': case 'H': dit dit dit dit; break;
    case 'i': case 'I': dit dit; break;
    case 'j': case 'J': dit dah dah dah; break;
    case 'k': case 'K': dah dit dah; break;
    case 'l': case 'L': dit dah dit dit; break;
    case 'm': case 'M': dah dah; break;
    case 'n': case 'N': dah dit; break;
    case 'o': case 'O': dah dah dah; break;
    case 'p': case 'P': dit dah dah dit; break;
    case 'q': case 'Q': dah dah dit dah; break;
    case 'r': case 'R': dit dah dit; break;
    case 's': case 'S': dit dit dit; break;
    case 't': case 'T': dah; break;
    case 'u': case 'U': dit dit dah; break;
    case 'v': case 'V': dit dit dit dah; break;
    case 'w': case 'W': dit dah dah; break;
    case '*':
    case 'x': case 'X': dah dit dit dah; break;
    case 'y': case 'Y': dah dit dah dah; break;
    case 'z': case 'Z': dah dah dit dit; break;
    case '0': dah dah dah dah dah; break;
    case '1': dit dah dah dah dah; break;
    case '2': dit dit dah dah dah; break;
    case '3': dit dit dit dah dah; break;
    case '4': dit dit dit dit dah; break;
    case '5': dit dit dit dit dit; break;
    case '6': dah dit dit dit dit; break;
    case '7': dah dah dit dit dit; break;
    case '8': dah dah dah dit dit; break;
    case '9': dah dah dah dah dit; break;
    case ' ': word_space; return; break;
    case '&': di dah di di dit; break;
    case '\'': di dah dah dah dah dit; break;
    case '@': di dah dah di dah dit; break;
    case ')': dah di dah dah di dah; break;
    case '(': dah di dah dah dit; break;
    case ':': dah dah dah di di dit; break;
    case ',': dah dah di di dah dah; break;
    case '=': dah di di di dah; break;
    case '!': dah di dah di dah dah; break;
    case '.': di dah di dah di dah; break;
    case '-': dah di di di di dah; break;
    case '%': sayMorse("0/0"); return; break;
    case '+': di dah di dah dit; break;
    case '"': di dah di di dah dit; break;
    case '?': di di dah dah di dit; break;
    case '/': dah di di dah dit; break;
    default: return; break;
  }
  // Post-letter gap
  letter_space;
}

void sayMorse(char *words) {
  // Ignores prosigns like SOS
  while (*words != 0) {
    sayMorse(*words++);
  }
}

void loop() {
  //sayMorse("SOS "); //dit dit dit dah dah dah dit dit dit word_space;
  //sayMorse("What hath God wrought? ");
  sayMorse("Hello ");
}
