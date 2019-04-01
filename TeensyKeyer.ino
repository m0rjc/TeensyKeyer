

int PIN_DIT = 3;
int PIN_DAH = 4;

int ditLengthMillis = 200;
int dahWeightTenths = 25;
int letterSpacingTenths = 30;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_DIT, INPUT_PULLUP);
  pinMode(PIN_DAH, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  sendLetter(B00000011); // M
  sendLetter(B00011111); // 0
  sendLetter(B11111010); // R
  sendLetter(B00001110); // J
  sendLetter(B11110101); // C
  sendEndCh();
  sendEndCh();
  sendEndCh();
}

void sendLetter(byte bitField) {
  while(bitField != 0 && bitField != 0xFF) {
    if(bitField & 1) {
      sendDah();
    } else {
      sendDit();
    }
    bitField = bitField >> 1;
    if(bitField & B01000000) bitField = bitField | B10000000;
  }
  sendEndCh();
}

void sendDit() {
  Serial.print('.');
  sendMark();
  delay(ditLengthMillis);
  sendSpace();
  delay(ditLengthMillis);
}

void sendDah() {
  Serial.print('-');
  sendMark();
  delay(ditLengthMillis * dahWeightTenths / 10);
  sendSpace();
  delay(ditLengthMillis);  
}

void sendEndCh() {
  Serial.print('/');
  sendSpace();
  delay(ditLengthMillis * dahWeightTenths / 10);    
}

void sendMark() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void sendSpace() {
  digitalWrite(LED_BUILTIN, LOW);
}
