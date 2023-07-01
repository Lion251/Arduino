

#define OUT1	0
#define OUT2	1
#define OUT3	4

#define DELAY	100
#define AMPL	100

void setup() {
  // put your setup code here, to run once:

  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(OUT1, 50+random(AMPL));
  delay(random(DELAY));
  analogWrite(OUT2, 50+random(AMPL));
  delay(random(DELAY));
  analogWrite(OUT3, 50+ random(AMPL));
  delay(random(DELAY));
}
