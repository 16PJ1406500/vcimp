#include "pi3.h"
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

	lhe_par *par=malloc(sizeof(*par));
	lhep_new(par);
printf("\n--------------------------------------begin test-------------------------\n\n\n");

        //test the rand matrix algorithm
        int mat_m=100;
        int mat_d=100;
	clock_t s1,s2,s3,s4,e1,e2,e3,e4;

        fq_mat_t F;
        fq_mat_init(F,mat_m,mat_d,par->ctx);
        fq_mat_randz(F,par->ctx,par->p);

        //test the vc_keygen algorithm
        vc_k *vck=malloc(sizeof(*vck));
        vck->m=mat_m;
        vck->d=mat_d;

	s1 = clock();
        pi3_keygen(vck,F,par);
	e1 = clock();
	printf("The keygen time was: %f\n", (double)(e1 - s1) /CLOCKS_PER_SEC);        


        //test the vc_pgen algorithm

        vc_p *vcp=malloc(sizeof(*vcp));
        //generate a random message vector of d elements
        bn_t x[vck->d];
        for (int j=0;j<vck->d;j++)
          bn_rand_mod(x[j],par->p);
        // compute the problem instance
        vcp->c=x;

        //test the vc_comp algorithm
        lhe_c **nv=malloc(sizeof(lhe_c)*vck->m);
	for(int i=0;i<mat_m;i++)
	{
		nv[i] = malloc(sizeof(lhe_c));
	}
        fq_poly_t m1;
        fq_poly_init(m1,par->ctx);

	s3 = clock();
        pi3_comp(nv,vck,vcp,par);
	e3 = clock();
	printf("The compute time was: %f\n", (double)(e3 - s3) /CLOCKS_PER_SEC);

       // test the vc_vrfy algorithm
	fq_t *y = malloc(sizeof(fq_t)*mat_m);
	for(int i=0;i<mat_m;i++)
	{
		fq_init(y[i],par->ctx);
	}
	s4 = clock();
	int flag=pi3_vrfy(y,vck,vcp,nv,par);
	e4 = clock();
	printf("The verify time was: %f\n", (double)(e4 - s4) /CLOCKS_PER_SEC);
	printf("The client time was: %f\n", (double)(e1+e2+e4 - s4-s2-s1) /CLOCKS_PER_SEC);
	printf("\n\n the verification result is flag=%d\n\n",flag);

	free(vck);
	free(vcp);
	for(int i=0;i<mat_m;i++)
	{
		free(nv[i]);
	}
	free(nv);
	for(int i=0;i<mat_m;i++)
	{
		fq_clear(y[i],par->ctx);
	}
	free(y);
        return 0;
}
