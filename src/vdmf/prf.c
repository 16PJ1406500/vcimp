#include "prf.h"

int prfkg(prfk *fk, bn_t mod)
{
//	bn_new(fk->k1);
//	bn_new(fk->k2);
	bn_rand_mod(fk->k1,mod);
	bn_rand_mod(fk->k2,mod);

	return 0;
}

int prf(hv2 *fkx, prfk *fk2, prfk *fk1, lhe_par *par)
{
	bn_t out,temp;	
	bn_new(out);
	bn_new(temp);
	
//fk1-u,v;fk2-a,b;	

	bn_mul(out,fk1->k1,fk2->k1);//ua
//	bn_mod_basic(out,out,par->q);
	bn_mul(temp,fk1->k2,fk2->k2);//vb
//	bn_mod_basic(temp,temp,par->q);
	bn_add(out,out,temp);//ua+vb
	bn_mod_basic(out,out,par->q);
	g1_mul(fkx->h1,par->g,out);
	g1_norm(fkx->h1,fkx->h1);
	g2_mul(fkx->h2,par->h,out);
	g2_norm(fkx->h2,fkx->h2);
	bn_free(out);
	bn_free(temp);

	return 0;
	
}

