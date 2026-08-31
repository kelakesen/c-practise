#include <stdio.h>
#include <errno.h>

int main(){
	goto error;


 out:
	fprintf(stderr,"stderr \n");
	fprintf(stdout,"stdout \n");


	FILE *tty = fopen("/dev/tty","w");

	fprintf(tty,"tty \n");

	return 0;
	
 error:
	errno = 22;
	perror("22");

	errno = -1;
	perror("-1");

	errno = 0;
	perror("0");

	goto out;
}
