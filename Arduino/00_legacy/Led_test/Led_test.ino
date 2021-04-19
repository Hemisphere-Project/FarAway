
// BLINK
const byte led_pin = 12;
void setup() {
  pinMode(led_pin, OUTPUT);

  Serial.begin(115200);
}

void loop() {

  digitalWrite(led_pin,HIGH);
  delay(200);
  digitalWrite(led_pin,LOW);
  delay(200);
}


// PWM
// const byte pwm_pin = 12;
// int value = 0;
// int inc = 10;
// int val = 0;
//
// void setup() {
//   ledcAttachPin(pwm_pin, 0);
//
//   // ledcSetup(pwmChannel, frequence, resolution);
//   ledcSetup(0, 24000, 8); // 22 kHz PWM, 8-bit resolution -- >20kHz, -> pwm noise inaudible
//   Serial.begin(115200);
// }
//
// void loop() {
//
//
//   Serial.println(value);
//   value = value + inc;
//   if (value <= 0 || value >= 250) {
//     inc = -inc;
//   }
//
//   ledcWrite(0, value);
//   delay(50);
// }
