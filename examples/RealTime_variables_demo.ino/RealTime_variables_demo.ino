#include <EMFButton.h>

EMFButton btn (2, HIGH_P);

void setup() {
  Serial.begin(112500);
}

void loop() {
  btn.tick();

  if (btn.isPressed())
    digitalWrite(13, 1);
  else
    digitalWrite(13, 0);

    Serial.print("Clicks: " + String(btn.hasClicksNow()) + "\t");
    Serial.print("Before hold: " + String(btn.hasClicksWithHold()) + "\t");
    Serial.print("Press Time: " + String(btn.holdingTime()) + "\n");
}