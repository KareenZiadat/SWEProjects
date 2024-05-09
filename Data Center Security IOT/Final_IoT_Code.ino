/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6n_6mq8xh"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "uIGIzVS0rpau6VfNx-cEfILpdbyPfIrX"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Keypad.h>
#include <Servo.h>

// WiFi credentials.
char ssid[] = "BA.Fiber_EXT";
char pass[] = "0777509503";

Servo servoMotor;
BlynkTimer timer;
const byte ROWS = 1; 
const byte COLS = 3; // Three columns
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'}
};
byte rowPins[ROWS] = {D1}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {D2, D3, D4}; // Connect to the column pinouts of the keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
String password = "1111";
String input = "";
int trials = 0;
const int maxTrials = 3;
int buzzerPin = D0;
int servoPin = D8;
const int trigPin = D5;
const int echoPin = D6;
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701
long duration;
float distanceCm;
float distanceInch;

enum State {
  ENTER_PASSWORD,
  CHECK_PASSWORD,
  CORRECT_PASSWORD,
  WRONG_PASSWORD,
  LOCKED
};

State currentState = ENTER_PASSWORD;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  Blynk.virtualWrite(V2, millis() / 1000);
}


void setup()
{
  Blynk.virtualWrite(V5, LOW); // Set the pin initially to LOW state (buzzer off)

  pinMode(buzzerPin, OUTPUT);
  // Debug console
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  Serial.println("Enter Password");
  servoMotor.attach(servoPin);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop()
{
  Blynk.run();
  timer.run();
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  // Prints the distance on the Serial Monitor
  if (distanceCm < 10) {
    Serial.println("door was opened");
    Blynk.logEvent("Warning", "Someone enetred .");
    Blynk.virtualWrite(4, "Yes"); // Write "door open" to V4 on Blynk app
  }
  else{
        Blynk.virtualWrite(4, "No"); // Write "door open" to V4 on Blynk app
  }
    char customKey = customKeypad.getKey();
  if (currentState == ENTER_PASSWORD) {
    if (customKey) {
      if (input.length() < 4) {
        Serial.print(input);
        input += customKey;
      }
    }
    if (input.length() == 4) {
      currentState = CHECK_PASSWORD;
    }
  }
  if (currentState == CHECK_PASSWORD) {
    if (password == input) {
      Serial.println("\nCorrect Password");
      currentState = CORRECT_PASSWORD;
    } else {
      Serial.println("\nWrong Passcode");
      trials++;
      Serial.println("Trials: " + String(trials));
      input = "";
      if (trials >= maxTrials) {
        currentState = LOCKED;
      } else {
        currentState = WRONG_PASSWORD;
      }
    }
  }
  if (currentState == CORRECT_PASSWORD) {
    openDoor();
    delay(7000);
    closeDoor();
    delay(200000000);
  }
  if (currentState == WRONG_PASSWORD) {
    delay(1000);
    Serial.println("\nEnter Passcode:");
    currentState = ENTER_PASSWORD;
  }
  if (currentState == LOCKED) {
    Blynk.logEvent("intrusion_alret", "There is a possible intruder.");
    lock();
  }
}
void lock() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    delay(500);
  }
}
void unlock() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, LOW);
    delay(7000);
  }
}
void openDoor() {
  servoMotor.write(0);
}
void closeDoor() {
  servoMotor.write(90);
}

BLYNK_WRITE(V5) {
  int buttonState = param.asInt();
  if (buttonState == HIGH) {
    lock();
  } else {
    unlock();
  }
}
