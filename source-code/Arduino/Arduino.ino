#define led4 5    // BUTTON LED CIRCLE
#define led3 6    // BUTTON LED CROSS
#define led2 7    // BUTTON LED SQUARE
#define led1 8    // BUTTON LED TRIANGLE

#define led_B1 9  // LEFT PARTITION LED BLUE
#define led_R1 10 // LEFT PARTITION LED RED
#define led_G1 11 // LEFT PARTITION LED GREEN

#define led_B2 12 // RIGHT PARTITION LED BLUE
#define led_R2 14 // RIGHT PARTITION LED RED
#define led_G2 13 // RIGHT PARTITION LED GREEN


void setup() {
     Serial.begin(38400);
     pinMode(led1, OUTPUT);
     pinMode(led2, OUTPUT);
     pinMode(led3, OUTPUT);
     pinMode(led4, OUTPUT);
     pinMode(led_R1, OUTPUT);
     pinMode(led_G1, OUTPUT);
     pinMode(led_B1, OUTPUT);
     pinMode(led_R2, OUTPUT);
     pinMode(led_G2, OUTPUT);
     pinMode(led_B2, OUTPUT);
}

void updateSidesLeds(uint8_t inData) 
{
	digitalWrite(led_R1, bitRead(inData,0));
	digitalWrite(led_G1, bitRead(inData,1));
	digitalWrite(led_B1, bitRead(inData,2));
	
	digitalWrite(led_R2, bitRead(inData,3));
	digitalWrite(led_G2, bitRead(inData,4));
	digitalWrite(led_B2, bitRead(inData,5));
}

void updateButtonsLeds(uint8_t inData) 
{   
	digitalWrite(led1, bitRead(inData,0));
	digitalWrite(led2, bitRead(inData,1));
	digitalWrite(led3, bitRead(inData,2));
	digitalWrite(led4, bitRead(inData,3));
}

void loop(){
	if(Serial.available()){
		uint8_t inData = Serial.read();
		uint8_t typeData = inData & 0b11000000;
		switch(typeData){
		case 0b00000000:
			updateSidesLeds(inData);
			break;
		case 0b11000000:
			updateButtonsLeds(inData);
			break;
		}
	}
}
