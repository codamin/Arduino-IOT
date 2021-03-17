#include <Arduino.h>
#include <Wire.h>

#define TEMPERATURE_SIGN 't'
#define HUMIDITY_SIGN 'h'
#define ADDR 0x40
#define TEMPERATURE_COMMAND 0xF3
#define HUMIDITY_COMMAND 0xF5

float temperature = -1;
float humidity = -1;

void updateTemperature();
void updateHumidity();
void updateState();
void writeState();

void setup() {
	Wire.begin();
	Serial.begin(9600);
}

void loop() {
	updateState();
	writeState();
}

void updateState() {
	updateTemperature();
	updateHumidity();
}

void writeState() {
	Serial.print(TEMPERATURE_SIGN);
	Serial.print(temperature);
	Serial.print(HUMIDITY_SIGN);
	Serial.println(humidity);
}

void updateTemperature() {
	Wire.beginTransmission(ADDR);  
	Wire.write(TEMPERATURE_COMMAND);  
	Wire.endTransmission();  
	delay(500);

	Wire.requestFrom(ADDR, 2);
	unsigned int data[2];
	if(Wire.available() == 2) {    
		data[0] = Wire.read();    
		data[1] = Wire.read();
		temperature = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
	}  
}

void updateHumidity(){
	Wire.beginTransmission(ADDR);  
	Wire.write(HUMIDITY_COMMAND);  
	Wire.endTransmission();  
	delay(500);

	Wire.requestFrom(ADDR, 2);
	unsigned int data[2];
	if(Wire.available() == 2) {
		data[0] = Wire.read();  
		data[1] = Wire.read();
		humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
	}
}