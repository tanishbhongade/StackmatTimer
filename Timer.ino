/*
	It'll only work on Arduinos
    The below program will give the time passed between a solve
*/

#include<CapacitiveSensor.h>
#include<LiquidCrystal.h>

CapacitiveSensor CS1=CapacitiveSensor(10,9);// 10M resistor between pins 10 & 9, pin 9 is sensor pin, add a wire and or foil if desired
CapacitiveSensor CS2=CapacitiveSensor(10,6);// 10M resistor between pins 10 & 6, pin 6 is sensor pin, add a wire and or foil if desired

LiquidCrystal LCD(12,11,5,4,3,2);

const int THRESHOLD=2000;
unsigned long long int previousTime=0;
unsigned long long int currentTime=0;
unsigned long long int deltaTime=0;

void setup() {
  //Put your setup code here, to run once:
    Serial.begin(9600);
    LCD.begin(16,2);
    pinMode(13,INPUT);
}

void loop(){
	unsigned long long int deltaTime=0;
	int minutes=0;
	int seconds=0;
	int milliSeconds=0;
	
	deltaTime=timePassed();
	
	convertToHHmmSS(&minutes, &seconds, &milliSeconds, deltaTime);
	
	//Use serial monitor to get times
	Serial.print(minutes);
	Serial.print(":");
	Serial.print(seconds);
	Serial.print(":");
	Serial.print(milliSeconds);
	Serial.print("\n\n");
	
	//Uncomment the below line to use LCD interface to get times
	//printLCD(minutes,seconds,milliSeconds);
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

void printLCD(int minutes, int seconds, int milliSeconds){
	//We'll use the function to print to LCD display of 16cols x 2rows
	int timeArray[7];
	
	LCD.setCursor(0,0);
	LCD.print("STACKMAT TIMER");
	
	timeArray[1]=minutes%10;
	minutes=minutes/10;
	timeArray[0]=minutes%10;
	
	timeArray[3]=seconds%10;
	seconds=seconds/10;
	timeArray[2]=seconds%10;
	
	timeArray[6]=milliSeconds%10;
	milliSeconds=milliSeconds/10;
	timeArray[5]=milliSeconds%10;
	milliSeconds=milliSeconds/10;
	timeArray[4]=milliSeconds%10;
	
	//Use LCDsetCursorNPrint function to pass col, row and timeArray[i]
	LCDsetCursorNPrint(0,1,timeArray[0]);
	LCDsetCursorNPrint(1,1,timeArray[1]);
	
	LCD.setCursor(2,1);
	LCD.print(":");
	
	LCDsetCursorNPrint(3,1,timeArray[2]);
	LCDsetCursorNPrint(4,1,timeArray[3]);
	
	LCD.setCursor(5,1);
	LCD.print(".");
	
	LCDsetCursorNPrint(6,1,timeArray[4]);
	LCDsetCursorNPrint(7,1,timeArray[5]);
	LCDsetCursorNPrint(8,1,timeArray[6]);
	
	while(1!=0){
   int RSTBtn=digitalRead(13);
		if(RSTBtn==1){
			LCD.clear();
			break;
		}
	}
}

void LCDsetCursorNPrint(int colLCD, int rowLCD, int timeLCD){
	LCD.setCursor(colLCD, rowLCD);
	LCD.print(timeLCD);
}