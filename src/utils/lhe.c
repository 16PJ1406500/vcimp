#include "lhe.h"

int lhe_keygen(fq_poly_t s, lhe_par *par)
{
  	fq_poly_rand(s,par->ni,par->ctx,par->r);
	return 0;
}

int lhe_enc(lhe_c *c, fq_poly_t s, fq_poly_t m,lhe_par *par)
{
	fq_poly_t a, e;
	fq_poly_rand(a,par->ni,par->ctx,par->q); 
	fq_poly_rand(e,par->ni,par->ctx,par->r);

	fq_poly_init(c->p0,par->ctx);
	fq_poly_init(c->p1,par->ctx);

	fq_poly_neg(c->p1,a,par->ctx);
	  
	fq_poly_t v;
	fq_poly_init(v,par->ctx);
//compute p0
	fq_poly_mul(c->p0,a,s,par->ctx);	
	fq_poly_mul(v,e,par->modp,par->ctx);
	fq_poly_add(c->p0,c->p0,v,par->ctx);
	fq_poly_add(c->p0,c->p0,m,par->ctx);
// reduction modulo x^n+1
	fq_poly_rem(c->p0,c->p0,par->modf,par->ctx);
	fq_poly_clear(a,par->ctx);
	fq_poly_clear(e,par->ctx);
	return 0;   
}


int lhe_dec(fq_t m, fq_poly_t s, lhe_c *c, lhe_par *par)
{
	bn_t mb;
	bn_new(mb);
	fq_poly_t m1;
	fq_poly_init(m1,par->ctx);
	fq_poly_init(m1,par->ctx);
	fq_poly_mul(m1,s,c->p1,par->ctx);
	fq_poly_add(m1,m1,c->p0,par->ctx);

	fq_poly_get_coeff(m,m1,0,par->ctx);
	
	fq2bn(mb,m,par->ctx);
	bn_mod_basic(mb,mb,par->p);
	bn2fq(m,mb,par->ctx);
	
	fq_poly_clear(m1,par->ctx);
	bn_free(mb);
	return 0;   
}

int lhe_eval(lhe_c *c, lhe_c **C, fq_t *L, int d, lhe_par *par)
{
	fq_poly_init(c->p0,par->ctx);
	fq_poly_init(c->p1,par->ctx);
  
	fq_poly_t z;
	fq_poly_init(z,par->ctx);

	for (int i=0; i<d; i++)
	{
		fq_poly_scalar_mul_fq(z,C[i]->p0,L[i],par->ctx);
		fq_poly_add(c->p0,c->p0,z,par->ctx);
		fq_poly_rem(c->p0,c->p0,par->modf,par->ctx);
		fq_poly_scalar_mul_fq(z,C[i]->p1,L[i],par->ctx);
		fq_poly_add(c->p1,c->p1,z,par->ctx);
		fq_poly_rem(c->p1,c->p1,par->modf,par->ctx);
	}
	//release the memory
	fq_poly_clear(z,par->ctx);
	return 0;
}

