const int MAX_SENS = 15;
String result[MAX_SENS];
int index = 0;

void setup() {
  Serial.begin(9600);

  printInit();
  
  // LEDS
  testLed(6, "RGB BD6 (Azul)");
  testLed(5, "RGB GD5 (Verde)");
  testLed(9, "RGB RD9 (Rojo)");
  testLed(13, "LED D13 (Rojo)");
  testLed(12, "LED D12 (Naranja)");
  testLed(11, "LED D11 (Verde)");

  // Buzzer
  testBuzzer(10, "Buzzer D10");

  // Joystick
  testJoystick(0, 0, "Joystick A0 (Izquierda)");
  testJoystick(0, 1, "Joystick A0 (Derecha)");
  testJoystick(1, 1, "Joystick A1 (Arriba)");
  testJoystick(1, 0, "Joystick A1 (Abajo)");

  // Acelerometro
  testAceler(2, 0, "Acelerómetro A2X (Izquierda)");
  testAceler(2, 1, "Acelerómetro A2X (Derecha)");
  testAceler(3, 1, "Acelerómetro A3Y (Adelante)");
  testAceler(3, 0, "Acelerómetro A3Y (Atrás)");

  printResult();
}

void loop() {
}


void testLed(int pin, String text) {
  Serial.println("¿Está encendido el " + text + "?");

  analogWrite(pin, 255);
  saveResult(isCorrect(), text);
  analogWrite(pin, 0);
}

void testBuzzer(int pin, String text) {
  Serial.println("¿Ha sonado el " + text + "?");

  for(int i=1; i<10; i++) {
    tone(pin, 1000*i, 100);
    delay(200);
  }
  saveResult(isCorrect(), text);
}

void testJoystick(int pin, int op, String text) {
  unsigned long time = millis();
  int timeout = 5000;
  bool isTimeout = false;
  
  Serial.println("Mueve " + text);
  
  switch(op) {
    case 0:
      while(analogRead(pin) > 240) { 
        if(millis() - time > timeout) {
          saveResult(false, text);
          isTimeout = true;
          break;
        }  
      }
      break;
    case 1:
      while(analogRead(pin) < 400) {
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


void testAceler(int pin, int op, String text) {
  unsigned long time = millis();
  int timeout = 5000;
  bool isTimeout = false;

  Serial.println("Apoya la placa sobre la mesa ... ");
  delay(1000);
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


bool isCorrect() {
  const int SR = 2, SL = 3;
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
  String aux = op ? "✓" : "✗";
  aux+= " -> ";
  aux+= text;
  result[index++] = aux;

  printSpace();
  delay(1000);
}

void printInit() {
  Serial.println("---------------------------------------------------");
  Serial.println("El test de EchidnaShield empezará en 5 segundos ...");
  Serial.println("---------------------------------------------------");
  delay(5000);
}

void printSpace() {
  Serial.println("\n");
}

void printResult() {
  Serial.println("----------------------------------------------");
  Serial.println("El resultado del test ha sido el siguiente ...");
  for(int i=0; i<MAX_SENS; i++) {
    Serial.println(result[i]);
  }
  Serial.println("----------------------------------------------");
}
