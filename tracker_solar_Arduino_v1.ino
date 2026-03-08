// Fotodioda dreapta-sus A2 merge (numarul 3)
// Celelalte fotodiode nu merg (afiseaza mereu valori intre 1000 
// cand tii degetul pe ea si pana la 1023, aproape valoare maxima)
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 8

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

float Celsius = 0;
float Fahrenheit = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows
class Photodiode {
private:
    int id;
    int pin;
    int value;

public:
  // Constructor
  Photodiode(int id, int pin) {
      this->id = id;
      this->pin = pin;
      this->value = 0;
  }

  // Citesc valoarea dintr-un pin
  int citesteValoare() {
      value = analogRead(pin);
      return value;
  }
  // Getteri
  int getId() const {
      return id;
  }
  int getValue() const {
      return value;
  }
  int getPin() const {
      return pin;
  }
  // Afisare informatii despre o fotodioda
  void display() const {
      Serial.print("Photodiode ");
      Serial.print(id);
      Serial.print(" (Pin ");
      Serial.print(pin);
      Serial.print("): ");
      Serial.println(value);
  }
};

class Servomotor {
private:
  Servo servo;
  char* id;
  int pin;
  int pozitie;
  int limitLow;
  int limitHigh;
public:
  // Constructor
  Servomotor(Servo& servo, int pozitie, int limitLow, int limitHigh, const char* id) {
    this->servo = servo;
    this->pozitie = pozitie;
    this->limitLow = limitLow;
    this->limitHigh = limitHigh;
    this->id = new char[strlen(id) + 1];
    strcpy(this->id, id);
    pin = -1;
  }
  // Destructor
  ~Servomotor() {
    delete[] id;
  }
  // Attach
  void attach(int pin) {
    this->pin = pin;
    servo.attach(pin);
    servo.write(pozitie);
  }
  // Muta la pozitia
  void mutaServo(int pozitieNoua) {
        if (pozitieNoua >= limitLow && pozitieNoua <= limitHigh) {
            pozitie = pozitieNoua;
            servo.write(pozitie);
        }
  }
  // Getteri
  const char* getId() const { return id; }
  int getPozitie() const { return pozitie; }
  int getLimitLow() const { return limitLow; }
  int getLimitHigh() const { return limitHigh; }
  // Afisare informatii despre un servomotor
  void display() const {
      Serial.print("Servomotorul: ");
      Serial.print(id);
      Serial.print(", Pin: ");
      Serial.print(pin);
      Serial.print(", Pozitie: ");
      Serial.println(pozitie);
  }
};

Photodiode* photodiode[4];
int numarFotodiode = 0;

Servomotor* servomotoare[2];
int numarServomotoare = 0;

Servo horizontal;
Servo vertical;

int maxvalue = -1, pozv = -1, pozh = -1;

void setup() {
  Serial.begin(9600); // rata baud 9600

  sensors.begin();

  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Panou tracker");
  lcd.setCursor(0, 1);
  lcd.print("solar!!!!!!!!!!!");

  photodiode[numarFotodiode++] = new Photodiode(1, A0);
  photodiode[numarFotodiode++] = new Photodiode(2, A1);
  photodiode[numarFotodiode++] = new Photodiode(3, A2); // doar fotodioda asta merge
  photodiode[numarFotodiode++] = new Photodiode(4, A3);

  servomotoare[numarServomotoare++] = new Servomotor(horizontal, 150, 0, 180, "horizontal");
  servomotoare[numarServomotoare - 1]->attach(9);

  servomotoare[numarServomotoare++] = new Servomotor(vertical, 90, 0, 180, "vertical");
  servomotoare[numarServomotoare - 1]->attach(10);

  delay(7000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Setez pozitia de");
  lcd.setCursor(0, 1);
  lcd.print("inceput panou...");

  delay(2500);
  for(int servoh = servomotoare[0]->getPozitie(); servoh > 0 ; servoh -= 1) {
    servomotoare[0]->mutaServo(servoh);
    delay(50);
  }
  for(int servov = servomotoare[1]->getPozitie(); servov > 0 ; servov -= 1) {
    servomotoare[1]->mutaServo(servov);
    delay(50);
    if(servov == 30) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Incepe masurarea");
      lcd.setCursor(0, 1);
      lcd.print("..........!!!!!!");
    }
  }
  delay(2000);
}

void loop() {

  for(int servov = servomotoare[1]->getPozitie(); servov < servomotoare[1]->getLimitHigh(); servov += 10) {
    servomotoare[1]->mutaServo(servov);
    int value = photodiode[2]->citesteValoare();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Val: ");
    lcd.print(value);
    Serial.println(value);
    if(value > maxvalue) {
      maxvalue = value;
      pozh = servomotoare[0]->getPozitie();
      pozv = servomotoare[1]->getPozitie();
      lcd.setCursor(0, 1);
      lcd.print("Nou maxim: ");
      lcd.print(maxvalue);
      Serial.print("New max value = ");
      Serial.println(maxvalue);
    }
    else {
      lcd.setCursor(0, 1);
      lcd.print("Curent max: ");
      lcd.print(maxvalue);
    }
    delay(500);
  }
  for(int servoh = servomotoare[0]->getPozitie(); servoh < servomotoare[0]->getLimitHigh(); servoh += 10) {
    servomotoare[0]->mutaServo(servoh);
    int value = photodiode[2]->citesteValoare();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Val: ");
    lcd.print(value);
    Serial.println(value);
    if(value > maxvalue) {
      maxvalue = value;
      pozh = servomotoare[0]->getPozitie();
      pozv = servomotoare[1]->getPozitie();
      lcd.setCursor(0, 1);
      lcd.print("Nou maxim: ");
      lcd.print(maxvalue);
      Serial.print("New max value = ");
      Serial.println(maxvalue);
    }
    else {
      lcd.setCursor(0, 1);
      lcd.print("Curent max: ");
      lcd.print(maxvalue);
    }
    delay(500);
  }
  for(int servov = servomotoare[1]->getPozitie(); servov > servomotoare[0]->getLimitLow(); servov -= 10) {
    servomotoare[1]->mutaServo(servov);
    int value = photodiode[2]->citesteValoare();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Val: ");
    lcd.print(value);
    Serial.println(value);
    if(value > maxvalue) {
      maxvalue = value;
      pozh = servomotoare[0]->getPozitie();
      pozv = servomotoare[1]->getPozitie();
      lcd.setCursor(0, 1);
      lcd.print("Nou maxim: ");
      lcd.print(maxvalue);
      Serial.print("New max value = ");
      Serial.println(maxvalue);
    }
    else {
      lcd.setCursor(0, 1);
      lcd.print("Curent max: ");
      lcd.print(maxvalue);
    }
    delay(500);
  }
  for(int servoh = servomotoare[0]->getPozitie(); servoh > servomotoare[0]->getLimitLow(); servoh -= 10) {
    servomotoare[0]->mutaServo(servoh);
    int value = photodiode[2]->citesteValoare();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Val: ");
    lcd.print(value);
    Serial.println(value);
    if(value > maxvalue) {
      maxvalue = value;
      pozh = servomotoare[0]->getPozitie();
      pozv = servomotoare[1]->getPozitie();
      lcd.setCursor(0, 1);
      lcd.print("Nou maxim: ");
      lcd.print(maxvalue);
      Serial.println("New max value = ");
      Serial.println(maxvalue);
    }
    else {
      lcd.setCursor(0, 1);
      lcd.print("Curent max: ");
      lcd.print(maxvalue);
    }
    delay(500);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Merg la pozitia");
  lcd.setCursor(0, 1);
  lcd.print("cu val maxima...");

  for(int i = servomotoare[0]->getPozitie(); i < pozh; i+=1) {
    servomotoare[0]->mutaServo(i);
    delay(50);
  }
  for(int i = servomotoare[1]->getPozitie(); i < pozv; i+=1) {
    servomotoare[1]->mutaServo(i);
    delay(50);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Am ajuns la");
  lcd.setCursor(0, 1);
  lcd.print("pozitia dorita!!");
  delay(5000);

  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celsius);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatura   *C");
  lcd.setCursor(0, 1);
  lcd.print("curenta: ");
  lcd.print(Celsius);
  delay(8000);

  Serial.println("----------------------");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pauza 30 secunde");
  lcd.setCursor(0, 1);
  lcd.print("---( *^__*^ )---");
  
  delay(30000); // Wait for 60 second
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Resetez pozitia");
  lcd.setCursor(0, 1);
  lcd.print("panoului........");
  for(int i = servomotoare[0]->getPozitie(); i > servomotoare[0]->getLimitLow(); i-=1) {
    servomotoare[0]->mutaServo(i);
    delay(50);
  }
  for(int i = servomotoare[1]->getPozitie(); i > servomotoare[1]->getLimitLow(); i-=1) {
    servomotoare[1]->mutaServo(i);
    delay(50);
  }
  maxvalue = -1; pozv = -1; pozh = -1;
  Serial.println("Urmatoarea masurare");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Incep urmatoarea");
  lcd.setCursor(0, 1);
  lcd.print("masurare...!!!!!");
  delay(5000);
}