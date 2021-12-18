// Makefile:
/*
miru: miru.ino
  arduino-cli compile -b arduino:avr:uno
  sudo arduino-cli upload -p /dev/ttyACM0 -b arduino:avr:uno
*/

// Libreria
#include <Servo.h>

// Entradi
#define kippSchalterUpPin 5
#define kippSchalterDownPin 4
#define endSchalterUpPin 7
#define endSchalterDownPin 6
#define remoteControlPin 3
#define switchInterval 3000

// Üscida dal servo
#define servoPin 9

// Veocità dal servo, 1 => lent, 90 => svelt (negatif al gira ala altra)
#define servoSpeed 90
static_assert(servoSpeed >= -90 && servoSpeed <= 90, "Velocità invalida");

Servo servo;

void setup() {
  // Pizza al servo => al fa forza
  servo.attach(servoPin);

  // Cun la modalità INPUT_PULLUP la entrada le ativa a 0
  pinMode(kippSchalterUpPin,   INPUT_PULLUP);
  pinMode(kippSchalterDownPin, INPUT_PULLUP);
  pinMode(endSchalterUpPin,    INPUT_PULLUP);
  pinMode(endSchalterDownPin,  INPUT_PULLUP);
  pinMode(remoteControlPin,    INPUT_PULLUP);
}

int goUp = 0;
int goDown = 0;
unsigned long lastSwitch = 0;
int lastDirection = 0;

void loop() {
  if (!digitalRead(remoteControlPin)) {
    unsigned long now = millis();

    if (now - lastSwitch > switchInterval) {
      lastSwitch = now;
      goUp = lastDirection;
      goDown = !lastDirection;
      lastDirection = !lastDirection;
    }
  }


  // Modalità manuale
  if (!digitalRead(kippSchalterUpPin)) {
    goUp = 1;
    goDown = 0;
  }

  if (!digitalRead(kippSchalterDownPin)) {
    goUp = 0;
    goDown = 1;
  }

  // Controla i endschalter
  if (!digitalRead(endSchalterUpPin)) {
    goUp = 0;
  }

  if (!digitalRead(endSchalterDownPin)) {
    goDown = 0;
  }

  if (goUp) {
    servo.write(90 + servoSpeed);
    return;
  }

  if (goDown) {
    servo.write(90 - servoSpeed);
    return;
  }

  // Bloca al servo in pusizion
  servo.write(94);
}
