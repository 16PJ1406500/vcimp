#include <time.h>
#include <stdio.h>
#include "mvm.h"

int main(int argc, char **argv)
{

	sinit();
	printf("begin test :\n");
	mvm_par *par = malloc(sizeof(*par));
	mvmp_new(par);

	int n = 100;
	int d = 100;
	clock_t s1,s2,s3,s4,e1,e2,e3,e4;
	fq_mat_t M;
	fq_mat_init(M,n,d,par->ctx);
	fq_mat_randz(M,par->ctx,par->p);
	
	gt_t PKm;
	gt_new(PKm);
	
	g1_t **W = malloc(sizeof(g1_t*)*n);

	for(int i=0;i<n;i++)
	{
		W[i] = malloc(sizeof(g1_t)*d);
	}

	prfk *K = malloc(sizeof(prfk));
//keygen
	s1 = clock();
	mvm_keygen(PKm,W,K,M,par);
	e1 = clock();
	printf("The keygen time was: %f\n", (double)(e1 - s1) /CLOCKS_PER_SEC);

//progen
	gt_t *VKx = malloc(sizeof(gt_t)*n);
	bn_t *x = malloc(sizeof(bn_t)*d);

	for(int i=0;i<d;i++)
		bn_rand_mod(x[i],par->p);
	s2 = clock();
	mvm_pgen(VKx,n,d,K,x,par);
	e2 = clock();
	printf("The probgen time was: %f\n", (double)(e2 - s2) /CLOCKS_PER_SEC);	
	
//compute

	fq_mat_t y;
	fq_mat_init(y,n,1,par->ctx);
	
	g1_t *V = malloc(sizeof(g1_t)*n);
	s3 = clock();
	mvm_comp(y,V,W,M,x,par);
	e3 = clock();
	printf("The compute time was: %f\n", (double)(e3 - s3) /CLOCKS_PER_SEC);
	
//verify
	int flag;
	s4 = clock();
	flag = mvm_vrfy(PKm,VKx,y,V,par);
	e4 = clock();
	printf("The verify time was: %f\n", (double)(e4 - s4) /CLOCKS_PER_SEC);
	printf("The client time was: %f\n", (double)(e1+e2+e4 - s4-s2-s1) /CLOCKS_PER_SEC);
	printf("flag is :%d\n",flag);
//free
	gt_free(PKm);
	mvmp_free(par);
	for(int i=0;i<n;i++)
	{
		free(W[i]);
	}
	free(W);

	free(K);
	free(VKx);
	fq_mat_clear(M,par->ctx);
	fq_mat_clear(y,par->ctx);
	free(V);
	return (0);
}

