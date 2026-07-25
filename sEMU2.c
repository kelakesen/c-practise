#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// 1. 定义全部CPU运行状态
typedef enum {
    STATE_IF,    // 取指
    STATE_ID,    // 译码拆分指令
    STATE_EX,    // 运算执行
    STATE_WB,    // 写回寄存器
} CPU_STATE;

int main(int argc,char *argv[]){
    uint8_t pc=0;
    uint8_t M[16]={0b10010000,0b10100000,0b10110001,0b00101011,0b00010110,0b11001110,0b01000000};
    uint8_t R[4] = {0};
    R[0]=atoi(argv[1]);
    uint8_t inst, opcode, rd, rs1, rs2, imm, addr;
    uint8_t rd_code, rs1_code, rs2_code;
    CPU_STATE curr_state = STATE_IF; // 初始状态：取指

    while(pc < 7)
    {
        switch(curr_state)
        {
            case STATE_IF:
                // 状态1：取指令
                inst = M[pc];
                // 切换下一个状态：译码
                curr_state = STATE_ID;
                break;

            case STATE_ID:
                // 状态2：拆分指令字段（你原来的位运算）
                opcode = (inst >> 6) & 0x03;
                rd     = (inst >> 4) & 0x03;
                rs1    = (inst >> 2) & 0x03;
                rs2    = inst & 0x03;
                imm    = inst & 0x0F;
                addr   = (inst >> 2) & 0x0F;

                // rd、rs1、rs2 编码转换
                switch(rd){
                    case 0b00: rd_code=0; break;
                    case 0b01: rd_code=1; break;
                    case 0b10: rd_code=2; break;
                    case 0b11: rd_code=3; break;
                }
		switch(rs1){
                case 0b00:
                        rs1_code = 0;
                        break;
		case 0b01:
                        rs1_code = 1;
                        break;
                case 0b10:
                        rs1_code = 2;
                        break;
                case 0b11:
                        rs1_code = 3;
                        break;
                default:
                        printf("rs1_code error");
                        break;
       		 }
		switch(rs2){
                case 0b00:
                        rs2_code = 0;
                        break;
                case 0b01:
                        rs2_code = 1;
                        break;
                case 0b10:
                        rs2_code = 2;
                        break;
                case 0b11:
                        rs2_code = 3;
                        break;
                default:
                        printf("rs2_code error");
                        break;
       		 }
                curr_state = STATE_EX; // 跳转执行阶段
                break;

            case STATE_EX:
                // 状态3：执行运算、跳转、立即数赋值
                switch(opcode){
                    case 0: // add
                        R[rd_code] = R[rs1_code] + R[rs2_code];
                        pc++;
                        break;
                    case 2: // li
                        R[rd_code] = imm;
                        pc++;
                        break;
                    case 1: // print
                        printf("R1 = %d\n", R[1]);
                        pc++;
                        break;
                    case 3: // 跳转
                        if(R[0] != R[rs2_code]) pc = addr;
                        else pc++;
                        break;
                }
                curr_state = STATE_WB;
                break;

            case STATE_WB:
                // 状态4：写回完成，回到取指状态，下一条指令
                curr_state = STATE_IF;
                break;
        }
    }
    return 0;
}
