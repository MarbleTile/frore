
#include <Wire.h>

int led = LED_BUILTIN;
bool led_state = false;

void i2c_recv(int num) {
  while (Wire.available()) {
    Wire.read();
    led_state = !led_state;
    digitalWrite(led, led_state ? LOW : HIGH);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.onReceive(i2c_recv);
  Wire.begin(0x42, 5, 4, 100000);
}

void loop() {
}
