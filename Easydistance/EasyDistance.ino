#include <EEPROM.h>
#include "TM1637.h"
#define upKey 4
#define downKey 9
#define menuKey 2

// TM1637
#define Dio 5
#define Clk 6
// 超声波模块设置
#define Trig 8
#define Echo 7
#define Error 10
int a = 0;
int num = 0;
int addr = 0;
float cm;
int menu_Flag;
int buttonState = 0; 
bool buttonStateFlag =true;
TM1637 tm1637(Clk,Dio);

void setup()
{
	pinMode(upKey,INPUT);
	pinMode(downKey,INPUT);
	pinMode(menuKey,INPUT);
	Serial.begin(9600);
	pinMode(Trig, OUTPUT);
	pinMode(Echo,INPUT);
	pinMode(Error,OUTPUT);
	num = EEPROM.read(addr);
	tm1637.init();
	tm1637.set(BRIGHT_TYPICAL); 

	if(num > 9){
		tm1637.display(2,(num/10));
		tm1637.display(3,(num%10));
		delay(100);
	}

}

void loop()
{
	MenuMode();
	switch(a) {
	    case 1:
	      Setting();
	      break;
	    case 0:
	      displayDistance();
	      break;
	}
}


unsigned char MenuMode(){
	buttonState = digitalRead(menuKey);
	if(buttonState == HIGH){
		delay(10);
		if(buttonState == HIGH){
		    a = !a;
		    delay(120);    
		}
	}
}


void Setting(){
	tm1637.clearDisplay();
	if(digitalRead(upKey) == HIGH){

	    delay(20);
	    num++;
	    if(num > 9){
	        int num_H = num / 10;
	        int num_L = num % 10;
	        tm1637.display(2,num_H);
	        tm1637.display(3,num_L);
	        if(num_H > 9){
	            int num_HH = num_H / 10;
	            int num_LL = num_H % 10;
	            tm1637.display(1,num_HH);
	            tm1637.display(2,0);
	        	tm1637.display(3,0);
	        }
	    }else
	    {
	    	tm1637.clearDisplay();
	    	tm1637.display(3,num);
	    }
	    while(digitalRead(upKey) == HIGH);
	}
	if(digitalRead(downKey) == HIGH){
	    delay(20);
	    num--;
	    if(num < 1){
	        num = 0;
	        tm1637.clearDisplay();
	    }else{
		    if(((num/10) != 0)){
		        tm1637.display(2,(num/10));
		        tm1637.display(3,(num%10));
		    }else{
		    	tm1637.clearDisplay();
		    	tm1637.display(3,num);
		    }
		}
	    while(digitalRead(downKey) == HIGH);
	}	
	EEPROM.write(addr,num);
}


void displayDistance(){
	int dis_now = int(distance());
	int dis_nowH = dis_now / 10;
	int dis_nowL = dis_now % 10;
	if(dis_now >= num){
	    if(dis_nowH == 0){
	        tm1637.clearDisplay();
	        tm1637.display(2,dis_now);
	        tm1637.display(3,12);
	    }else{
	    	if(dis_nowH > 9){
	    	    int dis_nowHH = dis_nowH / 10;
	    	    int dis_nowLL = dis_nowH % 10;
	    	    tm1637.display(0,dis_nowHH);
	    	    tm1637.display(1,dis_nowLL);
	    	    tm1637.display(1,dis_nowL);
	    	    tm1637.display(3,12);
	    	}else{
	    		tm1637.clearDisplay();
		    	tm1637.display(1,dis_nowH);
		    	tm1637.display(2,dis_nowL);
		    	tm1637.display(3,12);
			}

	    }
	}else{
  		tm1637.display(1,dis_nowH);
    	tm1637.display(2,dis_nowL);
    	tm1637.display(3,12);
		digitalWrite(Error,HIGH);
		delay(20);
		digitalWrite(Error,LOW);
	}
}

/*测距程序*/
float distance()
{
  float temp;
  digitalWrite(Trig,LOW);
  delayMicroseconds(2);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);
  temp = pulseIn(Echo,HIGH);
  cm = temp / 58.0;
  cm = (int(cm * 100.0)) / 100.0; 
  delay(200);
  return cm;
 }
