/*
  Dump memory contents to the screen and serial port.
*/

#include "SPI.h"
#include "tamsyn8x16r.h"
#include <LibPrintf.h>

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

static uint8_t vcom = 0;
inline uint8_t toggle_vcom() {
  return vcom = vcom ? 0 : VCOM_BIT;  // Toggle black polarity so the screen doesn't degrade
}

static unsigned char reverse_nibble[16] = { 0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe, 0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf, };
uint8_t reverse_bits(uint8_t n) {
  return (reverse_nibble[n & 0x0f] << 4) | reverse_nibble[(n & 0xf0) >> 4];
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
  for (int r = 0; r < GLYPH_HEIGHT; r++) framebuffer[y * GLYPH_HEIGHT + r][x] = tamsyn8x16r_bits[BITS_TO_BYTES(tamsyn8x16r_width) * r + charIndex];
}

void drawLine(char *text, int line) {
  int x = 0;
  while (text[x] != 0 && x < 50) {
    drawLetter(text[x], line, x);
    x++;
  }
}

void setup() {
  Serial.begin(1000000);
  // Chip select is always on
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, LOW);

  // Clock and data out
  SPI.begin();

  memset(framebuffer, 0xff, SCREEN_HEIGHT * SCREEN_WIDTH_BYTES); // Clear the buffer to white
  drawBuffer();

  //hexDump(0, 0x17f000); // Memory
  hexDump(0x10000000ull, 0x1005FFFFull); // Boot loader ROM
}

// Flash/ROM goes 0x000 0000 - 0x1fff ffff  somewhere
// RAM 0x10000000 to 0x1005FFFF
// Flash goes 0x0000 0000 - 0xffff ffff somewhere

void hexDump(unsigned long long minAddress, unsigned long long maxAddress) {
  char lineBuffer[50];
  unsigned long long memoryPos = minAddress;
  while (memoryPos < maxAddress) {
    for (int line = 0; line < 15; line++) {
      // Read bytes
      unsigned long long *A = (unsigned long long*)memoryPos;
      //printf("Reading %08x\n", A);
      unsigned long long D0 = *(A); // first 8 bytes
      //printf("Reading %08x\n", A+1);
      unsigned long long D1 = *(A+1); // second 8 bytes
      uint16_t d0 = D0&0xFFFF000000000000 >> 48,
               d1 = D0&0x0000FFFF00000000 >> 32,
               d2 = D0&0x00000000FFFF0000 >> 16,
               d3 = D0&0x000000000000FFFF,
               d4 = D1&0xFFFF000000000000 >> 48,
               d5 = D1&0x0000FFFF00000000 >> 32,
               d6 = D1&0x00000000FFFF0000 >> 16,
               d7 = D1&0x000000000000FFFF;
      sprintf(lineBuffer, "%08x: %04x %04x %04x %04x %04x %04x %04x %04x", memoryPos, d0, d1, d2, d3, d4, d5, d6, d7);
      Serial.println(lineBuffer);
      memoryPos+=16;
      drawLine(lineBuffer, line);
    }
    drawBuffer();
  }
}

void loop() {
}
