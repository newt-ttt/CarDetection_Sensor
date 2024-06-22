#include <WiFi101.h>
#include "Adafruit_VL53L1X.h"
#include "arduino_secrets.h" 

#define XSHUT_PIN 2
#define IRQ_PIN 3
#define TimingBudget 33// Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!

extern Adafruit_VL53L1X vl53;
extern WiFiClient client;

void wifi_setup();
void printWiFiStatus();
void sensor_setup();