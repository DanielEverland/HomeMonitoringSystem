const int pResistor = D2;

int value;
void setup() {
  Serial.begin(115200);
  pinMode(pResistor, INPUT);
}

void loop() {
    value = analogRead(pResistor);
    Serial.println(value);
}
