#include "hash.h"

// the hash key generation algorithm
int hash_keygen(hash_k *hk, lhe_par *par)
{
	bn_t zt;
	bn_null(zt);

	hk->a=malloc(sizeof(bn_t)*2);
	hk->b=malloc(sizeof(bn_t)*par->ni);
	hk->K=malloc(sizeof(g2_t*));

	for (int i=0;i<2;i++)
	{
		bn_rand_mod(zt,par->q);
		bn_copy(hk->a[i],zt);
	}

	for (int j=0;j<par->ni;j++)
	{
		bn_rand_mod(zt,par->q);
		bn_copy(hk->b[j],zt);
	}

	g2_t zg2;
	g2_new(zg2);
	g2_set_infty(zg2); 

	for (int i=0; i<2; i++)
	{
		hk->K[i]=malloc(sizeof(g2_t)*par->ni);
		for (int j=0;j<par->ni;j++)
		{
			bn_mul(zt,hk->a[i],hk->b[j]);
			bn_mod_basic(zt,zt,par->q);
			g2_mul(zg2,par->h,zt);
			g2_copy((hk->K[i])[j],zg2);
		}
	}

	bn_free(zt);
	g2_free(zg2);
	return 0;
}

// compute the hash digest
int hash_H(g2_t hv, lhe_c * c, hash_k *hk, int mode, lhe_par *par)
{ 
	g2_t zg2;
	g2_new(zg2);
	g2_set_infty(zg2);
   
	fq_t zq;
	fq_init(zq,par->ctx);

	bn_t s, E;
	bn_new(s);
	bn_new(E);
	bn_zero(s);
	bn_zero(E);

	if (mode==0) 
	{
		for (int j=0;j<par->ni;j++)
		{
			fq_poly_get_coeff(zq,c->p0,j,par->ctx); 
			fq2bn(E,zq,par->ctx);
			bn_mul(E,E,hk->a[0]);
			bn_mul(E,E,hk->b[j]);
			bn_add(s,s,E);
			bn_mod_basic(s,s,par->q);
		}    

		for (int j=0;j<par->ni;j++)
		{
			fq_poly_get_coeff(zq,c->p1,j,par->ctx);
			fq2bn(E,zq,par->ctx);
			bn_mul(E,E,hk->a[1]);
			bn_mul(E,E,hk->b[j]);
	 		bn_add(s,s,E);
			bn_mod_basic(s,s,par->q);
		}  

		g2_mul(zg2,par->h,s);
		g2_copy(hv,zg2);   
	}
    
	else 
	{
		for (int j=0;j<par->ni;j++)
		{
			fq_poly_get_coeff(zq,c->p0,j,par->ctx);
			fq2bn(E,zq,par->ctx); 
			g2_mul(zg2,hk->K[0][j],E);  
			g2_add(hv,hv,zg2);
			g2_norm(hv,hv);
		}    

		for (int j=0;j<par->ni;j++)
		{
			fq_poly_get_coeff(zq,c->p1,j,par->ctx);
			fq2bn(E,zq,par->ctx);
			g2_mul(zg2,hk->K[1][j],E);  
			g2_add(hv,hv,zg2);
			g2_norm(hv,hv);
		}   
	}
	return 0;
}



// the homomorphic evaluation algorithm of the hash function
// L -> r
// HV -> v
int hash_eval(g2_t hv, g2_t *HV, fq_t *L, int d, lhe_par *par)
{
	g2_t zg2;
	g2_new(zg2);
	g2_set_infty(zg2);
   
	bn_t zb;
	bn_new(zb);
  
 	fq2bn(zb,L[0],par->ctx);
	g2_mul(hv,HV[0],zb);

	for (int i=1;i<d;i++)
	{
		fq2bn(zb,L[i],par->ctx);
		g2_mul(zg2,HV[i],zb);
		g2_add(hv,hv,zg2);
	}
   
	bn_free(zb);
	g2_free(zg2);
   
	return 0;
}
