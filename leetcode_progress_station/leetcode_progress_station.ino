#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// WiFi credentials
const char* ssid = "Barnard Guest"; // WiFi Name
const char* password = ""; // WiFi Password

// API URL
const String baseUrl = "https://leetcode-api-faisalshohag.vercel.app/";
String leetcodeUsername = "PowerYang";
String apiUrl = baseUrl + leetcodeUsername;

// Buttons
#define BUTTON_LEFT 0
#define BUTTON_RIGHT 35

// Define OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create the OLED object
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// Globals
int currentScreen = 1; 
JSONVar apiResponse;

// Time zone (0 for GMT)
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

// Display setup
TFT_eSPI tft = TFT_eSPI();

// Color Palette
uint16_t blueColor = tft.color565(0x2d, 0x8e, 0xff);
uint16_t greenColor = tft.color565(0x1c, 0xbb, 0xba); 
uint16_t orangeColor = tft.color565(0xff, 0xb7, 0x03); 
uint16_t redColor = tft.color565(0xf5, 0x38, 0x37);
uint16_t brightGreenColor = tft.color565(0x01, 0xc1, 0x02);
uint16_t greyColor = tft.color565(0xf5, 0xf5, 0xf5); 

void fetchLeetCodeData() {
  HTTPClient http;
  http.begin(apiUrl);

  int httpResponseCode = http.GET();

  if (httpResponseCode == 200) {
    String response = http.getString();
    apiResponse = JSON.parse(response);

    if (JSON.typeof(apiResponse) == "undefined") {
      Serial.println("Error parsing API response");
    }
  } else {
    Serial.print("Error on HTTP request: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void displayTotalSolved() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(50, 10);
  tft.print("Total Solved");

  tft.setTextSize(4);
  tft.setTextFont(4); 
  tft.setTextColor(blueColor, TFT_BLACK);
  tft.setCursor(35, 35);
  tft.print((int)apiResponse["totalSolved"]);
}

void displayDifficultyBreakdown() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(1);
  tft.setTextSize(3);
  
  // Easy
  tft.setTextColor(greenColor, TFT_BLACK);
  tft.setCursor(10, 10);
  tft.print("Easy: ");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println((int)apiResponse["easySolved"]);

  // Medium
  tft.setTextColor(orangeColor, TFT_BLACK);
  tft.setCursor(10, 55);
  tft.print("Medium: ");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println((int)apiResponse["mediumSolved"]);

  // Hard
  tft.setTextColor(redColor, TFT_BLACK);
  tft.setCursor(10, 100);
  tft.print("Hard: ");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println((int)apiResponse["hardSolved"]);
}

int getTodayMidnightTimestamp() {
    // Get the current time
    time_t now;
    struct tm timeInfo;
    time(&now);                  // Get current time as a UNIX timestamp
    gmtime_r(&now, &timeInfo);   // Convert to GMT (UTC) time structure
    
    // Set time to midnight
    timeInfo.tm_hour = 0;
    timeInfo.tm_min = 0;
    timeInfo.tm_sec = 0;
    
    // Convert back to UNIX timestamp
    return mktime(&timeInfo);
}

void displaySubmissionHeatMap() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 20);
  tft.print("14 Days Streak");

  JSONVar submissionCalendar = apiResponse["submissionCalendar"];
  int heatmapSize = 14;
  int squareSize = 27;
  int padding = 5;
  int startX = 10;
  int startY = 50;

  int midnightTimestamp = getTodayMidnightTimestamp();
  Serial.println("Today's midnight timestamp (GMT): " + String(midnightTimestamp));
  for (int i = 0; i < heatmapSize; i++) {
    // Calculate timestamp for each day in the past 14 days
    int dayTimestamp = midnightTimestamp - (13 - i) * 86400;
    String dayKey = String(dayTimestamp);

    // Check if the day has submissions
    int submissions = submissionCalendar.hasOwnProperty(dayKey) ? (int)submissionCalendar[dayKey] : 0;

    // Determine square color
    uint16_t color = submissions > 0 ? brightGreenColor : greyColor;

    // Draw the square (positioning logic remains the same)
    int row = i / 7; 
    int col = i % 7;
    int x = startX + col * (squareSize + padding);
    int y = startY + row * (squareSize + padding);
    tft.fillRoundRect(x, y, squareSize, squareSize, 5, color);
  }
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  // Start I2C communication for OLED
  Wire.begin(21, 22); // SDA = 21, SCL = 22
  
  // Initialize the OLED display
  if (!display.begin(0x3C)) { // 0x3C is the default I2C address for SH110X
    Serial.println(F("SH110X allocation failed"));
    for (;;);
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  // Configure time
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");

  // Wait for time to be set
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)) {
      Serial.println("Failed to obtain time");
      return;
  }

  // Display first screen
  drawScreen(currentScreen);

  // Display OLED screen
  drawOLEDScreen();

  // Button interrupts
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
}

void drawScreen(int screenNumber) {
  if (screenNumber == 1) {
    fetchLeetCodeData();
    displayTotalSolved();
  } else if (screenNumber == 2) {
    displayDifficultyBreakdown();
  } else if (screenNumber == 3) {
    displaySubmissionHeatMap();
  }
}

// OLED screen to display a random problem
void drawOLEDScreen() {
  if (JSON.typeof(apiResponse["recentSubmissions"]) == "undefined" || apiResponse["recentSubmissions"].length() == 0) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("No recent submissions.");
    display.display();
    return;
  }

  // Pick a random problem from recentSubmissions
  int submissionCount = apiResponse["recentSubmissions"].length();
  int randomIndex = random(0, submissionCount);
  String problemTitle = (const char*)apiResponse["recentSubmissions"][randomIndex]["title"];

  // Clear OLED and display the question
  display.clearDisplay();
  display.setTextSize(1);            
  display.setTextColor(SH110X_WHITE);

  display.setCursor(0, 0);
  display.print("void recallRecent() {");

  display.setCursor(0, 20);
  display.print("problem = \"" + problemTitle + "\";");

  display.setCursor(0, 55);
  display.print("}");

  display.display();
}

void loop() {
  if (digitalRead(BUTTON_LEFT) == LOW) {
    // Wait for button debounce
    delay(200);
    drawOLEDScreen(); // Redraw OLED with a new random problem
  }

  if (digitalRead(BUTTON_RIGHT) == LOW) {
    // Wait for button debounce
    delay(200);
    currentScreen = (currentScreen % 3) + 1; // Cycles through 1, 2, 3, 1...
    drawScreen(currentScreen);
  }
}
