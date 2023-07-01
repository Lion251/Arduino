#define OutPin	1

void setup() {
	pinMode(OutPin, OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
	digitalWrite(OutPin, 1);
	delay(500);
	digitalWrite(OutPin, 0);
	delay(500);
}
