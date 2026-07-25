#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char*argv[]){

uint8_t pc=0;
uint8_t M[16]={0b10010000,0b10100000,0b10110001,0b00101011,0b00010110,0b11010010,0b01000000};
uint8_t R[4];
R[0]=atoi(argv[1]);
	while(pc!=7){
		uint8_t inst = M[pc];
        // 正确位运算拆分指令（替代十进制除法
	// ）
	uint8_t opcode = (inst >> 6) & 0x03;   // 高2位
        uint8_t rd     = (inst >> 4) & 0x03;   // 2位
        uint8_t rs1    = (inst >> 2) & 0x03;   // 2位
        uint8_t rs2    = inst & 0x03;          // 最低2位
        uint8_t imm    = inst & 0x0F;          // 低4位 (li用)
        uint8_t addr   = (inst >> 2) & 0x0F;   // 重要！只取低4位（位5-2）

	uint8_t rd_code;
	switch(rd){
		case 0b00:
			rd_code = 0;
			break;
		case 0b01:
			rd_code = 1;
			break;
		case 0b10:
			rd_code = 2;
			break;
		case 0b11:
			rd_code = 3;
			break;
		default:
			printf("rd_code error");
			break;
		}

	uint8_t rs1_code;
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

	uint8_t rs2_code;
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


	switch(opcode){
		case 0b00:
			R[rd_code]=R[rs1_code]+R[rs2_code];
			pc+=1;
			break;
		case 0b10:
			R[rd_code]=imm;
			pc+=1;
			break;
		case 0b11:
			if(R[0]!=R[rs2]){
				pc = addr;
			
			}
			else{pc+=1;}
			break;
		case 0b01:
			printf("%d",R[1]);
			pc+=1;
			break;
		default:
			printf("opcode error");
			break;
		}
	}
}



