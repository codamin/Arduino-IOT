#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <string.h>

#define NUM_DATA_BYTES 4
#define HUMIDITY_DELIM_START 'h'
#define TEMEPERATURE_DELIM_START 't'
#define MAX_PWM 255

// Defining LCD pins
// LCD RS pin to digital pin 12
// LCD Enable pin to digital pin 11
// LCD D4 pin to digital pin 5
// LCD D5 pin to digital pin 4
// LCD D6 pin to digital pin 3
// LCD D7 pin to digital pin 2

// LCD R/W pin to GND
// LCD VSS pin to GND
// LCD VCC pin to 5V
int in1 = 10;
int in2 = 9;
int en = 6;

const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

float humidity = 0;
float temperature = 0;
bool must_update_lcd = false;
bool is_rcving_humidity = false;
bool is_rcving_temperature = false;
bool has_started = false;
String watering_type = "No Water";
///////////////////////////////////////////////
void rcv_data()
{
  char byte;
  float data;
  if(Serial.available()) {
    if(!is_rcving_humidity && !is_rcving_temperature) {
      byte = Serial.read();
      if (byte == HUMIDITY_DELIM_START) {
        is_rcving_humidity = true;
      }
      else if (byte == TEMEPERATURE_DELIM_START) {
        is_rcving_temperature = true;
      }
    }
    else {
      if(Serial.available() >= 2) {
        data = Serial.parseFloat();
        if (is_rcving_humidity) {
          humidity = data;
          is_rcving_humidity = false;
        }
        else if (is_rcving_temperature) {
          temperature = data;
          is_rcving_temperature = false;
        }
      }
      must_update_lcd = true;
    }
  }
}

void init_lcd()
{
  pinMode(en, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  lcd.setCursor(0, 0); // top left
  lcd.print("No Humidity");

  lcd.setCursor(0, 1); // bottom left
  lcd.print("NO Temperature");

  lcd.setCursor(0, 2);
  lcd.println(watering_type);
}

void update_lcd()
{
  if (!must_update_lcd)
    return;
  lcd.clear();
  lcd.setCursor(0, 0); // top left
  lcd.print("H: ");
  lcd.println(humidity);

  lcd.setCursor(0, 1); // bottom left
  lcd.print("T: ");
  lcd.println(temperature);

  lcd.setCursor(0, 2);
  lcd.println(watering_type);

  must_update_lcd = false;
}

void process_watering() {
  if (!temperature || !humidity) return;
  if(humidity > 50) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    watering_type = "No Water";
  }
  else if (humidity < 20) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(en, MAX_PWM * 0.25);
    watering_type = "Water 20 CC";
  }
  else {
    if(temperature < 25) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      watering_type = "No Water";
    }
    else {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      analogWrite(en, MAX_PWM * 0.1);
      watering_type = "Water 10 CC";
    }
  } 
}

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 3);
  init_lcd();
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(en, 0);
}

void loop()
{
  rcv_data();
  process_watering();
  update_lcd();
}