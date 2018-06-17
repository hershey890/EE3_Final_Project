#define PT 3
#define MOTOR 6

/* LED locations */
#define LEDb 9 // blue
#define LEDg 8 // green
#define LEDr 7 // red


/*initial speed*/
unsigned char inputSpeed = 150;
unsigned int initial_counter = 0;
unsigned char output_speed = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PT, INPUT);
  pinMode(MOTOR, OUTPUT);

  digitalWrite(LEDg, LOW);
  digitalWrite(LEDr, LOW);
  digitalWrite(LEDb, LOW);
  analogWrite(MOTOR, inputSpeed);
}

bool stopped = false;
int count = 0;

void loop() {
  unsigned char reading_high = 0;
  unsigned char reading_low = 255;
  
  

  for (int i = 0; i < 200; i++) {
    reading_high = analogRead(PT) > reading_high ? analogRead(PT) : reading_high;
    if (analogRead(PT) < reading_low && analogRead(PT) != 0) {
      reading_low = analogRead(PT);
    }
    //reading_low = analogRead(PT) < reading_low ? analogRead(PT) : reading_low;
    //analogWrite(MOTOR, inputSpeed);
  }

    
  /*Serial.print(inputSpeed);
  Serial.print(" ");
  Serial.print(reading_high);
  Serial.print(" ");
  Serial.println(reading_low);*/
  
  if (reading_low < 20 && reading_high > 80)
    stopped = false;
  
  if (!(reading_low < 20 && reading_high > 80)) {// && !stopped) {
    count++;
    //Serial.println("Success");
    stopped = true;
    for (int i = 0; i < 15; i++) {
      digitalWrite(LEDg, HIGH);
      digitalWrite(LEDr, HIGH);
      digitalWrite(LEDb, HIGH);
      delay(100);
      digitalWrite(LEDg, LOW);
      digitalWrite(LEDr, LOW);
      digitalWrite(LEDb, LOW);
      delay(100);
    }
  }

  if(stopped) {
    inputSpeed -= 5;
    inputSpeed -= (inputSpeed > 5) ? 5 : 0; 
    //analogWrite(MOTOR, inputSpeed);
  }

  analogWrite(MOTOR, inputSpeed);
}

/*class delayOnce {
public:
  delayOnce() {}
  void delayOnce(void) {
    if(count == 0) {
      delay(200);
    }
    count++;
    return;
  }
private:
  byte count = 0;
};*/

