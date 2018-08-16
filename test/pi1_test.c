#include "pi1.h"
#include <time.h>

int main(int argc, char **argv)
{
//initialize the project        
	if (sinit())
	{
		printf("Testing FAILED\n");
		printf("Problem initializing the library\n");
		return 1;
        }

//generate the parameters	
	cgpp *par = malloc(sizeof(*par));
	cgpp_new(par);
	
	printf("test pi1\n");
	int m = 100;
	int d = 100;

	clock_t s1,s2,s3,s4,e1,e2,e3,e4;
//keygen
	fq_mat_t F;
	fq_mat_init(F,m,d,par->ctx);
	fq_mat_randz(F,par->ctx,par->q);
	
	fq_mat_t Y;
	fq_mat_init(Y,m,1,par->ctx);

	vkf *vkf = malloc(sizeof(*vkf));

	s1 = clock();	
	pi1_keygen(vkf,F,par);
	e1 = clock();
	printf("The keygen time was: %f\n", (double)(e1 - s1) /CLOCKS_PER_SEC);
//compute
	bn_t x[d];
	for(int i=0;i<d;i++){
		bn_rand_mod(x[i],par->q);
	}
	s3 = clock();
	pi1_comp(Y,F,x,par);
	e3 = clock();
	printf("The compute time was: %f\n", (double)(e3 - s3) /CLOCKS_PER_SEC);
//verify
	s4 = clock();
	int flag = pi1_vrfy(vkf,Y,x,par);
	e4 = clock();
	printf("The verify time was: %f\n", (double)(e4 - s4) /CLOCKS_PER_SEC);
	printf("The client time was: %f\n", (double)(e1+e2+e4 - s4-s2-s1) /CLOCKS_PER_SEC);
	printf("flag is:%d\n",flag);
	
	sclose();
	return 0;
}
