#include <Arduino.h>
#include "bigUmbrella.cpp"

BigUmbrella* bu;

void setup() {
  Serial.begin(115200);
  bu = new BigUmbrella();
}

void loop() { bu->tick(); }