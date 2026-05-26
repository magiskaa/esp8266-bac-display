// Pin definitions
const int digitPins[] = {13, 10, 9, 7}; // Digits 1, 2, 3, 4
const int segmentPins[] = {12, 8, 5, 3, 2, 11, 6, 4}; // A, B, C, D, E, F, G

// 7-segment patterns for digits 0-9 (common cathode)
const byte digitPatterns[10] = {
  0b11111100, // 0 - segments A,B,C,D,E,F
  0b01100000, // 1 - segments B,C
  0b11011010, // 2 - segments A,B,D,E,G
  0b11110010, // 3 - segments A,B,C,D,G
  0b01100110, // 4 - segments B,C,F,G
  0b10110110, // 5 - segments A,C,D,F,G
  0b10111110, // 6 - segments A,C,D,E,F,G
  0b11100000, // 7 - segments A,B,C
  0b11111110, // 8 - segments A,B,C,D,E,F,G
  0b11110110  // 9 - segments A,B,C,D,F,G
};

char displayBuffer[5] = "0000";
char inputBuffer[5];
byte inputPos = 0;


void setup() {
  // Initialize segment pins as outputs
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW);
  }

  // Initialize digit pins as outputs
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH); // Turn off all digits (common cathode)
  }

  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c >= '0' && c <= '9') {
      if (inputPos < 4) {
        inputBuffer[inputPos++] = c;
      }
    }
    else if (c == '\n' || c == '\r') {
      if (inputPos == 4) {
        for (int i = 0; i < 4; i++) {
          displayBuffer[i] = inputBuffer[i];
        }
        displayBuffer[4] = '\0';
      }
      inputPos = 0;
    }
    else {
      inputPos = 0;
    }
  }
  displayDigit();
}

void displayDigit() {
  for (int digitIndex = 0; digitIndex < 4; digitIndex++) {
    // turn all digits off
    for (int i = 0; i < 4; i++) {
      digitalWrite(digitPins[i], HIGH);
    }

    char c = displayBuffer[digitIndex];
    if (c >= '0' && c <= '9') {
      byte pattern = digitPatterns[c - '0'];

      // turn decimal point on only for first digit
      if (digitIndex == 0) {
        pattern |= 0b00000001;
      }

      setSegments(pattern);
    } else {
      setSegments(0);
    }

    // turn this digit on
    digitalWrite(digitPins[digitIndex], LOW);

    delay(2);
  }
}

void setSegments(byte pattern) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], (pattern >> (7 - i)) & 0x01);
  }
}
