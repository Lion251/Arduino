void setup() {
  // put your setup code here, to run once:
	DDRB	= 0xFF;
}

void loop() {
  // put your main code here, to run repeatedly:
	PORTB	= 0;
	delay(100);
	PORTB	= 0xFF;
	delay(100);
}
