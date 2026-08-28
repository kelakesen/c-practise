#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
uint32_t sign_extension(uint32_t imm);
uint32_t Byte_select(uint32_t byte_select,uint32_t data);
uint32_t SB(uint32_t byte_select,uint32_t M,uint32_t R);
// 1. 定义全部CPU运行状态
typedef enum {
    STATE_IF,    // 取指
    STATE_ID,    // 译码拆分指令
    STATE_EX,    // 运算执行
    STATE_WB,    // 写回寄存器
} CPU_STATE;

int main(int argc,char *argv[]){
    uint64_t pc=0;
    uint32_t *M = malloc(1024 * 1024 *1024);
    uint32_t R[32] = {0};


    FILE *fp = fopen("logisim-bin/sum.bin","rb");
    if(fp)printf("open success\n");
    if(!fp){
	  printf("open error\n");
	  exit(1);
    }
    fseek(fp,0,SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    size_t n = fread(M,1,file_size,fp);
    printf("%ld\n",n);
    for(int j=135;j<140;j++){
	    printf("%d:%"PRIx32"\n",j,M[j]);
    }


    uint32_t inst, opcode, rd, rs1, rs2, addr,funct3,funct7,word_addr,byte_select;
    uint32_t imm;

    CPU_STATE curr_state = STATE_IF; // 初始状态：取指

    //for(int i=0;i<30;i++)
    while(1)
    {
        switch(curr_state)
        {
            case STATE_IF:
                // 状态1：取指令
                inst = M[pc>>2];
		printf("inst:%"PRIx32"\n",inst);
		printf("pc:%"PRIu64"\n",pc);
                // 切换下一个状态：译码
                curr_state = STATE_ID;
                break;

            case STATE_ID:
                // 状态2：拆分指令字段（你原来的位运算）
                opcode = inst & 0x7F;
		printf("opcode:%"PRIx32"\n",opcode);
			switch(opcode){
				case 0x67:
				case 0x03:
				case 0x13:
					imm = (inst >> 20) & 0xFFF;
					rs1 = (inst >> 15) & 0x1F;
					funct3 = (inst >> 12) & 0x7;
					rd = (inst >> 7) & 0x1F;
					break;
				case 0x23:
					imm = sign_extension(((inst >> 25) << 5)|((inst & 0xF80)>>7));
					rs1 = (inst >> 15) & 0x1F;
					funct3 = (inst >> 12) & 0x7;
					rs2 = (inst >> 20) & 0x1F;
					break;
				case 0x37://LUI
					rd = (inst >> 7) & 0x1F;
					imm = (inst >> 12);
					funct3 = 0x0;
					break;
				case 0x33://ADD
					rs1 = (inst >> 15) & 0x1F;
					funct3 = (inst >> 12) & 0x7;
					rd = (inst >> 7) & 0x1F;
					rs2 = (inst >> 20) & 0x1F;
					funct7 = (inst >> 25);
					break;
				case 0x7F://ebreak
					printf("a0:%"PRIu32"\n",R[10]);
					break;
				default:
					printf("ID error\n");
					exit(1);
					break;
				}
                curr_state = STATE_EX; // 跳转执行阶段
                break;

            case STATE_EX:
                // 状态3：执行运算、跳转、立即数赋值
		uint32_t funct3_opcode = (funct3<<7) + opcode;

		//printf("funct3:%"PRIx32"\n",funct3);
		//printf("%"PRIx32"\n",funct3_opcode);
	
                switch(funct3_opcode){
                    case 0x67://JALR 
			    uint32_t pc_temp = pc + 4;
			    pc = (R[rs1] + sign_extension(imm)) & ~3U;
			    R[rd] = pc_temp;

			    printf("JALR:%"PRIu64"\n",pc);
                        break;
                    case 0x103://LW
			   word_addr = (R[rs1] + sign_extension(imm))>>2;//
			   R[rd] = M[word_addr];
			   pc += 4; 
			break;
		    case 0x203://LBU
			   word_addr = (R[rs1] + sign_extension(imm))>>2;//
			   byte_select = (R[rs1] + sign_extension(imm)) & 0x3;
			   R[rd] = Byte_select(byte_select,M[word_addr]);//zero_extension
			   pc += 4;
                        break;
		    case 0x13://ADDI 
			   R[rd] = R[rs1] + sign_extension(imm); 
			   pc += 4;
                        break;
		    case 0x123://SW
			   addr = (R[rs1] + imm)>>2;
			   M[addr] = R[rs2];
			   pc += 4; 
			break; 
	  	    case 0x23://SB
			   word_addr = (R[rs1] + imm)>>2;
			   byte_select = (R[rs1] + imm) & 0x3;
			   M[word_addr] = SB(byte_select,M[word_addr],R[rs2]&0xFF);
			   pc += 4;
			break;
		    case 0x37://LUI
			   R[rd] = (imm << 12);
			   pc += 4;
			break;
		    case 0x33://ADD
			   R[rd] = R[rs1] + R[rs2];
			   pc += 4;
		        break;
		    case 0x7F://ebreak
			   printf("END\n");
			   fclose(fp);
			   exit(0);
			break;
		    default:
			printf("EX nothing\n");
			fclose(fp);
			exit(1);
			break;
                }
                curr_state = STATE_WB;
                break;

            case STATE_WB:
                // 状态4：写回完成，回到取指状态，下一条指令
		R[0] = 0;
                curr_state = STATE_IF;
                break;
        }
    }
    fclose(fp);
    return 0;

}



uint32_t sign_extension(uint32_t imm){

	if((imm & 0x800) == 0){return imm;}
	else{return (imm | 0xFFFFF000);}

}
uint32_t Byte_select(uint32_t byte_select,uint32_t data){
	switch(byte_select){
		case 0:return (data & 0xFF);break;
		case 1:return (data & 0xFF00)>>8;break;
		case 2:return (data & 0xFF0000)>>16;break;
		case 3:return (data & 0xFF000000)>>24;break;
		default:printf("byte_select error\n");exit(1);break;
		}
}
uint32_t SB(uint32_t byte_select,uint32_t M,uint32_t R){
	switch(byte_select){
		case 0:return (M & 0xFFFFFF00) + R;break;
		case 1:return (M & 0xFFFF00FF) + (R<<8);break;
		case 2:return (M & 0xFF00FFFF) + (R<<16);break;
		case 3:return (M & 0x00FFFFFF) + (R<<24);break;
		default:printf("SB error\n");exit(1);break;
	}
}

