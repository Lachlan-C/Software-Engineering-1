void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((digitalRead(6) == HIGH && digitalRead(7) == HIGH)||(digitalRead(7) == HIGH && digitalRead(8) == HIGH)||(digitalRead(6) == HIGH && digitalRead(8) == HIGH))
  {
    Serial.println("Alarm Active");
  }
  else {
    Serial.println("Alarm Inactive");
  }
  delay(500);
}
