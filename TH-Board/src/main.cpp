#include <Arduino.h>
#include <Wire.h>

// SHT25 I2C address is 0x40(64)
#define Addr 0x40

#define BLUETOOTH_TEMPERATURE_START_CHAR '#'
#define BLUETOOTH_HUMIDITY_START_CHAR '*'

float readTemperature();
float readHumidity();


void setup() {
	Wire.begin();                // join i2c bus (address optional for master)
	Serial.begin(9600);          // start serial communication at 9600bps
}

void loop() {
	float temperature = readTemperature();
	float humidity = readHumidity();
	Serial.print(BLUETOOTH_TEMPERATURE_START_CHAR);
	Serial.println(temperature);
	Serial.print(BLUETOOTH_HUMIDITY_START_CHAR);
	Serial.println(humidity);
}

float readTemperature(){
	unsigned int data[2];
	// Start I2C transmission  
	Wire.beginTransmission(Addr);  
	// Send temperature measurement command, NO HOLD master  
	Wire.write(0xF3);  
	// Stop I2C transmission  
	Wire.endTransmission();  
	delay(500);
	// Request 2 bytes of data  
	Wire.requestFrom(Addr, 2);
	// Read 2 bytes of data  
	// temp msb, temp lsb  
	if(Wire.available() == 2)  
	{    
		data[0] = Wire.read();    
		data[1] = Wire.read();
		// Convert the data    
		float cTemp = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;    
		float fTemp = (cTemp * 1.8) + 32;
		// Output data to Serial Monitor
		return cTemp;
	}  
}

float readHumidity(){
	unsigned int data[2];  
	// Start I2C transmission  
	Wire.beginTransmission(Addr);  
	// Send humidity measurement command, NO HOLD master  
	Wire.write(0xF5);  
	// Stop I2C transmission  
	Wire.endTransmission();  
	delay(500);
	// Request 2 bytes of data  
	Wire.requestFrom(Addr, 2);
	// Read 2 bytes of data  
	// humidity msb, humidity lsb
	if(Wire.available() == 2)
	{
		data[0] = Wire.read();  
		data[1] = Wire.read();
		// Convert the data    
		float humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
		// Output data to Serial Monitor
		return humidity;
	}
}