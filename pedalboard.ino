const int PEDAL_COUNT = 2;
const int NOTE_ON = 144;
const int NOTE_OFF = 128;
int velocity = 100;
int octave = 0;

class Pedal {
  private:
    int _pin;
    bool _state;
    int _note;
    
  public:
    int pin;
    bool state;
    int note;
    
  Pedal(int _pin = 0, bool _state = 0, int _note = 0) {
    pin = _pin;
    state = _state;
    note = _note;
  };
  
  void changeState() {
    state = !state;
    if (state == 1) {
      //Serial.write(NOTE_ON);
      //Serial.write(note);
      //Serial.write(velocity);
      Serial.println(NOTE_ON);
      Serial.println(note);
      Serial.println(velocity);
    }
    else if (state == 0) {
      //Serial.write(NOTE_OFF);
      //Serial.write(note);
      //Serial.write(velocity);
      Serial.println(NOTE_OFF);
      Serial.println(note);
      Serial.println(velocity);
    }
  };
};

Pedal pedals[PEDAL_COUNT];

void setup() {
  Serial.begin(31250);                        // Start serial connection at std MIDI baud rate
  assign_notes();                             // Assign notes to all pedals
  for (int i = 0; i < PEDAL_COUNT; i++) {     // Loop through each pedal object in the array and assign sequential pins
     pedals[i].pin = i+2;
  }  
}

void assign_notes() {                         // Assign notes to all pedals according to current octave
  for (int i = 0; i < PEDAL_COUNT; i++) {
       pedals[i].note = i + 24 + (octave * 12);
  }
}

void check_velocity() {
  int pot_value = map(analogRead(A0), 0, 1023, 0, 127);
  velocity = pot_value;
}

void check_octave() {
  int pot_value = map(analogRead(A1), 0, 1023, 0, 6);
  if (octave != pot_value) {
    octave = pot_value;
    assign_notes();
  }
}

void read_pedals(){
  for (int i = 0; i < PEDAL_COUNT; i++) {
    bool pedal_turned_on = (digitalRead(pedals[i].pin) == HIGH) && (pedals[i].state == 0);
    bool pedal_turned_off = (digitalRead(pedals[i].pin) == LOW) && (pedals[i].state == 1);
    
    if (pedal_turned_on || pedal_turned_off){
      pedals[i].changeState();
      delay(3);
    }
  }
}

void loop() {
  check_velocity();
  check_octave();
  read_pedals();
}
