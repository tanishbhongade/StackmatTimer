/*
	It'll only work on Arduinos
    The below program will give the time passed between a solve
*/

#include<CapacitiveSensor.h>
#include<LiquidCrystal.h>

CapacitiveSensor CS1=CapacitiveSensor(10,9);// 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapacitiveSensor CS2=CapacitiveSensor(10,8);// 10M resistor between pins 4 & 6, pin 4"" is sensor pin, add a wire and or foil if desired

LiquidCrystal LCD(12,11,5,4,3,2);

const int THRESHOLD=2000;
unsigned long long int previousTime=0;
unsigned long long int currentTime=0;
unsigned long long int deltaTime=0;

void setup() {
  //Put your setup code here, to run once:
    Serial.begin(9600);
    LCD.begin(16,2);
}

void loop(){
	unsigned long long int deltaTime=0;
	int minutes=0;
	int seconds=0;
	int milliSeconds=0;
	
	deltaTime=timePassed();
	
	convertToHHmmSS(&minutes, &seconds, &milliSeconds, deltaTime);
	
	Serial.print(minutes);
	Serial.print(":");
	Serial.print(seconds);
	Serial.print(":");
	Serial.print(milliSeconds);
	Serial.print("\n\n");
}

//Continue until capacitive pads value is not greater than a certain value
//As soon as above event is encountered immediately return time passed in milliseconds
unsigned long long int timePassed() {
	//Amount of time passed by system before beginning solve
	stage1();
	previousTime=millis();
	
	//Do the solve stuff i.e count the time in milliseconds between solve
	
	stage2();
	//Amount of time passed by system after doing solve
	currentTime=millis();
    
    //This is the amount of time passed
    deltaTime=currentTime-deltaTime;
    
    //Using the above logic, there is no need to continuosly reset Arduino
    return deltaTime;
}

void convertToHHmmSS(int* minutes, int* secs, int *milliSecs, int deltaTime){
    //Using pointers the given function calculates the no of minutes, seconds and milliseconds passed.
    while(deltaTime>=60000){
        //Count no of minutes
        deltaTime=deltaTime-60000;
        *minutes=*minutes+1;
    }

    while(deltaTime>=1000){
        //Count no of seconds
        deltaTime=deltaTime-1000;
        *secs=*secs+1;
    }
        
    while(deltaTime>0){
        //Count no of milliseconds
        deltaTime=deltaTime-1;
        *milliSecs=*milliSecs+1;
    }
}

int capValue(){
	//Checks whether palm are placed or not
	unsigned long long int capValue1=CS1.capacitiveSensor(30);
	unsigned long long int capValue2=CS2.capacitiveSensor(30);
	
	if(capValue1>THRESHOLD && capValue2>THRESHOLD){
		return 1;
	}
	else{
		return 0;
	}
}

void stage1(){
	//Palms placed
	while(capValue()==1){
		Serial.println("Ready!");
	}
}

void stage2(){
	//Palms released
	while(capValue()==0){
		Serial.println("Solve!");
	}
}

void printLCD(int colLCD, int rowLCD, int minutes, int seconds, int milliSeconds){
	//We'll use the function to print to LCD display of 16cols x 2rows
	int timeArray[7];
	
	timeArray[0]=minutes%10;
	minutes=minutes/10;
	timeArray[1]=minutes%10;
	
	timeArray[2]=seconds%10;
	seconds=seconds/10;
	timeArray[3]=seconds%10;
	
	timeArray[4]=milliSeconds%10;
	milliSeconds=milliSeconds/10;
	timeArray[5]=milliSeconds%10;
	milliSeconds=milliSeconds/10;
	timeArray[6]=milliSeconds%10;
	
	//Use LCDsetCursorNPrint function to pass col, row and timeArray[i]
}