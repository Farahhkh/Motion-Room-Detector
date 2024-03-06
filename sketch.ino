#include "DHT.h"

#define DHTPIN 3         // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22    // DHT 22 (AM2302), AM2321
#define PIRPIN 24        // Pin for the PIR sensor

DHT dht(DHTPIN, DHTTYPE);

int pirState = LOW;     // Assume no motion detected initially
int pirVal = 0;         // Variable for reading the PIR sensor status

void setup() {
    Serial.begin(9600);
    pinMode(PIRPIN, INPUT);  // Declare PIR sensor as input
    dht.begin();
}

void loop() {
    // Read PIR sensor
    pirVal = digitalRead(PIRPIN);

    // Read DHT22 sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
    } else {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C\t");
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");

        // Estimate air quality based on humidity
        String airQuality;
        if (humidity >= 40 && humidity <= 60) {
            airQuality = "Good";
        } else if (humidity < 40) {
            airQuality = "Dry";
        } else {
            airQuality = "Humid";
        }
        Serial.println("Air quality: " + airQuality);

        // Determine room comfort on a scale out of 5
        int comfortLevel = calculateComfortLevel(temperature, humidity);
        Serial.print("Comfort level (out of 5): ");
        Serial.println(comfortLevel);
    }

    // Check for motion detection
    if (pirVal == HIGH) {
        if (pirState == LOW) {
            Serial.println("Motion detected!");
            pirState = HIGH;
        }
    } else {
        if (pirState == HIGH) {
            Serial.println("Motion ended!");
            pirState = LOW;
        }
    }

    delay(1000); // Adjust delay as needed
}

int calculateComfortLevel(float temperature, float humidity) {
    // Map temperature to a scale of 1 to 5
    float tempScore = map(temperature, 18, 28, 1, 5); 

    // Map humidity to a scale of 1 to 5
    float humidityScore = map(humidity, 40, 60, 1, 5); 

    // Calculate average comfort level
    float comfortLevel = (tempScore + humidityScore) / 2.0;

    // Ensure comfort level does not exceed 5
    if (comfortLevel > 5) {
        comfortLevel = 5;
    }

    return int(comfortLevel); // Return as an integer
}
