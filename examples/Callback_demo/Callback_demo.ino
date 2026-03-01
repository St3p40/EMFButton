#define EMFB_USE_CALLBACKS

#include <EMFButton.h>

EMFButton btn (2, HIGH_P);

void call_clicked() {
  Serial.println("Clicked");
}
void call_held() {
  Serial.println("Held");
}
void call_released() {
  Serial.println("Released");
}
void call_endclicks() {
  Serial.println("Click strike is ended");
}

void setup() {
  Serial.begin(9600);
  btn.attach(C_CLICKED, call_clicked);
  btn.attach(C_HELD, call_held);
  btn.attach(C_RELEASED, call_released);
  btn.attach(C_ENDCLICKS, call_endclicks);
}

void loop() {
  btn.tick();

  if (btn.isPressed())
    digitalWrite(13, 1);
  else
    digitalWrite(13, 0);

}