#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <SPI.h>
#include <RFID.h>

#define SS_PIN 20
#define RST_PIN 49
RFID rfid(SS_PIN, RST_PIN);

String rfidCard;

const int ROW_NUM = 4;
const int COLUMN_NUM = 4;

const int SPEAKER_PIN = 39;
Servo myservo;

char keys[ROW_NUM][COLUMN_NUM] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
byte pin_rows[ROW_NUM] = { 22, 24, 26, 28 };
byte pin_column[COLUMN_NUM] = { 30, 32, 34, 36 }; 

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

class User {
public:
  String name;
  String password;
  String rfid;

  User(String n, String pw, String r)
    : name(n), password(pw), rfid(r) {}
};

const int MAX_USERS = 2;
User users[MAX_USERS] = {
  User("User1", "1111", "217 5 247 151"),
  User("User2", "2222", "38 220 43 31"),
};

String input_password;

bool isTonePlayed = false;

void playTone(int frequency, int duration) {
  tone(SPEAKER_PIN, frequency, duration);
  delay(duration);
  noTone(SPEAKER_PIN);
}

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(SPEAKER_PIN, OUTPUT);  
  input_password.reserve(4); 
  SPI.begin();
  rfid.init();
}

void processPassword() {
  bool accessGranted = false;
  String username;

  for (int i = 0; i < MAX_USERS; ++i) {
    if (users[i].password == input_password) {
      accessGranted = true;
      username = users[i].name;
      break;
    }
  }

  if (accessGranted) {
    lcd.setCursor(0, 1);
    lcd.print("Hello, " + username);
    playTone(1000, 250);
    myservo.write(180);
    delay(3000);
    lcd.clear();
    myservo.write(0);
  } else {
    lcd.print("Incorrect");
    playTone(100, 250);
    delay(3000);
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }

  input_password = "";
}

void scanRFIDCard() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
      lcd.setCursor(0, 1);
      lcd.print(rfidCard);
      delay(3000);
      lcd.clear();
    }
    rfid.halt();
  }
}

void processRFID() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);

      bool accessGranted = false;
      String username;

      for (int i = 0; i < MAX_USERS; ++i) {
        if (users[i].rfid == rfidCard) {
          accessGranted = true;
          lcd.setCursor(0, 1);
          lcd.print("Hello, " + users[i].name);
          playTone(1000, 250);
          delay(2000);
          lcd.clear();
          break;
        }
      }

      if (!accessGranted) {
        lcd.setCursor(0, 1);
        lcd.print("Incorrect");
        playTone(100, 250);
        delay(3000);
        lcd.clear();
      }
    }

    rfid.halt();
  }
}

void loop() {
  char key = keypad.getKey();
  lcd.setCursor(0, 0);
  lcd.print("Enter password:");

  if (key) {
    lcd.setCursor(0, 1);
    playTone(500, 100);
    if (key == 'D') {
      processPassword();
    } else if (key == '*') {
      input_password = "";
      lcd.setCursor(0, 1);
      lcd.clear();
      playTone(750, 50);
    } else if (key == 'C' || key == 'A' || key == 'B' || key == '#') {
      if (input_password.length() > 0) {
        input_password.remove(input_password.length() - 1);
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(input_password);
      }
    } else {
      input_password += key;
      if (input_password.length() <= 16) {
        lcd.print(input_password);
      } else {
        input_password = input_password.substring(1);
        lcd.print(input_password);
      }
    }
  }
  //scanRFIDCard();
  processRFID();
}
