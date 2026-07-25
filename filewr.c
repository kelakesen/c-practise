#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc ,char*argv[]){
	char* p="HELLO";
	char* filename = "gao";
	FILE* file=fopen(filename,"r+");
	fwrite(p,sizeof(char),5,file);
	exit(0);



}
