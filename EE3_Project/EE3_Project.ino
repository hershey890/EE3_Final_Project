/* LED locations */
const int LEDb = 9; // blue
const int LEDg = 8; // green
const int LEDr = 7; // red

/* phototransistors */
const int PTl = 0; // left
const int PTr = 1; // right
const int PTs = 2; // straight

/* motors */
const int Mr = 6; // right
const int Ml = 5; // left

/* calibrated constants */
unsigned int PTr_calibrated = 0;
unsigned int PTl_calibrated = 0;
unsigned int PTs_calibrated = 0;

/* Motor speed */
const int right_fast_speed = 25;
const int right_slow_speed = 230;
const int left_fast_speed = 40;
const int left_slow_speed = 230;
const int straight_offset = 0; //should be a negative value
bool readyToStop = false;

/* Reading High/Low Values */
const int l_black = 145;
const int l_white = 447;
const int s_black = 570;
const int s_white = 961;
const int r_black = 450;
const int r_white = 959;
const float white_black_diff = 0.8;
const float stop_white_black_diff = 0.6;

/* Right and Left */
bool prev_right = false;
bool prev_left = false;

void setup() {
  Serial.begin(9600);

  //delay(1500);
  
  pinMode(LEDg, OUTPUT);
  pinMode(LEDb, OUTPUT);
  pinMode(LEDr, OUTPUT);
  pinMode(PTr, INPUT);
  pinMode(PTl, INPUT);
  pinMode(PTs, INPUT);
  pinMode(Mr, OUTPUT);
  pinMode(Ml, OUTPUT);
  /* calibration of IR sensors */
  for (int i = 0; i < 50; i++) {
    if(analogRead(PTr) < 0) {
      i--;
      continue;
    }
    PTr_calibrated += analogRead(PTr);
  }

  for (int i = 0; i < 50; i++) {
    if(analogRead(PTl) < 0) {
      i--;
      continue;
    }
    PTl_calibrated += analogRead(PTl);
  }

  for (int i = 0; i < 50; i++) {
      if(analogRead(PTs) < 0) {
        i--;
        continue;
      }
    PTs_calibrated += analogRead(PTs);
  }

  PTr_calibrated /= 50;
  PTl_calibrated /= 50;
  PTs_calibrated /= 50;

//  if (PTr_calibrated < 900)
//    PTr_calibrated -= 100;
//  if (PTl_calibrated < 900)
//    PTl_calibrated -= 100;
//  if (PTs_calibrated > 100)
//    PTs_calibrated += 100;
  
  /*Serial.print("RC ");
  Serial.print(PTr_calibrated);
  Serial.print(" SC ");
  Serial.print(PTs_calibrated);
  Serial.print(" LC "); 
  Serial.println(PTl_calibrated);
  */
  analogWrite(Mr, right_fast_speed);
}

void loop() {
  /*int PTr_reading = map(analogRead(PTr), 0, PTr_calibrated, 0, 1023);
  int PTl_reading = map(analogRead(PTl), 0, PTl_calibrated, 0, 1023);
  int PTs_reading = map(analogRead(PTs), PTs_calibrated, 1023, 0, 1023);*/
  int PTr_reading = analogRead(PTr);
  int PTl_reading = analogRead(PTl);
  int PTs_reading = analogRead(PTs);

//Serial.print(" l ");
//Serial.print(analogRead(PTl));
//Serial.print(" s ");
//Serial.print(analogRead(PTs));
//Serial.print(" r ");
//Serial.println(analogRead(PTr));
/*
  Serial.print(" l ");
  Serial.print(PTl_reading);
  Serial.print(" C ");
  Serial.print(PTl_calibrated);
  Serial.print(" s ");
  Serial.print(PTs_reading);
  Serial.print(" C ");
  Serial.print(PTs_calibrated);
  Serial.print(" r ");
  Serial.print(PTr_reading);
  Serial.print(" C ");
  Serial.println(PTr_calibrated);
*/
    /* Stop */
  if (PTl_reading < PTl_calibrated * stop_white_black_diff && 
      PTr_reading < PTr_calibrated * stop_white_black_diff && 
      PTs_reading < PTs_calibrated / stop_white_black_diff) {
    analogWrite(Mr, 255); //turn off
    analogWrite(Ml, 255); //turn off
    while(true) {
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

  /* Turning Right - RED */
  else if (PTr_reading < PTr_calibrated * white_black_diff) {
    prev_right = true;
    prev_left = false;
    digitalWrite(LEDg, LOW);
    digitalWrite(LEDb, LOW);
    digitalWrite(LEDr, HIGH);
    //Serial.println("RIGHT");
    analogWrite(Mr, right_slow_speed);
    analogWrite(Ml, left_fast_speed);
  }

  /* Turning Left - BLUE */
  else if (PTl_reading < PTl_calibrated * white_black_diff) {
    prev_right = false;
    prev_left = true;
    digitalWrite(LEDr, LOW);
    digitalWrite(LEDg, LOW);
    digitalWrite(LEDb, HIGH);
    //Serial.println("LEFT");
    analogWrite(Mr, right_fast_speed);
    analogWrite(Ml, left_slow_speed);
  }

  /* Straight - GREEN */
  else if (PTs_reading < PTs_calibrated / white_black_diff) {
    prev_right = false;
    prev_left = false;
    digitalWrite(LEDr, LOW);
    digitalWrite(LEDb, LOW);
    digitalWrite(LEDg, HIGH);
    //Serial.println("STRAIGHT");
    analogWrite(Mr, right_fast_speed);
    analogWrite(Ml, left_fast_speed);
  }

//
//  /* Straight - GREEN */
//  else {// (PTs_reading < PTs_calibrated + white_black_diff) {
//    digitalWrite(LEDr, LOW);
//    digitalWrite(LEDb, LOW);
//    digitalWrite(LEDg, HIGH);
//    Serial.println("STRAIGHT");
//    analogWrite(Mr, fast_speed + straight_offset);
//    analogWrite(Ml, fast_speed + straight_offset);
//  }
  /*else if (prev_right) {
    digitalWrite(LEDg, LOW);
    digitalWrite(LEDb, LOW);
    digitalWrite(LEDr, HIGH);
    //Serial.println("RIGHT");
    analogWrite(Ml, fast_speed);
    analogWrite(Mr, slow_speed);
    
  }
  else if (prev_left) {
    digitalWrite(LEDr, LOW);
    digitalWrite(LEDg, LOW);
    digitalWrite(LEDb, HIGH);
    //Serial.println("LEFT");
    analogWrite(Mr, fast_speed);
    analogWrite(Ml, slow_speed);
  }
  else {
    prev_right = false;
    prev_left = false;
    digitalWrite(LEDr, LOW);
    digitalWrite(LEDb, LOW);
    digitalWrite(LEDg, HIGH);
    //Serial.println("STRAIGHT");
    analogWrite(Mr, fast_speed);
    analogWrite(Ml, fast_speed);
  }*/
//  else {
//    digitalWrite(LEDg, LOW);
//    digitalWrite(LEDr, LOW);
//    digitalWrite(LEDb, LOW);
//  }
}
