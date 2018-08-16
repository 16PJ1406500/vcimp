#include <stdio.h>
#include <time.h>
#include "mf.h"

int main(int argc, char **argv)
{
	
	clock_t s1,s2,s3,s4,e1,e2,e3,e4;

	sinit();

	printf("begin test :\n");
	lhe_par *par = malloc(sizeof(*par));
	lhep_new(par);

	int n = 100;

	fq_mat_t M;
	fq_mat_init(M,n,n,par->ctx);
	fq_mat_randz(M,par->ctx,par->p);
//keygen
	skm *skm = malloc(sizeof(*skm));
	g1_t **T = malloc(sizeof(g1_t*)*n);
	for(int i=0;i<n;i++)
	{
		T[i] = malloc(sizeof(g1_t)*n);
	}
	s1 = clock();
	mf_keygen(T,skm,M,par);
	e1 = clock();
	printf("The keygen time was: %f\n", (double)(e1 - s1) /CLOCKS_PER_SEC);

//probgen

	lhe_c **sigmax = malloc(sizeof(lhe_c)*n);
	for(int i = 0;i<n;i++)
		sigmax[i] = malloc(sizeof(lhe_c));
	prfk *tau = malloc(sizeof(prfk));
	bn_t *x = malloc(sizeof(bn_t)*n);

	for(int i=0;i<n;i++)
	{
		bn_rand_mod(x[i],par->p);

	}
	s2 = clock();
	mf_pgen(sigmax,tau,skm,x,n,par);
	e2 = clock();
	printf("The probgen time was: %f\n", (double)(e2 - s2) /CLOCKS_PER_SEC);

//compute
	sigma_y *sy = malloc(sizeof(sigma_y));
	s3 = clock();
	mf_comp(sy,M,T,skm->hk,sigmax,par);
	e3 = clock();
	printf("The compute time was: %f\n", (double)(e3 - s3) /CLOCKS_PER_SEC);
//verify
	fq_t *y = malloc(sizeof(fq_t)*n);

	s4 = clock();
	int flag = mf_vrfy(y,skm,tau,sy,n,par);
	e4 = clock();
	printf("The verify time was: %f\n", (double)(e4 - s4) /CLOCKS_PER_SEC);
	printf("The client time was: %f\n", (double)(e1+e2+e4 - s4-s2-s1) /CLOCKS_PER_SEC);
	printf("flag is:%d\n",flag);

	lhep_free(par);
	free(par);
	free(skm);
	for(int i=0;i<n;i++)
	{

		free(T[i]);
	}
	free(T);
	for(int i = 0;i<n;i++)
	{	
		free(sigmax[i]);
	}	
	free(sigmax);
	free(tau);
	free(x);
	free(sy);
	free(y);
	return 0;
}

