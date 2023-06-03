/*
  Memory Display w/Framebuffer
  Add a font

  Draws some stuff on the screen to test.

*/

#include "SPI.h"
#include "tamsyn8x16r.h"

#define SPI_FREQ 1800000
#define SPI_MODE SPI_MODE1
#define SPI_ORDER MSBFIRST
#define CS_PIN 10

#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 400
#define SCREEN_WIDTH_BYTES 50
//((SCREEN_WIDTH+7)/8)
#define SCREEN_MIN 1
#define SCREEN_MAX SCREEN_HEIGHT

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

void drawLetter(char c, int y, int x) {
  int charIndex = c - 32;
  for (int r = 0; r < 16; r++) framebuffer[y * 16 + r][x] = tamsyn8x16r_bits[(tamsyn8x16r_width / 8) * r + charIndex];
}
void drawLetter(char c, int pos) { // 0 < pos <= 750
  drawLetter(c, pos / 50, pos % 50);
}

void drawText(char *text) {
  int pos = 0;
  while (text[pos] != 0) {
    drawLetter(text[pos], pos);
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

void loop() {
  drawText("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVXYZ1234567890-=!@#$%^&*()_+[]{};':\",.<>/?~\\|~");
  drawBuffer();
}
