// Sleuteldoosje op Doorduino
// Revspace.nl - revspace.nl/sleutelkastje
//
// 12v, GND en Strike in gebruik van Doorduino kabel
// Strike op pin 17 (A3)
// Mosfet 1 op 18 (A4)
// Mosfet 2 op 19 (A5)
// iButtonreader direct op doorduino aangesloten
// Standaard mosfets, IRL520, 530 of 540 zijn meer dan genoeg.

#define doorduino 17
#define reedrelay 3
#define close_delay 1000
#define solenoidout 19
#define solenoidin 18

bool door_is_locked;

void setup() {
  pinMode(doorduino, INPUT_PULLUP);
  pinMode(solenoidout, OUTPUT);
  pinMode(solenoidin, OUTPUT);
  pinMode(reedrelay, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  door_is_locked = true;  // anders weigert unlockdoor
  unlockdoor(); // zodat de deur altijd bij boot even open gaat. Handig als de boel borkt!
}

void loop() {
  static bool state = 1;
  digitalWrite(LED_BUILTIN, digitalRead(reedrelay));

  bool newstate = digitalRead(doorduino);
  if (newstate != state) {
    state = newstate;

    if (state == 0) {
      unlockdoor();
    }
  }

  if (state == 1) {
    unsigned long start = millis();
    unsigned long elapsed = 0;
    while (digitalRead(reedrelay) == 0 && elapsed < close_delay) {
      elapsed = millis() - start;
    }
    Serial.println(elapsed);
    if (elapsed >= close_delay) {
      lockdoor();
    }
  }
  
  if (digitalRead(reedrelay)) {
    // unexpected open door, apparently not locked
    door_is_locked = false;
  }
  
}

void unlockdoor() {
  if (! door_is_locked) return;
  digitalWrite(solenoidout, HIGH);
  delay(50);
  digitalWrite(solenoidout, LOW);
  delay(50);
  door_is_locked = false;
}

void lockdoor() {
  if (door_is_locked) return;
  digitalWrite(solenoidin, HIGH);
  delay(50);
  digitalWrite(solenoidin, LOW);
  delay(50);
  door_is_locked = true;
}

