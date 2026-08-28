#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdint.h>
int main(int argc ,char*argv[]){
	//fwrite(p,sizeof(char),5,fp);




    uint32_t *M = malloc(1024 * 1024 *1024);
    if(M)printf("malloc\n");
    FILE *fp = fopen("logisim-bin/sum.bin","rb");
    if(!fp)printf("error\n");
    fseek(fp,0,SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    fread(M,1,file_size,fp);
    printf("%"PRIx32"\n",M[0]);


    free(M);
    fclose(fp);
	return 0;


}
