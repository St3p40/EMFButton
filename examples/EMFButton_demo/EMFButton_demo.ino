#include "EMFButton.h"

EMFButton btn (3, 0);

void setup() {
  Serial.begin(9600);
}

void loop() {
  btn.tick();

  if (btn.isPressed())
    digitalWrite(13, 1);
  else
    digitalWrite(13, 0);

  if (btn.isClicked())
    Serial.print("Clicked ");
  
  if (btn.isHeld())
    Serial.print("Hold " + String(btn.hasClicksWithHeld()) + "\n");
  
  if (btn.hasClicks())
    Serial.print("Clicks:" + String(btn.hasClicks()) + "\n");
}
