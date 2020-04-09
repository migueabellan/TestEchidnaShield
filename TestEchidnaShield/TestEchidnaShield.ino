const int MAX_SENS = 27;
String result[MAX_SENS];
int index = 0;

void setup() {
  Serial.begin(9600);

  printInitSens();

  // Buttons
  testButton(2, "SR D2");
  testButton(3, "SL D3");
  testButton(2, "Joystick SJ D2");

  // LEDS
  testLed(6, "RGB BD6 (Azul)");
  testLed(5, "RGB GD5 (Verde)");
  testLed(9, "RGB RD9 (Rojo)");
  testLed(13, "LED D13 (Rojo)");
  testLed(12, "LED D12 (Naranja)");
  testLed(11, "LED D11 (Verde)");

  // Buzzer
  testBuzzer(10, "Buzzer D10");

  // LDR
  testLdr(5, "LDR A5");

  // Joystick
  testJoystick(0, 0, "Joystick A0 (Izquierda)");
  testJoystick(0, 1, "Joystick A0 (Derecha)");
  testJoystick(1, 1, "Joystick A1 (Arriba)");
  testJoystick(1, 0, "Joystick A1 (Abajo)");

  // Acelerometro
  testAccel(2, 0, "Acel A2X (Izquierda)");
  testAccel(2, 1, "Acel A2X (Derecha)");
  testAccel(3, 1, "Acel A3Y (Adelante)");
  testAccel(3, 0, "Acel A3Y (Atrás)");

  printInitMkmk();
  
  // Mkmk
  testMkmk(0, 0, "Mkmk A0");
  testMkmk(1, 0, "Mkmk A1");
  testMkmk(2, 0, "Mkmk A2");
  testMkmk(3, 0, "Mkmk A3");
  testMkmk(4, 0, "Mkmk A4");
  testMkmk(5, 0, "Mkmk A5");
  testMkmk(2, 1, "Mkmk D2");
  testMkmk(3, 1, "Mkmk D3");

  printResult();
}

void loop() {
}


void testButton(int pin, String text) {
  pinMode(pin, INPUT);
  
  unsigned long time = millis();
  int timeout = 5000;
  bool isTimeout = false;
  
  Serial.println("Pulsa el botón " + text);

  while(digitalRead(pin) == LOW) { 
    if(millis() - time > timeout) {
      saveResult(false, text);
      isTimeout = true;
      break;
    }  
  }
  
  if(!isTimeout) saveResult(true, text);
}

void testLed(int pin, String text) {
  pinMode(pin, OUTPUT);
  
  Serial.println("¿Está encendido el " + text + "?");

  analogWrite(pin, 255);
  saveResult(isCorrect(), text);
  analogWrite(pin, 0);
}

void testBuzzer(int pin, String text) {
  pinMode(pin, OUTPUT);
  
  Serial.println("¿Ha sonado el " + text + "?");

  for(int i=1; i<10; i++) {
    tone(pin, 1000*i, 100);
    delay(200);
  }
  saveResult(isCorrect(), text);
}

void testLdr(int pin, String text) {
  unsigned long time = millis();
  int timeout = 5000;
  bool isTimeout = false;
  
  Serial.println("Tapa el " + text);

  while(analogRead(pin) > 200) { 
    if(millis() - time > timeout) {
      saveResult(false, text);
      isTimeout = true;
      break;
    }  
  }
  
  if(!isTimeout) saveResult(true, text);
}

void testJoystick(int pin, int op, String text) {
  unsigned long time = millis();
  int timeout = 5000;
  bool isTimeout = false;
  
  Serial.println("Mueve " + text);
  
  switch(op) {
    case 0:
      while(analogRead(pin) > 256) { 
        if(millis() - time > timeout) {
          saveResult(false, text);
          isTimeout = true;
          break;
        }  
      }
      break;
    case 1:
      while(analogRead(pin) < 768) {
        if(millis() - time > timeout) {
          saveResult(false, text);
          isTimeout = true;
          break;
        }
      }
      break;
  }

  if(!isTimeout) saveResult(true, text);
}


void testAccel(int pin, int op, String text) {
  unsigned long time = millis();
  int timeout = 5000;
  bool isTimeout = false;

  Serial.println("Mueve " + text);

  switch(op) {
    case 0:
      while(analogRead(pin) > 300) { 
        if(millis() - time > timeout) {
          saveResult(false, text);
          isTimeout = true;
          break;
        }  
      }
      break;
    case 1:
      while(analogRead(pin) < 500) {
        if(millis() - time > timeout) {
          saveResult(false, text);
          isTimeout = true;
          break;
        }
      }
      break;
  }

  if(!isTimeout) saveResult(true, text);
}

void testMkmk(int pin, int op, String text) {
  unsigned long time = millis();
  int timeout = 5000;
  bool isTimeout = false;
  
  Serial.println("Toca el " + text);

  switch(op) {
    case 0:
      while(analogRead(pin) < 500) { 
        if(millis() - time > timeout) {
          saveResult(false, text);
          isTimeout = true;
          break;
        }  
      }
      break;
    case 1:
      pinMode(pin, INPUT);
      while(digitalRead(pin) == LOW) {
        if(millis() - time > timeout) {
          saveResult(false, text);
          isTimeout = true;
          break;
        }
      }
      break;
  }
  
  if(!isTimeout) saveResult(true, text);
}


bool isCorrect() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  
  int SR = 2, SL = 3;
  bool ok, ko;

  Serial.println("- Pulsa SR si es correcto");
  Serial.println("- Pulsa SL si no es correcto");

  do {
    delay(1);
    ok = digitalRead(SR) == HIGH;
    ko = digitalRead(SL) == HIGH;
  } while (!ok && !ko);

  if(ok) return true;
  if(ko) return false;
}



void saveResult(bool op, String text) {
  String status = op ? "✓" : "✗";
  result[index++] = status + " -> " + text;

  Serial.println(" > " + status);
  Serial.flush();
  delay(500);
}

void printInitSens() {
  Serial.println("---------------------------------");
  Serial.println("Test de EchidnaShield (sensores):");
  Serial.println(" > Fija Select en la Sens ...");
  Serial.println("---------------------------------");
  delay(5000);
}

void printInitMkmk() {
  Serial.println("-----------------------------");
  Serial.println("Test de EchidnaShield (MKMK):");
  Serial.println(" > Fija Select en MKMK ...");
  Serial.println("-----------------------------");
  delay(5000);
}

void printResult() {
  Serial.println("-------------------");
  Serial.println("Resultado del Test:");
  for(int i=0; i<MAX_SENS; i++) {
    Serial.println(result[i]);
  }
  Serial.println("-------------------");
}
