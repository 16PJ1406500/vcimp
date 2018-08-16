# include "hash2.h"

// the hash key generation algorithm
int hash2_keygen(hash2_k *hk, lhe_par *par)
{
//	bn_t zt;
	int dx = 2*(par->ni)-1;
	int dy = 2;
	bn_new(hk->a);
	bn_new(hk->b);
	bn_rand_mod(hk->a,par->q);
	bn_rand_mod(hk->b,par->q);

	bn_t ai;
	bn_t bj;
	bn_new(ai);
	bn_new(bj);
	
//compute public key
	hk->K1 = malloc(sizeof(g1_t*)*dy);
	hk->K2 = malloc(sizeof(g2_t*)*dy);
	for (int i=0;i<dy;i++)
	{
		hk->K1[i] = malloc(sizeof(g1_t)*dx);
		hk->K2[i] = malloc(sizeof(g2_t)*dx);
		bn_mxp_dig(ai,hk->a,i,par->q);
//		printf ("\nprint ai\n");
//		bn_print(ai);
		
		for (int j=0;j<dx;j++)
		{
			bn_mxp_dig(bj,hk->b,j,par->q);
			g1_mul(hk->K1[i][j],par->g,ai);
			g1_norm(hk->K1[i][j],hk->K1[i][j]);
			g1_mul(hk->K1[i][j],hk->K1[i][j],bj);
			g1_norm(hk->K1[i][j],hk->K1[i][j]);
			g2_mul(hk->K2[i][j],par->h,ai);
			g2_norm(hk->K2[i][j],hk->K2[i][j]);
			g2_mul(hk->K2[i][j],hk->K2[i][j],bj);
			g2_norm(hk->K2[i][j],hk->K2[i][j]);
		}
	}

	bn_free(ai);
	bn_free(bj);
	return 0;
}

//compute mu(b,a)
int mucom(bn_t out, lhe_c *c, bn_t a, bn_t b, fq_ctx_t ctx)
{
	fq_t temp,outf,fa,fb;
	fq_init(temp,ctx);
	fq_init(outf,ctx);
	fq_init(fa,ctx);
	fq_init(fb,ctx);

	bn2fq(fa,a,ctx);
	bn2fq(fb,b,ctx);
	
	//x
	fq_poly_evaluate_fq(outf,c->p0,fb,ctx);
		
	//y
	fq_poly_evaluate_fq(temp,c->p1,fb,ctx);
	fq_mul(temp,temp,fa,ctx);
	fq_add(outf,outf,temp,ctx);
	fq2bn(out,outf,ctx);

	//clean
	fq_clear(temp,ctx);
	fq_clear(outf,ctx);
	fq_clear(fa,ctx);
	fq_clear(fb,ctx);
	
}

//compute the hash digest
int hash2_H(hv2 *hv, lhe_c *c, hash2_k *hk, int mode, lhe_par *par)
{ 
	
	bn_t outb;
	bn_new(outb);
//	fq_t zq;
	if (mode==0) //k
	{	

		mucom(outb,c,hk->a,hk->b,par->ctx);
		//fq2bn(outb,out,par->ctx);
		g1_mul(hv->h1,par->g,outb);
		g1_norm(hv->h1,hv->h1);
		g2_mul(hv->h2,par->h,outb);
		g2_norm(hv->h2,hv->h2);
    }
/*
	else
	{
		for(int i=0;i<par->ni;i++)
		{
			fq_poly_get_coeff(zq,c->p0,j,par->ctx);
		}
	}
*/
	bn_free(outb);
    return 0;
}

