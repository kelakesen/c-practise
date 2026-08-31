#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void my_printf(char *fmt,...){

	va_list argp;
	va_start(argp,fmt);
	

	for(int i=0;fmt[i] != '\0';i++){

		if(fmt[i]=='%'){

			i++;

			switch(fmt[i]){

				case 'd':
					int out_int = va_arg(argp,int);
					char buf[100];
					snprintf(buf,sizeof(buf),"%d",out_int);
					for(int j=0;buf[j] != '\0';j++){
						fwrite(buf+j,sizeof(char),1,stdout);
					}
					break;
				case 'c':
					char out_ch = va_arg(argp,int);
					fwrite(&out_ch,sizeof(char),1,stdout);
					break;
				case 's':
					char *out_str = va_arg(argp,char*);
					for(int j=0;out_str[j] != '\0';j++){

						fwrite(out_str+j,sizeof(char),1,stdout);

					}
					break;
				default:
					va_end(argp);
					exit(1);
					break;
				}
		}
	}


	va_end(argp);
}

int main(){
	my_printf("%s %c %d","HELLO",'c',101);
}
