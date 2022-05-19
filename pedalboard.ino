const int NOTE_ON = 144;
const int NOTE_OFF = 128;
const int PEDAL_COUNT = 2;

int velocity = 100;


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
  // Start serial connection at std MIDI baud rate
  Serial.begin(31250);

  // Loop through each Pedal object in the array and assign sequential pins
  for (int i = 0; i < PEDAL_COUNT; i++) {
     pedals[i].pin = i+2;
     pedals[i].note = i+24;
     pedals[i].state = 0;
  }  
}

void check_velocity() {
  int pot_value = map(analogRead(A0), 0, 1023, 0, 127);
  velocity = pot_value;
}

void read_pedals(){

  for (int i = 0; i < PEDAL_COUNT; i++) {
    if (
         ((digitalRead(pedals[i].pin) == LOW) && (pedals[i].state == 1)) 
         || 
         ((digitalRead(pedals[i].pin) == HIGH) && (pedals[i].state == 0))
       ) {
      pedals[i].changeState();
      delay(3);
    }
  }
}

void loop() {
  check_velocity();
  read_pedals();
}
