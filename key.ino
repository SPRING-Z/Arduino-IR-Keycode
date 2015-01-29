#include <IRremote.h>
#include <stdio.h>

int RECV_PIN = 11;//定义红外接收器的引脚为11

IRrecv irrecv(RECV_PIN);

decode_results results;


void setup()
{
  Serial.begin(9600);//设置串口波特率

  irrecv.enableIRIn(); // 初始化红外接收器
}

void showIRProtocol(decode_results *results)//判断红外协议类型
{

Serial.print("Agreement: ");

switch(results->decode_type) {

case NEC:

Serial.print("NEC");

break;

case SONY:

Serial.print("SONY");

break;

case RC5:

Serial.print("RC5");

break;

case RC6:

Serial.print("RC6");

break;

default:

Serial.print("Error,Check the remote location and stay away from the light");

  }

}
#define convert_byte(__X__)  ((__X__ & 0x01)<<7) | ((__X__ & 0x02)<<5) | ((__X__ & 0x04)<<3) | ((__X__ & 0x08)<<1) | ((__X__ & 0x10)>>1) | ((__X__ & 0x20)>>3) | ((__X__ & 0x40)>>5) | ((__X__ & 0x80)>>7)
//#define convert_byte2(__x__) ((__x__ & 0x0f)<<4) | ((__x__ & 0xf0)>>4)
void loop() {

  if (irrecv.decode(&results)) {

    int val = results.value;

    unsigned char code = (val >> 8)&0xFF;

    unsigned char con = ((code & 0x01)<<7) | ((code & 0x02)<<5) | ((code & 0x04)<<3) | ((code & 0x08)<<1) | ((code & 0x10)>>1) | ((code & 0x20)>>3) | ((code & 0x40)>>5) | ((code & 0x80)>>7);
    
    unsigned char msb, lsb, msb_con, lsb_con;

    unsigned int aaa =(results.value&0xffff0000)>>16;
    
    msb = (aaa>> 8)&0xff;
    lsb = (aaa & 0xff);
    msb_con = convert_byte(msb);
    lsb_con = convert_byte(lsb);
    unsigned short bbb = (lsb_con << 8) | msb_con;
    unsigned short bb = (bbb<<8)|(bbb>>8);

    

    Serial.print("Bits: ");

    Serial.println(results.bits);//输出遥控器编码位数

    showIRProtocol(&results);//输出协议类型

    Serial.println();

    Serial.print("Source: ");   

    Serial.println(results.value,BIN);//以二进制换行输出接收代码
    //Serial.println(results.value, HEX);//以二进制换行输出接收代码

    Serial.print("Key Code: ");

    Serial.println(con, HEX);//以十六进制输出遥控器键码值
    //Serial.println(aaa, HEX);
    Serial.print("System Code: ");
    Serial.println(bb, HEX);
    Serial.println();//为了便于观看输出结果增加一个空行

    irrecv.resume(); // 接收下一个值

  }
}