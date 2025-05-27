#include <Keypad.h>
#include <HID-Project.h>
#include <HID-Settings.h>

const byte ROWS = 3;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  { '1', '4', '7' },
  { '2', '5', '8' },
  { '3', '6', '9' }
};

byte rowPins[ROWS] = { 2, 3, 4 };
byte colPins[COLS] = { 6, 7, 8 };

const int encoderAPin = 16;
const int encoderBPin = 14;
const int encoderSWPin = 15;

bool shortcutMode = false;
bool lastButtonState = HIGH;
int lastEncoderA = HIGH;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  BootKeyboard.begin();   // HID-Project keyboard
  Consumer.begin();       // Media keys

  pinMode(encoderAPin, INPUT_PULLUP);
  pinMode(encoderBPin, INPUT_PULLUP);
  pinMode(encoderSWPin, INPUT_PULLUP);
}

void loop() {
  // Encoder button toggles shortcut mode
  bool buttonState = digitalRead(encoderSWPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    shortcutMode = !shortcutMode;
    Serial.print("Shortcut Mode: ");
    Serial.println(shortcutMode ? "ON" : "OFF");
    delay(200);  // Debounce
  }
  lastButtonState = buttonState;

  // Encoder rotation detection
  int encoderA = digitalRead(encoderAPin);
  int encoderB = digitalRead(encoderBPin);

  if (encoderA != lastEncoderA) {
    if (encoderA == LOW) {
      if (encoderB == HIGH) {
        Serial.println("Volume Up");
        Consumer.write(MEDIA_VOLUME_UP);
      } else {
        Serial.println("Volume Down");
        Consumer.write(MEDIA_VOLUME_DOWN);
      }
    }
  }
  lastEncoderA = encoderA;

  // Keypad input
  char key = keypad.getKey();
  if (key) {
    Serial.print("Key Pressed: ");
    Serial.println(key);

    if (shortcutMode) {
      switch (key) {
        case '1':  // Space
          BootKeyboard.write(' ');
          break;

        case '2':  // Arrow up
          BootKeyboard.press(KEY_UP_ARROW);
          delay(100);
          BootKeyboard.releaseAll();
          break;

        case '3':  // Backspace
          BootKeyboard.press(KEY_BACKSPACE);
          delay(100);
          BootKeyboard.releaseAll();
          break;

        case '4':  // Arrow left
          BootKeyboard.press(KEY_LEFT_ARROW);
          delay(100);
          BootKeyboard.releaseAll();
          break;

        case '5':  // Arrow down
          BootKeyboard.press(KEY_DOWN_ARROW);
          delay(100);
          BootKeyboard.releaseAll();
          break;

        case '6':  // Arrow right
          BootKeyboard.press(KEY_RIGHT_ARROW);
          delay(100);
          BootKeyboard.releaseAll();
          break;

        case '7':  // Ctrl+C
          BootKeyboard.press(KEY_LEFT_CTRL);
          BootKeyboard.press('c');
          delay(100);
          BootKeyboard.releaseAll();
          break;

        case '8':  // Ctrl+V
          BootKeyboard.press(KEY_LEFT_CTRL);
          BootKeyboard.press('v');
          delay(100);
          BootKeyboard.releaseAll();
          break;

        case '9':  // Ctrl+Z
          BootKeyboard.press(KEY_LEFT_CTRL);
          BootKeyboard.press('z');
          delay(100);
          BootKeyboard.releaseAll();
          break;

        default:
          BootKeyboard.write(key);  // Fallback
          break;
      }
    } else {
      BootKeyboard.write(key);  // Normal typing
    }

    delay(200);  // Basic debounce
  }
}
