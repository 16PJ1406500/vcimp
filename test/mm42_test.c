#include <stdio.h>
#include <time.h>
#include "mm42.h"
int main()
{
	sinit();
	
	int n = 100;
	int m = 100;
	clock_t s1,s2,s3,s4,e1,e2,e3,e4;

	mm_par *par = malloc(sizeof(*par));

	
	mmp_new(par,n);
	
//setup
	g1_t **N = malloc(sizeof(g1_t*)*n);
	for(int i=0;i<n;i++)
		N[i] = malloc(sizeof(g1_t)*m);
	
	gt_t *PK = malloc(sizeof(gt_t)*m);

	fq_mat_t M;
	fq_mat_init(M,n,m,par->ctx);

	fq_mat_randz(M,par->ctx,par->p);
	s1 = clock();
	mm_setup(N,PK,M,par);
	e1 = clock();
	printf("The setup time was: %f\n", (double)(e1 - s1) /CLOCKS_PER_SEC);

//progen
	bn_t *x = malloc(sizeof(bn_t)*m);
	for(int j=0;j<m;j++)
		bn_rand_mod(x[j],par->p);

	gt_t VKx;
	
	gt_new(VKx);
	s2 = clock();
	mm_pgen(VKx,x,PK,m);
	e2 = clock();
	printf("The probgen time was: %f\n", (double)(e2 - s2) /CLOCKS_PER_SEC);

//compute

	fq_mat_t Y;
	fq_mat_init(Y,n,m,par->ctx);

	g1_t Pi;
	g1_new(Pi);
	s3 = clock();
	mm_comp(Y,Pi,x,M,N,par);
	e3 = clock();
	printf("The compute time was: %f\n", (double)(e3 - s3) /CLOCKS_PER_SEC);
	
//verify
	s4 = clock();
	int flag = mm_vrfy(Y,Pi,VKx,par);
	e4 = clock();
	printf("The verify time was: %f\n", (double)(e4 - s4) /CLOCKS_PER_SEC);
	printf("The client time was: %f\n", (double)(e1+e2+e4 - s4-s2-s1) /CLOCKS_PER_SEC);
	printf("flag is:%d\n",flag);
//free
	
	mmp_free(par);
	for(int i=0;i<n;i++)
		free(N[i]);
	free(N);
	free(PK);
	fq_mat_clear(Y,par->ctx);
	g1_free(Pi);
	return 0;
}

