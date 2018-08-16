#include "poly_eval.h"
#include <time.h>
int main(int argc, char **argv)
{

// initialize the project /////////////////////////////////////////////////        
        if (sinit())
        {
                printf("Testing FAILED\n");
                printf("Problem initializing the library\n");
               return 1;
        }


  pep *par=malloc(sizeof(*par));
  pep_new(par);
printf("\n--------------------------------------begin test-------------------------\n\n\n");
 	clock_t s1,s2,s3,s4,e1,e2,e3,e4;
        // rand polynomial algorithm
		    fq_poly_t A;
		    fq_poly_init(A,par->ctx);
        fq_poly_rand(A,100,par->ctx,par->p);

   
        // print 


              
        // test the vc_keygen algorithm
        pk_a *pka=malloc(sizeof(*pka));
        ek_a *eka=malloc(sizeof(*eka));

	s1 = clock();
        vc_keygen(pka,eka,A,par);
	e1 = clock();
	printf("The keygen time was: %f\n", (double)(e1 - s1) /CLOCKS_PER_SEC);
       

        // test the vc_pgen algorithm
        vk_x *vkx=malloc(sizeof(*vkx));
        // generate a random message vector of d elements
        bn_t x;
        bn_new(x);
        bn_rand_mod(x,par->p);
        
        // compute the problem instance
	s2 = clock();
        vc_pgen(pka,vkx,x,par);
	e2 = clock();
	printf("The probgen time was: %f\n", (double)(e2 - s2) /CLOCKS_PER_SEC);

        // test the vc_comp algorithm
        g2_t y;
        g2_new(y);
        g2_set_infty(y);
	s3 = clock();
        vc_comp(y,vkx,x,eka,par);
	e3 = clock();
	printf("The compute time was: %f\n", (double)(e3 - s3) /CLOCKS_PER_SEC);
        // print
        
		// gt_print(y);



       // test the vc_vrfy algorithm  
   	s4 = clock();
	int flag=vc_vrfy(y,vkx,par);
	e4 = clock();

	
	printf("The verify time was: %f\n", (double)(e4 - s4) /CLOCKS_PER_SEC);
	printf("The client time was: %f\n", (double)(e1+e2+e4 - s4-s2-s1) /CLOCKS_PER_SEC); 
 	printf("\n\n the verification result is flag=%d\n\n",flag);

        return 0;
}

