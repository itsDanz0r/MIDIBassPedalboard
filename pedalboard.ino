/*
MIDI BASS PEDALBOARD
YAMAHA ELECTONE EL-25 HACK
Danny "DENNY" Kerr 2022
*/

const int PEDAL_COUNT = 20;
const int NOTE_ON = 144;
const int NOTE_OFF = 128;
int velocity = 100;

class Pedal {
    
  public:
    int read_pin;
    int power_pin;
    bool state;
    int note;
    
  Pedal() {
  };
  
  void changeState() {
    state = !state;
    if (state == 1) {
      Serial.write(NOTE_ON);
      Serial.write(note);
      Serial.write(velocity);
    }
    else if (state == 0) {
      Serial.write(NOTE_OFF);
      Serial.write(note);
      Serial.write(velocity);
    }
  };
};

Pedal pedals[PEDAL_COUNT];

void setup() {                                       
  assign_notes();                             
  set_pin_modes();
  assign_pins();
  Serial.begin(115200);                     // Start serial connection at standard MIDI baudrate
}

void assign_pins(){
  byte pedal_pins[20][2] = {                // {Read, Power}
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
  for (int i = 6; i < 11; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void assign_notes() {                        
  for (int i = 0; i < PEDAL_COUNT; i++) {
       pedals[i].note = i + 24;
       pedals[i].state = 0;
  }
}


void read_pedals(){
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
