// Set pinouts as constants in the correct order
const int COLS[8] = {12, 2, 3, 9, 5, 10, 14, 15};
const int ROWS[8] = {8, 13, 7, 11, 0, 6, 1, 4};
const int NUM_COLS = (sizeof(COLS) / sizeof(COLS[0]));
const int NUM_ROWS = (sizeof(ROWS) / sizeof(ROWS[0]));

// TODO: Make buffers into array for easy bitwise swapping (bitwise-NOT the active buffer **probably can invert display too**)

/* TODO: Make buffer for each side of border for off screen memory (size of screen each), to store character off screen.
Can be used for wrap-around or character caching. */

// Init display buffer
int buffer[NUM_COLS];
// Second display buffer for quick swap
int b_buffer[NUM_COLS];

int x, y;


void matrixDraw() {
  // Draw display buffer to output
  for (x = 0; x < NUM_COLS; x++) {
    digitalWrite(COLS[x], LOW);
    
    for (y = 0; y < NUM_ROWS; y++) {
      if (bitRead(buffer[x], y) == 1) {
        digitalWrite(ROWS[y], HIGH);
      }
      else {
        digitalWrite(ROWS[y], LOW);
      }
    }
    digitalWrite(COLS[x], HIGH);
  }
}


void matrixClear(bool buff_en = true, bool bck_buff_en = false, bool draw_en = true) {
  bool d = draw_en;
  bool bck_b = bck_buff_en;
  bool b = buff_en;
  // If enabled, fill display buffer with 0s
  for (x = 0; x < NUM_COLS; x++) {
    if (b) {
      buffer[x] = 0;
    }
    // If enabled, clear back buffer
    if (bck_b) {
      b_buffer[x] = 0;
    }
  }
  // if enabled, draw to matrix
  if (d) {
    matrixDraw();
  }
}


/* TODO: bitwise-AND byte to check for equality (no change in byte),
and send flag to drawer to skip column */
void displayBufferAdd(int column, byte row_byte = 0, int buffer_select = 0) {
  // Insert row byte into specified column of display buffer
  int c = column;
  byte r_b = row_byte;
  // Buffers - 0: MAIN, 1: BACK, 2: BOTH
  int buf = buffer_select;
  
  switch (buf) {
    case 0:
      buffer[c] = r_b;
      break;
    
    case 1:
      b_buffer[c] = r_b;
      break;
    
    case 2:
      buffer[c] = r_b;
      b_buffer[c] = r_b;
      break;
  }
  
}

/*
void screenScroll(int direction = 0, int buffer = 0) {
  // Scroll the matrix display left (direction = 0), or right (direction = 1)
  // using selected buffer (0:MAIN, 1:BACK, 2:ALL)
  int dir = direction;
  int buf = buffer;
  int* b;
  
}
*/

void matrixDotTest(int speed = 150) {
  // Test all leds - one at a time, in sequence
  
  int s = speed;
  byte r_byte;
  
  matrixClear();
  
  for (x = 0; x < NUM_COLS; x++) {
    // Set row byte to 00000001, the first bit in the current column
    r_byte = 1;
    
    for (y = 0; y < NUM_ROWS; y++) {
      // Add the current row byte to the display buffer of col x and draw to the matrix
      displayBufferAdd(x, r_byte);
      matrixDraw();
      
      // Shift the byte left to 'walk' the lit led down the column
      r_byte = r_byte << 1;
      delay(s);
    }
    // Ensure the previous col is clear after last bit
    buffer[x] = 0;
  }
}


void setup() {
  // Setup pinouts - should work regardless of matrix size
  for (x = 0; x < NUM_COLS; x++) {
    pinMode(COLS[x], OUTPUT);
    digitalWrite(COLS[x], HIGH);
    for (y = 0; y < NUM_ROWS; y++) {
      pinMode(ROWS[y], OUTPUT);
      digitalWrite(ROWS[y], LOW);
    }
    buffer[x] = 0;
    b_buffer[x] = 0;
  }

}

// TODO: Experiment with draw. Do the LEDs turn off between draw calls or stay persistant until disabled?
void loop() {
		// The main show
  matrixDotTest(500);
  matrixClear();
  displayBufferAdd(0, 0b10000001);
  displayBufferAdd(7, 0b10000001);
  delay(3000);

}
