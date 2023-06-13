/*
  Cool on-screen demo.


*/

#include "SPI.h"
#include "tamsyn8x16r.h"

#define SPI_FREQ 1800000
#define SPI_MODE SPI_MODE1
#define SPI_ORDER MSBFIRST
#define CS_PIN 10

#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 400
#define BITS_TO_BYTES(x) ((x+7)/8)
#define SCREEN_WIDTH_BYTES BITS_TO_BYTES(SCREEN_WIDTH)
#define SCREEN_MIN 1
#define SCREEN_MAX SCREEN_HEIGHT

// We rely on GLYPH_WIDTH being 8 specifically in this code
#define GLYPH_WIDTH 8
#define GLYPH_HEIGHT 16

#define VCOM_BIT 0x40

enum {
  glyph_width = 8,
  glyph_height = 16,
  font_line_bytes = (tamsyn8x16r_width + 7) / 8,
  first_font_line = 3,
};


static uint8_t vcom = 0;
inline uint8_t toggle_vcom() {
  return vcom = vcom ? 0 : VCOM_BIT;  // Toggle black polarity so the screen doesn't degrade
}

static unsigned char reverse_nibble[16] = { 0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe, 0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf, };
uint8_t reverse_bits(uint8_t n) {
  return (reverse_nibble[n & 0x0f] << 4) | reverse_nibble[(n & 0xf0) >> 4];
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

uint8_t framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH_BYTES];
void drawBuffer() {
  digitalWrite(CS_PIN, HIGH);
  SPI.beginTransaction(SPISettings(SPI_FREQ, SPI_ORDER, SPI_MODE));
  SPI.transfer(0x80 | toggle_vcom()); // Multi-line, 1=white and 0=black
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    uint8_t line_buffer[SCREEN_WIDTH_BYTES + 2] = {0};
    line_buffer[0] = reverse_bits(y + 1);
    memcpy(line_buffer + 1, framebuffer[y], SCREEN_WIDTH_BYTES);
    line_buffer[sizeof(line_buffer) - 1] = 0x00;
    SPI.transfer(line_buffer, sizeof(line_buffer));
  }
  SPI.transfer(0x00);
  SPI.endTransaction();
  digitalWrite(CS_PIN, LOW);
}

void setPixel(int x, int y) {
  framebuffer[x/8][y] = framebuffer[x/8][y] | (1 << (x%8));
}
void clearPixel(int x, int y) {
  framebuffer[x/8][y] = framebuffer[x/8][y] & ~(1 << (x%8));
}

void drawLetter(char c, int y, int x) {
  int charIndex = c - 32;
  for (int r = 0; r < GLYPH_HEIGHT; r++) framebuffer[y * GLYPH_HEIGHT + r][x] = tamsyn8x16r_bits[BITS_TO_BYTES(tamsyn8x16r_width) * r + charIndex];
}
void drawLetter(char c, int pos) { // 0 < pos <= 750
  drawLetter(c, pos / 50, pos % 50);
}

void drawText(char *text) {
  int pos = 0;
  int x=0;
  int y=0;
  while (text[pos] != 0) {
    x++;
    if (x > 50) { x=0; y++; }
    drawLetter(text[pos], y, x);
    if (text[pos] == '\n') { x=0; y++; }
    pos++;
  }
}

void setup() {
  // Chip select is always on
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, LOW);

  // Clock and data out
  SPI.begin();
  
  memset(framebuffer, 0xff, SCREEN_HEIGHT * SCREEN_WIDTH_BYTES); // Clear the buffer to white
  clearScreen();
}



void demoBlank(int durationMs) {
  clearScreen();
  delay(durationMs);
}

void demoFlash(int durationMs) {
  // TODO: Slowly speed up the strobe rate
  demoFlash(durationMs, 0);
}
void demoFlash(int durationMs, int periodMs) {
  int halfPeriodMs = periodMs / 2;
  for (int i=0; i<durationMs/periodMs; i++) {
    memset(framebuffer, 0x00, SCREEN_HEIGHT * SCREEN_WIDTH_BYTES);
    drawBuffer();
    delay(halfPeriodMs);
    memset(framebuffer, 0xff, SCREEN_HEIGHT * SCREEN_WIDTH_BYTES);
    drawBuffer();
    delay(halfPeriodMs);
  }
}
void demoTyping(int durationMs, char *text) {
  // TODO: Slowly speed up the typing rate
  demoTyping(durationMs, 16, text);
}
void demoTyping(int durationMs, int msPerChar, char *loopText) {
  int textLen = strlen(loopText);
  for (int i=0; i<=(textLen/msPerChar); i++) {
    if (i%750 == 0) clearScreen();
    drawLetter(loopText[i%textLen], i%750);
    drawBuffer();
    delay(msPerChar);
  }  
}
void demoMunchingSquares(int durationMs) {
  // TODO: Slowly speed up the framerate
  demoMunchingSquares(durationMs, 16);
}
void demoMunchingSquares(int durationMs, int frameMs) {
  for (int t=0; t<durationMs/frameMs; t++) {
    memset(framebuffer, 0xff, SCREEN_HEIGHT * SCREEN_WIDTH_BYTES);
    for (int x=0; x<400; x++) {
      int y = x % t;
      setPixel(x,y);
    }
    drawBuffer();
    delay(frameMs);
  }
}

void demoText(int durationMs, char *text) {
  drawText(text);
  delay(durationMs);
}

void loop() {
  demoText(1000, "\n\n\n    zorchpad alpha\n\n      graphics demo");
  demoBlank(100);
  demoFlash(500);
  demoBlank(100);
  demoText(500, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVXYZ1234567890-=!@#$%^&*()_+[]{};':\",.<>/?~\\|~\nThe quick brown fox jumped over the lazy dog.\n");
  demoMunchingSquares(2000);
  demoTyping(1000, "The quick brown fox jumped over the lazy dog. ");
}
