/*
MIDI BASS PEDALBOARD
YAMAHA ELECTONE EL-25 HACK
Danny "DENNY" Kerr 2022
*/

const int PEDAL_COUNT = 20;
const int NOTE_ON = 144;
const int NOTE_OFF = 128;
const int VELOCITY = 100;


class Pedal {
// Main class for each pedal. Tracks the pedals read pin, power pin, currently saved state (on or off) and assigned note
    
  public:
    int read_pin;
    int power_pin;
    bool state;
    int note;
    
  Pedal() {
  };
  
  void changeState() {
  // Send bytes over serial when pedal state changes
    state = !state;
    if (state == 1) {
      Serial.write(NOTE_ON);
      Serial.write(note);
      Serial.write(VELOCITY);
    }
    else if (state == 0) {
      Serial.write(NOTE_OFF);
      Serial.write(note);
      Serial.write(VELOCITY);
    }
  };
};


Pedal pedals[PEDAL_COUNT];

void assign_pins(){
// Assign the appropriate power and read pins to each pedal.
// The original circuitry doesn't use all 25 combinations for this purpose and a couple are out of note sequence, so this needs to be handled manually.
  byte pedal_pins[20][2] = { // {Read, Power}
    {A0, 6}, {A0, 7}, {A1, 6}, {A1, 7},
    {A1, 8}, {A1, 9}, {A2, 6}, {A2, 7},
    {A2, 8}, {A2, 9}, {A2, 10}, {A3, 6},
    {A3, 7}, {A3, 8}, {A3, 9}, {A3, 10},
    {A4, 6}, {A4, 7}, {A4, 10}, {A4, 9}
  };
  for (int i = 0; i < PEDAL_COUNT; i++) {
    pedals[i].read_pin = pedal_pins[i][0];
    pedals[i].power_pin = pedal_pins[i][1];
  }
}

void set_pin_modes() {
// Set all five power pins to output and digital low
  for (int i = 6; i < 11; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void assign_notes() {
// Assign sequential notes to all pedals
  for (int i = 0; i < PEDAL_COUNT; i++) {
       pedals[i].note = i + 24;
       pedals[i].state = 0;
  }
}

void setup() {                                       
  assign_notes();                             
  set_pin_modes();
  assign_pins();
  Serial.begin(115200);   // Start serial connection at standard MIDI baudrate
}

void read_pedals(){
// Loop through each pedal, setting its power pin to high and then seeing if there is a mismatch between its saved state and the current reading
// If so, change the pedals state. Slight delay added for debouncing.
  for (int i = 0; i < PEDAL_COUNT; i++) {
    digitalWrite(pedals[i].power_pin, HIGH);
    bool pedal_turned_on = (analogRead(pedals[i].read_pin) > 700) && (pedals[i].state == 0);
    bool pedal_turned_off = (analogRead(pedals[i].read_pin) < 700) && (pedals[i].state == 1);
      if (pedal_turned_on || pedal_turned_off){
        pedals[i].changeState();
        delay(3);      
      }
    digitalWrite(pedals[i].power_pin, LOW);
  }
}

void loop() {
  read_pedals();
}
