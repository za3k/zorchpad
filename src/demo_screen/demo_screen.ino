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

uint8_t framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH_BYTES];
void redrawLines(int yMin, int yMax) {
  digitalWrite(CS_PIN, HIGH);
  SPI.beginTransaction(SPISettings(SPI_FREQ, SPI_ORDER, SPI_MODE));
  SPI.transfer(0x80 | toggle_vcom()); // Multi-line, 1=white and 0=black
  for (int y = yMin; y < yMax; y++) {
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
void drawBuffer() {
  redrawLines(0, SCREEN_HEIGHT);
}

void setPixel(int x, int y) {
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
  framebuffer[y][x/8] = framebuffer[y][x/8] & ~(1 << (x%8));
}
void clearPixel(int x, int y) {
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
  framebuffer[y][x/8] = framebuffer[y][x/8] | (1 << (x%8));
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
    
    if (text[pos] == '\n') { x=0; y++; }
    else {
      drawLetter(text[pos], y, x);
      if (++x >= 50) { x=0; y++; }
    }
    pos++;
  }
}

void redrawTextLine(int y) {
  redrawLines(y*GLYPH_HEIGHT,y*GLYPH_HEIGHT+GLYPH_HEIGHT);
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
  demoFlash(durationMs, 0);
}
void demoFlash(int durationMs, int periodMs) {
  int halfPeriodMs = periodMs / 2;
  int startTime = millis();
  for (int t=0; millis() - startTime < durationMs; t++) {
    drawConstScreen(t % 2 ? 0x00 : 0xff);
    delay(halfPeriodMs);
  }
}
void demoTyping(int durationMs, char *text) {
  memset(framebuffer, 0xff, SCREEN_HEIGHT * SCREEN_WIDTH_BYTES);
  // Slowly speed up the typing rate
  int pos = 0;
  pos = demoTyping(durationMs / 8, 64, text, pos);
  pos = demoTyping(durationMs / 8, 32, text, pos);
  pos = demoTyping(durationMs / 8, 16, text, pos);
  pos = demoTyping(durationMs / 8, 8, text, pos);
  pos = demoTyping(durationMs / 8, 4, text, pos);
  pos = demoTyping(durationMs / 8, 2, text, pos);
  pos = demoTyping(durationMs / 8, 1, text, pos);
}

void demoTyping(int durationMs, int msPerChar, char *loopText) { demoTyping(durationMs, msPerChar, loopText, 0); }
int demoTyping(int durationMs, int msPerChar, char *loopText, int startPos) {
  int startTime = millis();
  int textLen = strlen(loopText);
  int i;
  for (i=startPos; millis() - startTime < durationMs; i++) {
    if (i%750 == 0) clearScreen();
    drawLetter(loopText[i%textLen], i%750);
    redrawTextLine((i%750)/50);
    delay(msPerChar);
  }
  return i;
}
void demoMunchingSquares(int durationMs) {
  demoMunchingSquares(durationMs, 0);
}
void demoMunchingSquares(int durationMs, int frameMs) {
  int startTime = millis();
  memset(framebuffer, 0xff, SCREEN_HEIGHT * SCREEN_WIDTH_BYTES);
  for (unsigned int t=0; millis() - startTime < durationMs; t++) {
    for (unsigned int x=0; x<400; x++) {
      unsigned int y = x ^ t;
      setPixel(x,y);
    }
    drawBuffer();
    delay(frameMs);
  }
}

void demoText(int durationMs, char *text) {
  memset(framebuffer, 0xff, SCREEN_HEIGHT * SCREEN_WIDTH_BYTES);
  drawText(text);
  drawBuffer();
  delay(durationMs);
}

void loop() {
  demoText(1000, 
  "\n\n\n"
  "                 zorchpad pre-alpha\n\n"                  // End of screen
  "                 graphics demo");
  demoBlank(100);
  demoFlash(1000);
  demoBlank(100);
  demoText(2000, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVXYZ1234567890-=!@#$%^&*()_+[]{};':\",.<>/?~\\|~\nThe quick brown fox jumped over the lazy dog.\n");
  demoMunchingSquares(5000);
  demoTyping(5000, "The quick brown fox jumped over the lazy dog. ");
}
