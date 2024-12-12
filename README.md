# LeetCode Progress Display Station

<img width="1192" alt="Screenshot 2024-12-11 at 17 18 36" src="https://github.com/user-attachments/assets/d5372182-8aa8-4a7d-89e3-160d6d0f4542" />

This project creates a **LeetCode Station** using the **ESP32 TTGO T-Display** and an **OLED screen** to display personalized LeetCode statistics fetched from a public API. The display offers multiple interactive screens showcasing your problem-solving achievements and encourages you to recall solutions to recently solved problems.

## 🚀 Features

1. **Three Interactive Screens** on the TTGO T-Display:
    - **Total Solved Problems**: Displays the total number of problems you've solved.
    - **Difficulty Breakdown**: Shows the number of easy, medium, and hard problems you've solved.
    - **Submission Heatmap**: Visualizes your activity over the past 14 days with a grid of green and grey squares.
2. **OLED Prompt for Solution Recall**:
    - Randomly displays a title from your recent accepted submissions.
3. **Button Navigation**:
    - **Right Button**: Cycles through the three main screens.
    - **Left Button**: Displays a new random problem title on the OLED screen.

    ![C0242](https://github.com/user-attachments/assets/1bf1ea4f-c0f8-40ad-8834-2a3370e468a6)


## 🛠️ Hardware Requirements

- ESP32 TTGO T-Display
- 0.96" or 1.3" 4-Pin OLED Display (SH1106/SSD1306)
- 4 Female-Female Jumper Wires
- USB Cable for Programming
- Stainless Steel Wire 14 Gauge + Soldering wire (or other thinner jewelry wire) for Enclosure

## 📡 API

This project fetches data from the following public API by Faisal Shohag:

```
https://leetcode-api-faisalshohag.vercel.app/<your-username>
```

GitHub Repo: https://github.com/faisal-shohag/leetcode_api

The API provides information such as:

- Total problems solved
- Difficulty breakdown (Easy, Medium, Hard)
- Recent submissions with problem titles and statuses
- Submission calendar

## 🔧 Setup Instructions

1. **Wiring the OLED**:
    - SDA (OLED) to Pin 21 (ESP32)
    - SCL (OLED) to Pin 22 (ESP32)
    - GND (OLED) to G (ESP32)
    - VCC (OLED) to 3.3V (ESP32)
<img width="500" src="https://github.com/user-attachments/assets/76a13bd1-f8f7-45e7-8ed3-78acbe5bc510" />

2. **Install Dependencies**:
    - Install the following libraries in the Arduino IDE:
        - `TFT_eSPI` (for TTGO T-Display)
        - `WiFi` (for ESP32 Wi-Fi connection)
        - `HTTPClient` (for HTTP requests)
        - `Arduino_JSON` (for parsing JSON data)
        - `Adafruit_GFX` and `Adafruit_SH110X` (for OLED display)
3. **Update Wi-Fi Credentials**:
Replace the placeholders in the code with your Wi-Fi credentials:
    
    ```cpp
    const char* ssid = "YOUR_WIFI_SSID"; // Line 10
    const char* password = "YOUR_WIFI_PASSWORD"; // Line 11
    ```
    
4. **Set Your LeetCode Username**:
Update the `leetcodeUsername` variable:
    
    ```cpp
    String leetcodeUsername = "YourLeetCodeUsername"; // Line 15
    ```
    
5. **Upload the Code**:
    - Connect the ESP32 TTGO T-Display to your computer.
    - Select the appropriate board and port in the Arduino IDE.
    - Upload the code.
    
6. **Enclosure**
    - The wire frame was inspired by [Mohit Bhoite’s OLED Terminal](https://www.bhoite.com/sculptures/oled-terminal/).
    - See the [blog post](https://www.notion.so/vanessazhangyq/LeetCode-Progress-Display-Station-14ae96c0c3ee80ddbcdcfa32fcdfd578?pvs=4#159e96c0c3ee80b7bf82ca968ec81873) for detailed steps for creating the wire frame
  

## 🖥️ How It Works

- **On Boot**: The ESP32 connects to Wi-Fi and fetches LeetCode data from the API.
- **TTGO T-Display**:
    - Displays the three screens based on `currentScreen`.
    - Use the **right button** to navigate through the screens.
    - Each button pressed draws a new screen based on the `currentScreen` value.
- **OLED Display**:
    - Shows a random prompt resembling a code snippet from your recent accepted submissions.
    - Use the **left button** to generate a new random prompt.

happy leetcoding :p
 

