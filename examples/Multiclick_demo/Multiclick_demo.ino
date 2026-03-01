#include <EMFButton.h>

EMFButton btn (2, HIGH_P);

void setup() {
  Serial.begin(9600);
}

void loop() {
  btn.tick();

  if (btn.isPressed())
    digitalWrite(13, 1);
  else
    digitalWrite(13, 0);

  if (btn.hasSingle())
    Serial.print(" Single");
  if (btn.hasDouble())
    Serial.print(" Double");
  if (btn.hasTriple())
    Serial.print(" Tripple");
  if (btn.isReleased() && btn.hasClicksNow())
    Serial.print(" " + String(btn.hasClicksNow()));
  if (btn.hasClicks())
    Serial.print(" Clicks:" + String(btn.hasClicks()) + "\n");
  if (btn.isHeld())
    Serial.print(" Clicks before holding:" + String(btn.hasClicksWithHeld()) + "\n");
}
