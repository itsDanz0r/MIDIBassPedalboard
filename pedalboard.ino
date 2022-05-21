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
    int pin;
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

/*
Switch combos - just disorderly enough that they can't be assigned via simple loops. Joy.
C0 = 6, 1
C#0 = 7, 1
D0 = 6, 2
D#0 = 7, 2
E0 = 8, 2
F0 = 9, 2
F#0 = 6, 3
G0 = 7, 3
G#0 = 8, 3
A0 = 9, 3
A#0 = 10, 3
B0 = 6, 4
C1 = 7, 4
C#1 = 8, 4
D1 = 9, 4
D#1 = 10, 4
E1 = 6, 5
F1 = 7, 5
F#1 = 10, 5
G1 = 9, 5
*/

void setup() {
  Serial.begin(115200);                        // Start serial connection at std MIDI baud rate
  assign_notes();                              // Assign notes to all pedals
  pedals[0].pin = A0;
  pedals[0].power_pin = 6;
  pedals[1].pin = A0;
  pedals[1].power_pin = 7;
  pedals[2].pin = A1;
  pedals[2].power_pin = 6;
  pedals[3].pin = A1;
  pedals[3].power_pin = 7;
  pedals[4].pin = A1;
  pedals[4].power_pin = 8;
  pedals[5].pin = A1;
  pedals[5].power_pin = 9;
  pedals[6].pin = A2;
  pedals[6].power_pin = 6;
  pedals[7].pin = A2;
  pedals[7].power_pin = 7;
  pedals[8].pin = A2;
  pedals[8].power_pin = 8;
  pedals[9].pin = A2;
  pedals[9].power_pin = 9;
  pedals[10].pin = A2;
  pedals[10].power_pin = 10;
  pedals[11].pin = A3;
  pedals[11].power_pin = 6;
  pedals[12].pin = A3;
  pedals[12].power_pin = 7;
  pedals[13].pin = A3;
  pedals[13].power_pin = 8;
  pedals[14].pin = A3;
  pedals[14].power_pin = 9;
  pedals[15].pin = A3;
  pedals[15].power_pin = 10;
  pedals[16].pin = A4;
  pedals[16].power_pin = 6;
  pedals[17].pin = A4;
  pedals[17].power_pin = 7;
  pedals[18].pin = A4;
  pedals[18].power_pin = 10;
  pedals[19].pin = A4;
  pedals[19].power_pin = 9;

    
  for (int i = 1; i < 6; i++) {
    pinMode(i, INPUT);
  } 
  for (int i = 6; i < 11; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  } 
}

void assign_notes() {                        
  for (int i = 0; i < PEDAL_COUNT; i++) {
       pedals[i].note = i + 24;
       pedals[i].state = 0;                 // Make sure all pedals start in 'off' state
  }
}


void read_pedals(){
  for (int i = 0; i < PEDAL_COUNT; i++) {
    digitalWrite(pedals[i].power_pin, HIGH);
    bool pedal_turned_on = (analogRead(pedals[i].pin) > 700) && (pedals[i].state == 0);
    bool pedal_turned_off = (analogRead(pedals[i].pin) < 700) && (pedals[i].state == 1);
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
