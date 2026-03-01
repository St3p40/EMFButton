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

  if (btn.isClicked())
    Serial.print("Clicked ");

  if (btn.isReleased())
    Serial.println("Released:" + String(btn.holdinMillis()));

  if (btn.isHeld())
    Serial.print("Hold " + String(btn.hasClicksWithHeld()) + " ");

  if (btn.hasClicks())
    Serial.print("Clicks:" + String(btn.hasClicks()) + "\n");
}
