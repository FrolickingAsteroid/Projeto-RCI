//test.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
extern int errno;

int main(void) {
	char buffer[128];

	if(gethostname(buffer,128)==-1) {
		fprintf(stderr,"error: %s\n",stderr(errno));
	} else {
		printf("host name: %s\n",buffer);
	}
	exit(0);

}


/*! TODO: I forgor..
 * 
 *
 *
 *
 *
