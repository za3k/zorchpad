#include "SPI.h"
#include "tamsyn8x16r.h"

// On the keypad, the order is:
// C3 R0 R1 R2 R3 C0 C1 C2
// These pins are hooked up to those (Analog In, A0 to A3; Digital PWM out, 4 to 7)
// 16 17 18 19  7  6  5  4

int COL_PINS[4] = {6,5,4,16}; 
int ROW_PINS[4] = {17,18,19,7};

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
char text[750];
char *endText;
char lastButton;

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

void sendCol(int col) {
  for (int i=0; i<4; i++) {
    int pin = COL_PINS[i];
    if (col != i) pinMode(pin, INPUT_PULLUP);
    else {        pinMode(pin, OUTPUT);
                  digitalWrite(pin, LOW); }
  }
}

int readRow(int row) {
  for (int j=0; j<4; j++) {
    int pin = ROW_PINS[j];
    pinMode(pin, INPUT_PULLUP);
  }
  return digitalRead(ROW_PINS[row]) == LOW;
}

char getPressedButton() {
  int ERR = 444;
  unsigned int pressedBits=0;
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      unsigned int currentBit = 1u << (i*4+j);
      sendCol(i);
      int b = readRow(j);
      //Serial.print(b);
      if (b) pressedBits |= currentBit;
    }
  }
  sendCol(-1);
  switch (pressedBits) {
    case 1: return '0';
    case 2: return '4';
    case 4: return '8';
    case 8: return 'C';
    case 16: return '1';
    case 32: return '5';
    case 64: return '9';
    case 128: return 'D';
    case 256: return '2';
    case 512: return '6';
    case 1024: return 'A';
    case 2048: return 'E';
    case 4096: return '3';
    case 8192: return '7';
    case 16384: return 'B';
    case 32768: return 'F';
    default: return 0;
  }
  //Serial.print("\n");
}

void setup() {
  // Chip select is always on
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, LOW);
  
  // Clock and data out
  SPI.begin();
  
  memset(framebuffer, 0xff, SCREEN_HEIGHT * SCREEN_WIDTH_BYTES); // Clear the buffer to white
  clearScreen();

  char *welcome =  "> ";
  strcpy(text, welcome);
  endText = text + strlen(welcome);
  lastButton = 0;

  //Serial.begin(9600);
}

void loop() {
  char pb = getPressedButton();
  if (pb && pb != lastButton) {
    if (endText - text > 700) {
      endText = text;
      text[0] = 0;
    }
    
    lastButton = pb;
    *endText++ = pb;
    *endText = 0;
  } else if (pb == 0) lastButton = 0;

  memset(framebuffer, 0xff, SCREEN_HEIGHT * SCREEN_WIDTH_BYTES);
  drawText(text);
  drawBuffer();
  delay(10);
}
