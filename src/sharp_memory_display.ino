/*
  Memory Display

  Draws some stuff on the screen to test.
  
*/

#include "SPI.h"
#define SPI_FREQ 1800000
#define SPI_MODE SPI_MODE1
#define SPI_ORDER MSBFIRST
#define CS_PIN 10

#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 400
#define SCREEN_MIN 1
#define SCREEN_MAX SCREEN_HEIGHT

#define VCOM_BIT 0x40

static uint8_t vcom = 0;
inline uint8_t toggle_vcom() { return vcom = vcom ? 0 : VCOM_BIT; } // Toggle black polarity so the screen doesn't degrade

static unsigned char reverse_nibble[16] = { 0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe, 0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf, };
uint8_t reverse_bits(uint8_t n) {
   return (reverse_nibble[n&0x0f] << 4) | reverse_nibble[(n&0xf0)>>4];
}

int msb(int n) {
  int position = 0;
  while (n) { n >>= 1; position++; }
  return position;
}

int test_pattern(int x, int y) {
  // Is the highest one bit of X^Y in an odd position? Makes something fractal checkboard-y.
  return msb(x^y)%2;
}

void clearScreen() {
  digitalWrite(CS_PIN, HIGH);
  SPI.beginTransaction(SPISettings(SPI_FREQ, SPI_ORDER, SPI_MODE));
  SPI.transfer(0x20 | toggle_vcom());
  SPI.transfer(0x00);
  SPI.endTransaction();
  delay(1);
  digitalWrite(CS_PIN, LOW);
  delay(1);
}

void drawConstScreen(uint8_t pattern) { // 1=white and 0=black
  digitalWrite(CS_PIN, HIGH);
  SPI.beginTransaction(SPISettings(SPI_FREQ, SPI_ORDER, SPI_MODE));
  SPI.transfer(0x80 | toggle_vcom()); // Multi-line
  for (int y=SCREEN_MIN; y<=SCREEN_MAX; y++) {
    uint8_t line_buffer[(SCREEN_WIDTH/8+2)];
    line_buffer[0] = reverse_bits(y);
    for (int i=1; i<=SCREEN_WIDTH/8; i++) line_buffer[i]=pattern;
    line_buffer[sizeof(line_buffer)-1] = 0x00;
    SPI.transfer(line_buffer, sizeof(line_buffer));
  }
  SPI.transfer(0x00);
  SPI.endTransaction();
  digitalWrite(CS_PIN, LOW);
}

void drawBlackScreen() {
  drawConstScreen(0x00);
}
void drawWhiteScreen() {
  drawConstScreen(0xff);
}


void drawTestScreen() {
  // Draw a test pattern
  digitalWrite(CS_PIN, HIGH);
  SPI.beginTransaction(SPISettings(SPI_FREQ, SPI_ORDER, SPI_MODE));
  SPI.transfer(0x80 | toggle_vcom()); // Multi-line, 1=white and 0=black
  for (int y=SCREEN_MIN; y<=SCREEN_MAX; y++) {
    uint8_t line_buffer[(SCREEN_WIDTH/8+2)] = {0};
    line_buffer[0] = reverse_bits(y);
    for (int i=0; i<SCREEN_WIDTH/8; i++) {
      uint8_t next_byte = 0;
      for (int j=0; j<8; j++) {
        int x = i*8+j;
        if (test_pattern(x,y)) next_byte |= (0x80>>j); // little-endian
      }
      line_buffer[i+1]=next_byte;  
    }
    line_buffer[sizeof(line_buffer)-1] = 0x00;
    SPI.transfer(line_buffer, sizeof(line_buffer));
  }
  SPI.transfer(0x00);
  SPI.endTransaction();
  digitalWrite(CS_PIN, LOW);
}

// the setup function runs once when you press reset or power the board
void setup() {
  // Chip select is always on
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, LOW);

  // Clock and data out
  SPI.begin();
  clearScreen();
  drawTestScreen();
}

// the loop function runs over and over again forever
void loop() {
  drawBlackScreen();
  clearScreen();

  // Toggling the LED seems to affect SCK based on oscilloscope. Don't do that in the loop.
}
