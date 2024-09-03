#include <Arduino.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <AltSoftSerial.h>
#include <TinyGPS++.h>


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
// Initialize software serial for SIM800L
SoftwareSerial sim800l(2, 3); // RX, TX
// SIM800L connections
// RX to pin 2 (Arduino TX)
// TX to pin 3 (Arduino RX

// GPS connections
// RX to pin 5
// TX to pin 6
AltSoftSerial gpsSerial; // RX, TX

// Initialize TinyGPS++ for GPS data parsing
TinyGPSPlus gps;

// Define the relay pin
const int relayPin = 4;

// Define the keymap for the keypad
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Keypad row and column pins
byte rowPins[ROWS] = {5, 6, 7, 10};
byte colPins[COLS] = {11, 12, 13, A0};

// Create the Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Predefined password
const char correctPassword[5] = "1212";
char enteredPassword[5];
byte index = 0;
bool passwordCorrect = false;

//function declaration 
void getPassword();
void checkForSMS();
void sendLocation();
void sendSMS(String number, String text);


void setup() {
  // Initialize hardware serial communication
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  // Initialize software serial communication with SIM800L
  sim800l.begin(9600);
  // Initialize AltSoftSerial communication with GPS module
  gpsSerial.begin(9600);
  // Initialize relay pin as output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Start with relay off

  delay(5000);
  lcd.print(".....SYSTEM......");
  lcd.setCursor(0, 1);
  lcd.print("....BOOTING......");
  delay(2000);
  lcd.clear();
  Serial.println("Starting");

  // Test communication with SIM800L
  lcd.clear();
  sim800l.println("AT");
  lcd.print(sim800l.println("AT"));
  delay(1000);
  if (sim800l.available()) {
    Serial.println(sim800l.readString());
    lcd.print(sim800l.readString());
  }

  // Set SMS mode to text mode
  lcd.clear();
  sim800l.println("AT+CMGF=1");
  lcd.print(sim800l.println("AT+CMGF=1"));
  delay(1000);
  if (sim800l.available()) {
    Serial.println(sim800l.readString());
    lcd.print(sim800l.readString());
  }

  // Enable SMS notifications
  lcd.clear();
  sim800l.println("AT+CNMI=1,2,0,0,0");
  lcd.println("AT+CNMI=1,2,0,0,0");
  delay(1000);
  if (sim800l.available()) {
    Serial.println(sim800l.readString());
    lcd.print(sim800l.readString());
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SMART ANTI-THEFT WITH");
  lcd.setCursor(0, 1);
  lcd.print("2FA AUTHENTICATION");
  delay(2000);
  lcd.clear();
  Serial.println("ending");

  // Prompt for password
  Serial.println("Enter your password:");
  lcd.setCursor(0, 0);
  lcd.print("ENTER YOUR");
  lcd.setCursor(0, 1);
  lcd.print("PASSCODE ");
  delay(5000);
  lcd.clear();
}

void loop() {
  if (!passwordCorrect) {
    getPassword();
  } else {
    checkForSMS();
  }
}

void getPassword() {
  char key = keypad.getKey();

  if (key) {
    if (key == '*' || key == '#') {
      // Ignore special keys
      return;
    }
    lcd.clear();
    Serial.print("*"); // For debugging
    lcd.setCursor(index, 1);
    lcd.print("*");
    enteredPassword[index] = key;
    index++;

    if (index == 4) {
      enteredPassword[index] = '\0'; // Null terminate the string
      Serial.println(enteredPassword);
      Serial.println();

      if (strcmp(enteredPassword, correctPassword) == 0) {
        Serial.println("Password is correct!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PASSCODE");
        lcd.setCursor(0, 1);
        lcd.print("IS RIGHT");
        delay(1000);
        passwordCorrect = true;
        Serial.println("Password correct. Listening for SMS commands...");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Waiting for SMS");
      } else {
        Serial.println("Password is incorrect!");
        lcd.setCursor(0, 0);
        lcd.print("PASSWORD IS");
        lcd.setCursor(0, 1);
        lcd.print("INCORRECT");
        delay(2000); // Display result for 2 seconds
        lcd.setCursor(0, 0);
        lcd.print("ENTER YOUR");
        lcd.setCursor(0, 1);
        lcd.print("PASSWORD");
        Serial.println("Enter your password:");
        index = 0; // Reset the index for the next password
      }
    }
  }
}

void checkForSMS() {
  if (sim800l.available()) {
    String sms = sim800l.readString();
    Serial.println(sms);

    if (sms.indexOf("LOCATION") >= 0) {
      sendLocation();
      Serial.println("location sent");
    }

    //    if (sms.indexOf("ON") >= 0) {
    //      // Turn on the relay
    //      digitalWrite(relayPin, HIGH);
    //      sendSMS("+2348144836070", "Relay is ON");
    //    }
    //
    //    if (sms.indexOf("OFF") >= 0) {
    //      // Turn off the relay
    //      digitalWrite(relayPin, LOW);
    //      sendSMS("+2348144836070", "Relay is OFF");
    //    }
  }
}

void sendLocation() {
  Serial.println("Reading GPS...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reading GPS LOCATION..");

  // Wait for GPS data to be available
  unsigned long start = millis();
  while (millis() - start < 10000) { // Wait for up to 10 seconds
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }

    if (gps.location.isUpdated()) {
      break;
    }
  }

  if (gps.location.isValid()) {
    String latitude = String(gps.location.lat(), 6);
    String longitude = String(gps.location.lng(), 6);
    String message = "Latitude: " + latitude + " Longitude: " + longitude + " http://maps.google.com/?q=" + latitude + "," + longitude;

    // Send SMS via SIM800L
    sendSMS("+2348107128670", message);
    delay(2000);
    sendSMS("+2347055977529", message);
    Serial.println("Message sent!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Message sent!");
    Serial.println("Message sent!");
    sendSMS("+2348144836070", message);
  } else {
    Serial.println("GPS Timeout");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GPS Timeout");
  }
}

void sendSMS(String number, String text) {
  sim800l.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  sim800l.println("AT+CMGS=\"" + number + "\""); // Specify the recipient
  delay(1000);
  sim800l.println(text); // The message content
  delay(1000);
  sim800l.write(26); // ASCII code for Ctrl+Z to send the SMS
  delay(5000);

  if (sim800l.available()) {
    Serial.println(sim800l.readString());
  }
}
