#include "TM1637.h"
#include <OneWire.h>
#include <DS18B20.h>
#include <EEPROM.h>
// // TM1637
#define Dio 4
#define Clk 5
TM1637 tm1637(Clk,Dio);

DS18B20 ds(3);
#define upKey 6
#define downKey 7
#define menuKey 9
#define funPin A0

// 温度上下限
float Temp_Top;
float Temp_Low;
int buttonState;
int a = 0;
int num = 0;
int addr = 0;
void setup()
{
	Serial.begin(9600);
	pinMode(upKey, INPUT);
	pinMode(downKey, INPUT);
	pinMode(menuKey, INPUT);
	pinMode(funPin, OUTPUT);
	Temp_Top = int(EEPROM.read(addr+1));
	Temp_Low = int(EEPROM.read(addr+2));

	tm1637.init();
	tm1637.set(BRIGHT_TYPICAL); 
}

void loop()
{
	MenuModeHandler();
	Serial.println(a);
	switch (a) {
	    case 0:
	      DisplayTemp();
	      SmartFun(); 
	      break;
	    case 1:
	      tm1637.clearDisplay();
	      SetTopHandler();
	      break;
	    case 2:
	      tm1637.clearDisplay();
	      SetLowHandler();
	      break;
	}
}


void SmartFun(){
	int tmp_now = int(ds.getTempC());
	if (tmp_now >= Temp_Top){
    Serial.println("high temp");
		analogWrite(funPin, 255);
	}else if(tmp_now >= Temp_Low){
		analogWrite(funPin,150);
    Serial.println("middle temp");
	}else{
		analogWrite(funPin, 0);
    Serial.println("low temp");
	}
}

// 模式选择
unsigned char MenuModeHandler(){
	buttonState = digitalRead(menuKey);
	if(buttonState == HIGH){
		delay(10);
		if(buttonState == HIGH){
		    a += 1;
		    delay(120);    
		}
		if(a > 2){
			a = 0;
		}
	}
}


void SetTopHandler(){
	if(digitalRead(upKey) == HIGH){
	    delay(20);
	    Temp_Top++;
	    if(Temp_Top > 9){
	        int num_H = Temp_Top / 10;
	        int num_L = int(Temp_Top) % 10;
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
	    	tm1637.display(3,Temp_Top);
	    }
	    while(digitalRead(upKey) == HIGH);
	}
	if(digitalRead(downKey) == HIGH){
	    delay(20);
	    Temp_Top--;
	    if(Temp_Top < 1){
	        Temp_Top = 0;
	        tm1637.clearDisplay();
	    }else{
		    if(((Temp_Top/10) != 0)){
		        tm1637.display(2,(Temp_Top / 10));
		        tm1637.display(3,(int(Temp_Top) % 10));
		    }else{
		    	tm1637.clearDisplay();
		    	tm1637.display(3,Temp_Top);
		    }
		}
	    while(digitalRead(downKey) == HIGH);
	}	
	EEPROM.write(addr+1,Temp_Top);
}


void SetLowHandler(){
	if(digitalRead(upKey) == HIGH){
	    delay(20);
	    Temp_Low++;
	    if(Temp_Low > 9){
	        int num_H = Temp_Low / 10;
	        int num_L = int(Temp_Low) % 10;
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
	    	tm1637.display(3,Temp_Low);
	    }
	    while(digitalRead(upKey) == HIGH);
	}
	if(digitalRead(downKey) == HIGH){
	    delay(20);
	    Temp_Low--;
	    if(Temp_Low < 1){
	        Temp_Low = 0;
	        tm1637.clearDisplay();
	    }else{
		    if(((Temp_Low/10) != 0)){
		        tm1637.display(2,(Temp_Low/10));
		        tm1637.display(3,(int(Temp_Low)%10));
		    }else{
		    	tm1637.clearDisplay();
		    	tm1637.display(3,Temp_Low);
		    }
		}
	    while(digitalRead(downKey) == HIGH);
	}	
	EEPROM.write(addr+2,Temp_Low);	
}


void DisplayTemp(){
	int tmp_now = int(ds.getTempC());
	// Serial.println(tmp_now);
	if(tmp_now > Temp_Top){
		if((tmp_now / 10) != 0){
		   	int tmp_nowH = tmp_now / 10;
			int tmp_nowL = tmp_now % 10;
			tm1637.clearDisplay();
			tm1637.display(1,tmp_nowH);
			tm1637.display(2,tmp_nowL);
			tm1637.display(3,12);
		}else{
			tm1637.clearDisplay();
			tm1637.display(2,tmp_now);
		}	    
	}else if(tmp_now >= Temp_Low){
		if((tmp_now / 10) != 0){
		   	int tmp_nowH = tmp_now / 10;
			int tmp_nowL = tmp_now % 10;
			tm1637.clearDisplay();
			tm1637.display(1,tmp_nowH);
			tm1637.display(2,tmp_nowL);
			tm1637.display(3,12);
		}else{
			tm1637.clearDisplay();
			tm1637.display(2,tmp_now);
		}
	}else{
		if((tmp_now / 10) != 0){
		   	int tmp_nowH = tmp_now / 10;
			int tmp_nowL = tmp_now % 10;
			tm1637.clearDisplay();
			tm1637.display(1,tmp_nowH);
			tm1637.display(2,tmp_nowL);
			tm1637.display(3,12);
		}else{
			tm1637.clearDisplay();
			tm1637.display(2,tmp_now);
		}
	}
}
