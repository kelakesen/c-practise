#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fp = fopen("fff","r+");
	int x = 1223;
	fwrite(&x,4,1,fp);

//	fseek(fp,0,SEEK_SET);
	fprintf(fp,"%d\n",x);

//	fclose(fp);
	printf("hello\n");

	*(int*)0=0;
}

