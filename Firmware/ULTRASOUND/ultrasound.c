/********************************************************
t TxPin =18; //pin18為超音波模組的Tx接腳
int RxPin =19; //pin19為超音波模組的Rx接腳
int ledPin =10; //pin10的LED用來做PWM
int buzPin =11; //pin11為蜂鳴器
********************************************************/
#include "QuadCopterConfig.h"

//副函式：控制LED的亮度變化與蜂鳴器聲音長度變化
void Control_PWM(int d)
{
int x,y; //x:led的PWM轉換 , y:buzzer的持續時間轉換
if(d>50) //與超音波模組距離大於50cm
{
analogWrite(ledPin,0); //距離太遠了，所以led不亮
delay(300);
tone(buzPin,523,500); //tone(Pin,frequency,duration) 523代表Do的音
}
if(d<=50) //與超音波模組距離小於等於50cm
{
x=255-(d*5.1); //把0~50cm轉成PWM的"255~0" ps.50:255=d:x ---->x=255-d*(255/50)
y=10+19.8*d; //把0~50cm轉成持續時間"10~500"ms的範圍 (y=10+d*490/50)
analogWrite(ledPin,x); //當距離越遠LED的PWM越小(暗); 當距離越近LED的PWM越大(亮)
delay(300);
tone(buzPin,523,y); //tone(Pin,frequency,duration) 523代表Do的音
}
}

//副函式：停止led亮,停止蜂鳴器叫
void End()
{
digitalWrite(buzPin,LOW);
analogWrite(ledPin,0);
delay(1000); // 等待一秒鐘（每隔一秒測量一次）

}

//放要設置初始化的程式碼
void setup()
{
Serial.begin(9600); //設置鮑率為9600
Serial1.begin(9600); //設置pin18 pin19鮑率為9600
}

//主要的程式碼
void read_ultrasound()
{
	unsigned int lenHigh = 0; //高位
	unsigned int lenLow = 0; //低位
	unsigned int d = 0; //距離
	
	Serial1.flush(); //清空串列接收緩衝區
	Serial1.write(0x55); //發送0x55，觸發SRF06 開始測距

	vTaskDelay(200);

	if(Serial1.available() >= 2) //當串列接收緩衝區中資料大於2 位元組
	{
		lenHigh = Serial1.read(); //距離的高位元組
		lenLow = Serial1.read(); //距離的低位元組
		d =(lenHigh*256 + lenLow)*0.1; //計算距離值
		Serial.print(d);
		Serial.println("cm");
}
Control_PWM(d); //控制LED的亮度變化與蜂鳴器聲音長度變化
End(); //停止led亮,停止蜂鳴器叫
delay(500); // 等待500ms

}