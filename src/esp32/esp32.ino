#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal.h>

// Wi-Fi
const char* ssid = "OPPO";
const char* password = "16062003";
const char* serverUrl = "http://192.168.155.20:3000/data";


// LCD setup
const int rs = 18, en = 19, d4 = 21, d5 = 22, d6 = 23, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//user array
#define currentUser "mariaabreu"


const int led = 25;
const int button = 26;
const int buzzerPin = 33;

const byte sensorPin = 27;
const byte sensorInterrupt = digitalPinToInterrupt(sensorPin);

float calibrationFactor = 4.5;

volatile unsigned int pulseCount;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;

bool buttonState = HIGH;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long lastClickTime = 0;
const unsigned long doubleClickDelay = 500;
const unsigned long resetDelay = 5000;

bool countdownMode = false;
const int limitLitres = 2;
const float ledThresholdPercent = 0.8;

unsigned long lastFlowTime = 0;
unsigned long lastSendTime = 0;

#define BUZZER_CHANNEL 0

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}
bool sessionSent = false;
struct BathSession {
  unsigned long timestamp;
  unsigned int volumeML;
};

const int MAX_SESSIONS = 20;
BathSession sessions[MAX_SESSIONS];
int sessionCount = 0;


void setup() {
  Serial.begin(115200);

  // LCD setup
  lcd.begin(16, 2);
  lcd.print("Flow Sensor");
  delay(1500);
  lcd.clear();

  // Sensor setup
  pinMode(sensorPin, INPUT_PULLUP);
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

  // Input/Output setup
  pinMode(button, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Wi-Fi setup
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  oldTime = millis();
  lastFlowTime = millis();
  lastSendTime = millis();

  HTTPClient http;
http.begin("http://192.168.155.20:3000/ping"); // your PC's IP
int httpCode = http.GET();
Serial.print("Ping GET Response Code: ");
Serial.println(httpCode);
http.end();

}

void loop() {
  handleButton();

  if ((millis() - oldTime) > 1000) {
    detachInterrupt(sensorInterrupt);

    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();

    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;

    float totalLitres = totalMilliLitres / 1000.0;
    float displayLitres = countdownMode ? (limitLitres - totalLitres) : totalLitres;

    if (flowRate > 0) {
      lastFlowTime = millis();
      sessionSent = false;  // Reset flag when flow resumes
    }

    if (!sessionSent && millis() - lastFlowTime > resetDelay && totalMilliLitres >= 300) {
      sendBathSession(totalMilliLitres);
      sessionSent = true;
      totalMilliLitres = 0;
      beep(100, 50);
    }


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(countdownMode ? "Restante:" : "Total:");
    lcd.print(displayLitres, 1);
    lcd.print(" L");

    lcd.setCursor(0, 1);
    lcd.print("Media:");
    lcd.print(calculateAverageLitres(), 1);
    lcd.print("L");


    float percent = totalLitres / limitLitres;

    if (percent >= ledThresholdPercent) {
      digitalWrite(led, HIGH);
      ledcAttachPin(buzzerPin, BUZZER_CHANNEL);
      ledcWriteTone(BUZZER_CHANNEL, 2000);
      delay(1000);
      ledcWriteTone(BUZZER_CHANNEL, 0);
    } else {
      digitalWrite(led, LOW);
    }

    pulseCount = 0;
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

    // Send to server every 5s
    if (millis() - lastSendTime >= 5000) {
      lastSendTime = millis();
    }
  }
}

void handleButton() {
  bool reading = digitalRead(button);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        if (millis() - lastClickTime < doubleClickDelay) {
          totalMilliLitres = 0;
          Serial.println("Counter reset.");
        } else {
          countdownMode = !countdownMode;
          Serial.print("Mode: ");
          Serial.println(countdownMode ? "Countdown" : "Total");
        }
        lastClickTime = millis();
      }
    }
  }

  lastButtonState = reading;
}

void beep(int freq, int duration) {
  ledcAttachPin(buzzerPin, BUZZER_CHANNEL);
  ledcWriteTone(BUZZER_CHANNEL, freq);
  delay(duration);
  ledcWriteTone(BUZZER_CHANNEL, 0);
}

void sendFlowData(float litres) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.setTimeout(1000);
    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/json");

    String json =
      "{\"userId\":\"" currentUser "\","
      "\"litres\":"
      + String(litres, 2) + ","
      "\"timestamp\":"
      + String(millis()) + "}";
    Serial.print("Sending: ");
    Serial.println(json);

    int response = http.POST(json);
    Serial.print("Server response: ");
    Serial.println(response);
    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}
void sendBathSession(unsigned int volumeML) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.setTimeout(1000);
    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/json");

    String json = 
  "{\"userId\":\"" + String(currentUser) + "\"," +
  "\"volume\":" + String(volumeML / 1000.0, 2) + 
  ",\"timestamp\":" + String(millis()) + "}";



    Serial.print("Sending bath session: " + json);

    int response = http.POST(json);
    Serial.print("Server response: ");
    Serial.println(response);

    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}

float calculateAverageLitres() {
  if (sessionCount == 0) return 0;

  unsigned long totalML = 0;
  for (int i = 0; i < sessionCount; i++) {
    totalML += sessions[i].volumeML;
  }

  return totalML / 1000.0 / sessionCount;
}
