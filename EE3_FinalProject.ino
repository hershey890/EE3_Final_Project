#define MOTOR1 5
#define MOTOR2 6
#define RLED 7



void setup()
{
	Serial.begin(9600);
	analogWrite(MOTOR1, LOW);
	analogWrite(MOTOR2, LOW);
}

void loop()
{
	analogWrite(MOTOR1, 120);
	analogWrite(MOTOR2, 120);
	digitalWrite(RLED, HIGH);
}
