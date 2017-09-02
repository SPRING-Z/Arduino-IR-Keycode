#include <IRremote.h>
#include <stdio.h>

#define convert_byte(__X__)  ((__X__ & 0x01)<<7) | ((__X__ & 0x02)<<5) | ((__X__ & 0x04)<<3) | ((__X__ & 0x08)<<1) | ((__X__ & 0x10)>>1) | ((__X__ & 0x20)>>3) | ((__X__ & 0x40)>>5) | ((__X__ & 0x80)>>7)

//定义红外接收器的引脚为11
int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;


void setup() {
    // 设置波特率
    Serial.begin(9600);
    // 初始化红外接收器
    irrecv.enableIRIn();
}


void loop() {

    if (irrecv.decode(&results)) {

        // 红外数字输出
        int val = results.value;

        // 按照 NEC 协议 开始反转 移位
        unsigned char code = (val >> 8) & 0xFF;

        unsigned char con = ((code & 0x01) << 7) | ((code & 0x02) << 5) | ((code & 0x04) << 3) | ((code & 0x08) << 1) |
                            ((code & 0x10) >> 1) | ((code & 0x20) >> 3) | ((code & 0x40) >> 5) | ((code & 0x80) >> 7);

        unsigned char msb, lsb, msb_con, lsb_con;

        unsigned int value = (results.value & 0xffff0000) >> 16;

        msb = (value >> 8) & 0xff;
        lsb = (value & 0xff);
        msb_con = convert_byte(msb);
        lsb_con = convert_byte(lsb);
        unsigned short out_p = (lsb_con << 8) | msb_con;
        unsigned short out = (out_p << 8) | (out_p >> 8);


        Serial.print("Bits: ");

        //输出遥控器编码位数
        Serial.println(results.bits);

        //输出协议类型
        showIRProtocol(&results);

        Serial.print("Source: ");

        //以二进制换行输出原始接收代码
        Serial.println(results.value, BIN);

        //以十六进制输出遥控器键码值
        Serial.print("Key Code: ");
        Serial.println(con, HEX);

        // 输出需要的遥控器用户码
        Serial.print("System Code: ");
        Serial.println(out, HEX);

        
        irrecv.resume(); // 接收下一个值

    }

}

/*
 * 显示红外接收类型
 */
void showIRProtocol(decode_results *results)//判断红外协议类型
{

    Serial.print("Agreement: ");

    switch (results->decode_type) {

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