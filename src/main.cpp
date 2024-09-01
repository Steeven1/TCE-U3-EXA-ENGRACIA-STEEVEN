#include <Arduino.h>
#include <LiquidCrystal_I2C.h>   
#include <Wire.h>                  


LiquidCrystal_I2C lcd(0x27,16,2); // dependiendo del fabricante del LCD, la direccion 0x27 puede cambiar, utilice el I2C_Scan primero por favor!

#define I2C_SDA 1
#define I2C_SCL 2

#define btnVERDE 5
#define btnROJO 6
#define btnAZUL 7
#define btnAMARILLO 15

#define ledVERDE 16
#define ledROJO 17 
#define ledAZUL 18
#define ledAMARILLO 8

#define buzzer 19

#define maxNiveles 4
int secuenciaJuego[maxNiveles];

int nivel;

byte caritaTriste[8] = {
  B00000,
  B01010,
  B01010,
  B00000,
  B00100,
  B01010,
  B10001,
  B00000
};

byte caritaAlegre[8] = {
  B00000,
  B01010,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};

void empezar();
void perder();
void ganar();
int leerBoton();
int leerColor();
void simonDice(int i);
void enciendeColorYPantalla(int i);

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();

  // Cargar los caracteres personalizados
  lcd.createChar(0, caritaTriste);
  lcd.createChar(1, caritaAlegre);

  pinMode(ledVERDE, OUTPUT);
  pinMode(ledROJO, OUTPUT);
  pinMode(ledAZUL, OUTPUT);
  pinMode(ledAMARILLO, OUTPUT);

  pinMode(btnVERDE, INPUT_PULLUP);
  pinMode(btnROJO, INPUT_PULLUP);
  pinMode(btnAZUL, INPUT_PULLUP);
  pinMode(btnAMARILLO, INPUT_PULLUP);

  ledcSetup(0, 8000, 8);
  ledcAttachPin(buzzer, 0);

  randomSeed(analogRead(0));

  empezar();
}

void loop() {
  // put your main code here, to run repeatedly:
  int i;
  int entrada;

  delay(1000); //espera 1 segundo

  // Encender tantos colores como sea el nivel actual (1=un color, 2=dos colores, etc.)
  for (i = 0; i < nivel; i++) {
    // Encender el LED que toque y hacer el sonido
    simonDice(secuenciaJuego[i]);
    // Esperar 0.1 segundos (100 milisegundos) antes del siguiente color
    delay(100);
  }

  // Ahora vamos a leer tantos pulsadores como sea el nivel actual
  for (i = 0; i < nivel; i++) {
    entrada = leerColor();
    enciendeColorYPantalla(entrada);
  
    while (leerBoton() != -1);
    // ¿Ha acertado?
    if (entrada != secuenciaJuego[i]) {
        perder();
        empezar();
        return;
    }
  }

  if (nivel == maxNiveles) {
    ganar();
    empezar();
  } else {
      nivel++;
    }
}

//----------------------------------------------------------------------
//FUNCIONES
void empezar(){
  int i;
  nivel = 1;

  //Generar N números aleatorios entre 0 y 3. 
  //Cada número se corresponde con un color: verde, rojo, azul y amarillo
  for (i = 0; i < maxNiveles; i++) {
    secuenciaJuego[i] = random(0, 3);
  }

  lcd.setCursor(0,0);
  lcd.print("Vamos a jugar a");
  lcd.setCursor(3,1);
  lcd.print("SIMON DICE");
  delay(500);
  lcd.clear();
  //Comenzamos por el nivel 1
  lcd.setCursor(1,3);
  lcd.print("***NIVEL ");
  lcd.print(nivel);
  lcd.print("***");
  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_B, 5);
  delay(100);
  ledcWriteNote(0, NOTE_G, 3);
  delay(100);
  ledcWriteNote(0, NOTE_E, 5);
  delay(100);
  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_B, 5);
  delay(100);
  ledcWriteNote(0, NOTE_C, 3);
  delay(100);
  ledcWriteNote(0, NOTE_F, 5);
  delay(100);
  ledcWriteNote(0, NOTE_E, 3);
  delay(100);
  ledcWriteNote(0, NOTE_B, 5);
  delay(100);
  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_F, 5);
  delay(100);
  ledcWriteNote(0, NOTE_G, 3);
  delay(100);
  ledcWriteNote(0, NOTE_A, 5);
  delay(100);
  ledcWrite(0, 0);
  delay(500);

  
}

void perder(){
  int i;

  // Activar un tono grave
  ledcWriteNote(0,NOTE_C,2);
  ledcWrite(0, 255);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("OH NOO ");
  lcd.print("NIVEL ");
  lcd.print(nivel);
  lcd.setCursor(0,1);
  lcd.print("HAS PERDIDO! ");
  lcd.write(byte(0));
  

  delay(500);
  ledcWrite(0, 0); // Apagar tono

  for(int c=0;c<16;c++){
    lcd.scrollDisplayRight();    
    delay(200);
  }

  ledcWrite(0, 0);

}

void ganar(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ENHORABUENA");
  lcd.setCursor(0,1);
  lcd.print("HAS GANADO! ");
  lcd.write(byte(1));
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("NIVEL ");
  lcd.print(nivel);

  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_B, 5);
  delay(100);
  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_B, 5);
  delay(100);
  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_B, 5);
  delay(100);
  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_B, 5);
  delay(100);
  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_B, 5);
  delay(100);
  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_B, 5);
  delay(100);
  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_B, 5);
  delay(100);
  ledcWriteNote(0, NOTE_A, 3);
  delay(100);
  ledcWriteNote(0, NOTE_B, 5);
  delay(100);
  ledcWriteNote(0, NOTE_B, 5);
  delay(400);
  
  for(int c=0;c<16;c++){
    lcd.scrollDisplayRight();    
    delay(150);
  }


}

// Leer los interruptores y devolvemos un número u otro en función del que esté pulsado. 
//Si no hay ninguno, devolver -1
// NOTA: Los interruptores en pull-up devuelven LOW cuando se pulsan y HIGH cuando no
int leerBoton() {
  if (digitalRead(btnVERDE) == LOW) return 0;
  if (digitalRead(btnROJO) == LOW) return 1;
  if (digitalRead(btnAZUL) == LOW) return 2;
  if (digitalRead(btnAMARILLO) == LOW) return 3;

  return -1;
}

int leerColor() {
  int valor;
  while ((valor = leerBoton()) == -1) delay(50);

  return valor;
}

void simonDice(int i) {
    ledcWrite(0, 255);
    switch (i) {
        case 0:
            digitalWrite(ledVERDE, HIGH);

            ledcWriteNote(0, NOTE_F, 5);

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("SIMON DICE");
            lcd.setCursor(0,1);
            lcd.print("VERDE");
            break;
        case 1:
            digitalWrite(ledROJO, HIGH);

            ledcWriteNote(0, NOTE_G, 5);
            
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("SIMON DICE");
            lcd.setCursor(0,1);
            lcd.print("ROJO");
            break;
        case 2:
            digitalWrite(ledAZUL, HIGH);

            ledcWriteNote(0, NOTE_A, 5);

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("SIMON DICE");
            lcd.setCursor(0,1);
            lcd.print("AZUL");
            break;
        case 3:
            digitalWrite(ledAMARILLO, HIGH);

            ledcWriteNote(0, NOTE_B, 3);
            
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("SIMON DICE");
            lcd.setCursor(0,1);
            lcd.print("AMARILLO");
            break;
    }
    delay(750);
    ledcWrite(0, 0);
    lcd.clear();
    digitalWrite(ledVERDE, LOW);
    digitalWrite(ledROJO, LOW);
    digitalWrite(ledAZUL, LOW);
    digitalWrite(ledAMARILLO, LOW);
}

void enciendeColorYPantalla(int i) {
    ledcWrite(0, 255);
    switch (i) {
        case 0:
            digitalWrite(ledVERDE, HIGH);

            ledcWriteNote(0, NOTE_F, 5);

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Color");
            lcd.setCursor(0,1);
            lcd.print("VERDE");
            break;
        case 1:
            digitalWrite(ledROJO, HIGH);

            ledcWriteNote(0, NOTE_G, 5);
            
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Color");
            lcd.setCursor(0,1);
            lcd.print("ROJO");
            break;
        case 2:
            digitalWrite(ledAZUL, HIGH);

            ledcWriteNote(0, NOTE_A, 5);

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Color");
            lcd.setCursor(0,1);
            lcd.print("AZUL");
            break;
        case 3:
            digitalWrite(ledAMARILLO, HIGH);

            ledcWriteNote(0, NOTE_B, 6);
            
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Color");
            lcd.setCursor(0,1);
            lcd.print("AMARILLO");
            break;
    }
    delay(750);
    ledcWrite(0, 0);
    lcd.clear();
    digitalWrite(ledVERDE, LOW);
    digitalWrite(ledROJO, LOW);
    digitalWrite(ledAZUL, LOW);
    digitalWrite(ledAMARILLO, LOW);
}
