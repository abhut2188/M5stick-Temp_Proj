#include <M5StickCPlus.h>
#include "M5_ENV.h"
#include <Wire.h>
#include <ESPAsyncWebSrv.h>

SHT3X sht30;
QMP6988 qmp6988;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

AsyncWebServer server(80);

const char *ssid = "Galaxys22";
const char *password = "maxi2016";

void setup() {
    M5.begin();
    M5.Lcd.setRotation(3);
    Wire.begin(0, 26);

    // Connect to Wi-Fi
    connectToWiFi();

    qmp6988.init();
    sht30.init();
    M5.lcd.println(F("ENVIII Hat(SHT30 and QMP6988) test"));

    // Route to handle root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = "<html><body>";

        // Display temperature in a rectangle with color based on temperature
        html += "<div style='background-color:" + String(getTemperatureColor(tmp), HEX) + "; padding: 250px;'>";
        html += "<p>Ambient Temperature: " + String(tmp, 1) + " &deg;C</p>";
        html += "</div>";

        // Display humidity in a rectangle
        html += "<div style='background-color:#ADD8E6; padding: 10px;'>";  // Use hex color for LIGHTBLUE
        html += "<p>Humidity: " + String(hum, 0) + " %</p>";
        html += "</div>";

        // Display pressure in a rectangle
        html += "<div style='background-color:#90EE90; padding: 10px;'>";  // Use hex color for LIGHTGREEN
        html += "<p>Pressure: " + String(pressure, 0) + " Pa</p>";
        html += "</div>";

        html += "</body></html>";
        request->send(200, "text/html", html);
    });

    server.begin();
}

void loop() {
    // Check Wi-Fi connection
    if (WiFi.status() != WL_CONNECTED) {
        M5.Lcd.println("Not connected to Wi-Fi");
        connectToWiFi();
    } else {
        // Display Wi-Fi connection status
        M5.Lcd.println("Connected to Wi-Fi");
        M5.Lcd.println("");

        // Update sensor values
        pressure = qmp6988.calcPressure();
        if (sht30.get() == 0) {
            tmp = sht30.cTemp;
            hum = sht30.humidity;
        } else {
            tmp = 0, hum = 0;
        }

        // Display sensor values on the screen
        displaySensorValues();
    }

    delay(2000);
}

void connectToWiFi() {
    M5.Lcd.println("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        M5.Lcd.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        M5.Lcd.println("Connected to Wi-Fi");
    } else {
        M5.Lcd.println("Failed to connect to Wi-Fi");
    }
}

void displaySensorValues() {
    // Clear the screen
    M5.Lcd.fillScreen(BLACK);

    // Display temperature in a rectangle with color based on temperature
    M5.Lcd.fillRect(10, 10, 140, 60, getTemperatureColor(tmp));
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(20, 20);
    M5.Lcd.printf("Temp: %2.1f C", tmp);

    // Display humidity in a rectangle
    M5.Lcd.fillRect(10, 80, 140, 60, 0xADD8E6);  // Use hex color for LIGHTBLUE
    M5.Lcd.setCursor(20, 90);
    M5.Lcd.printf("Humi: %2.0f%%", hum);

    // Display pressure in a rectangle
    M5.Lcd.fillRect(10, 150, 140, 60, 0x90EE90);  // Use hex color for LIGHTGREEN
    M5.Lcd.setCursor(20, 160);
    M5.Lcd.printf("Pressure: %2.0f Pa", pressure);
}

uint32_t getTemperatureColor(float temperature) {
    // Change color based on temperature range
    float temperatureRange = temperature;
    
    if (temperatureRange >= 0 && temperatureRange <= 2)
        return 0x6495ED;   // CornflowerBlue
    else if (temperatureRange >= 3 && temperatureRange <= 5)
        return 0x87CEEB;   // SkyBlue
    else if (temperatureRange >= 6 && temperatureRange <= 8)
        return 0x00BFFF;   // DeepSkyBlue
    else if (temperatureRange >= 9 && temperatureRange <= 11)
        return 0x1E90FF;  // DodgerBlue
    else if (temperatureRange >= 12 && temperatureRange <= 14)
        return 0x4169E1; // RoyalBlue
    else if (temperatureRange >= 15 && temperatureRange <= 17)
        return 0x0000FF; // Blue
    else if (temperatureRange >= 18 && temperatureRange <= 20)
        return 0x0000CD; // MediumBlue
    else if (temperatureRange >= 21 && temperatureRange < 22)
        return 0x000080; // Navy
    else if (temperatureRange >= 22 && temperatureRange < 23)
        return 0x008080; // Teal
    else if (temperatureRange >= 23 && temperatureRange < 24)
        return 0x90EE90; // LightGreen
    else if (temperatureRange >= 24 && temperatureRange < 25)
        return 0x87CEEB;   // SkyBlue
    else if (temperatureRange >= 25 && temperatureRange < 26)
        return 0x32CD32; // LimeGreen
    else if (temperatureRange >= 26 && temperatureRange < 27)
        return 0x7CFC00; // LawnGreen
    else if (temperatureRange >= 27 && temperatureRange < 28)
        return 0xADFF2F; // GreenYellow
    else if (temperatureRange >= 28 && temperatureRange < 29)
        return 0xFFD700; // Gold
    else if (temperatureRange >= 29 && temperatureRange < 30)
        return 0xFF6347; // LightRed
    // Add more cases as needed for other temperature ranges
    else
        return 0xFF0000; // Red for unknown or high temperatures
}




