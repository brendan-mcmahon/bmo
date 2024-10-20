#include <WiFi.h>

// Pin definitions
const int xAxisPin = 33;
const int yAxisPin = 35;
const int joyClickPin = 32;  // Joystick button pin
const int buttonPin = 21;    // Separate button pin

void setup() {
  Serial.begin(115200);  // USB serial connection
  
  pinMode(xAxisPin, INPUT);
  pinMode(yAxisPin, INPUT);
  pinMode(joyClickPin, INPUT_PULLUP);  // Joystick button (active low)
  pinMode(buttonPin, INPUT_PULLUP);    // Separate button (active low)

  Serial.println("Setup complete. Ready to send joystick and button data over USB.");
}

// Activation state variables
bool isDownActivated = false;
bool isLeftActivated = false;
bool isRightActivated = false;
bool isJoyClickActivated = false;  // Joystick click state
bool isButtonActivated = false;    // Separate button state

int downThreshold = 1700;
int leftThreshold = 2300;
int rightThreshold = 3300;
int deadZone = 500;

void loop() {
  int xValue = analogRead(xAxisPin);
  int yValue = analogRead(yAxisPin);

//  xValue = applyDeadZone(xValue, 2993);
//  yValue = applyDeadZone(yValue, 2993);
//  Serial.print("(");
//  Serial.print(xValue);
//  Serial.print(", ");
//  Serial.print(yValue);
//  Serial.print(")");
//  Serial.println();

  // Check joystick directions
  checkDownPosition(yValue);
  checkLeftPosition(xValue);
  checkRightPosition(xValue);

  // Check joystick click and separate button
  checkJoystickClick();
  checkButton();

  delay(50);
}

int applyDeadZone(int value, int midpoint) {
  if (abs(value - midpoint) < deadZone) {
    Serial.println("neutral");
    return midpoint;
  }
  return value;
}

// Check joystick "down" direction
void checkDownPosition(int value) {
  if (isDownActivated && value >= downThreshold) {
    isDownActivated = false;
//    Serial.print("down released: ");
//    Serial.print(value);
//    Serial.println();
    Serial.println("1:UP");  // Send "down released" signal over USB
  }
  if (!isDownActivated && value < downThreshold) {
    isDownActivated = true;
//    Serial.print("down activated: ");
//    Serial.print(value);
//    Serial.println();
    Serial.println("1:DOWN");  // Send "down activated" signal over USB
  }
}

// Check joystick "left" direction
void checkLeftPosition(int value) {
  if (isLeftActivated && value > leftThreshold) {
    isLeftActivated = false;
//    Serial.print("left released: ");
//    Serial.print(value);
//    Serial.print(" > ");
//    Serial.print(leftThreshold);
//    Serial.println();
    Serial.println("6:UP");  // Send "left released" signal over USB
  }
  if (!isLeftActivated && value <= leftThreshold) {
    isLeftActivated = true;
//    Serial.print("left activated: ");
//    Serial.print(value);    
//    Serial.print(" <= ");
//    Serial.print(leftThreshold);
//    Serial.println();
    Serial.println("6:DOWN");  // Send "left activated" signal over USB
  }
}

// Check joystick "right" direction
void checkRightPosition(int value) {
  if (isRightActivated && value < rightThreshold) {
    isRightActivated = false;
//    Serial.print("right released: ");
//    Serial.print(value);
//    Serial.println();
    Serial.println("5:UP");  // Send "right released" signal over USB
  }
  if (!isRightActivated && value >= rightThreshold) {
    isRightActivated = true;
//    Serial.print("right activated: ");
//    Serial.print(value);
//    Serial.println();
    Serial.println("5:DOWN");  // Send "right activated" signal over USB
  }
}

// Check joystick click button (pin 32)
void checkJoystickClick() {
  int joyClickState = digitalRead(joyClickPin);
  if (isJoyClickActivated && joyClickState == HIGH) {
    isJoyClickActivated = false;
//    Serial.println("joystick click released");
    Serial.println("3:UP");  // Send "joystick click released" signal over USB
  }
  if (!isJoyClickActivated && joyClickState == LOW) {
    isJoyClickActivated = true;
//    Serial.println("joystick click activated");
    Serial.println("3:DOWN");  // Send "joystick click pressed" signal over USB
  }
}

// Check separate button (pin 21)
void checkButton() {
  int buttonState = digitalRead(buttonPin);
  if (isButtonActivated && buttonState == HIGH) {
    isButtonActivated = false;
//    Serial.println("button released");
    Serial.println("2:UP");  // Send "button released" signal over USB
  }
  if (!isButtonActivated && buttonState == LOW) {
    isButtonActivated = true;
//    Serial.println("button activated");
    Serial.println("2:DOWN");  // Send "button pressed" signal over USB
  }
}
