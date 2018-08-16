#include "mf.h"
int mf_keygen(g1_t **T, skm *skm, fq_mat_t M, lhe_par *par)
{

	int n = fq_mat_nrows(M,par->ctx);
	lhe_keygen(skm->s,par);
	skm->hk = malloc(sizeof(hash2_k));
	hash2_keygen(skm->hk,par);
	bn_new(skm->a);
	bn_rand_mod(skm->a,par->q);

	skm->fk1 = malloc(sizeof(prfk)*n);
	skm->fk2 = malloc(sizeof(prfk)*n);

//compute fk1(i) and fk2(i)
	for(int i=0;i<n;i++)
	{
		skm->fk1[i] = malloc(sizeof(prfk));
		skm->fk2[i] = malloc(sizeof(prfk));
		prfkg(skm->fk1[i],par->q);
		prfkg(skm->fk2[i],par->q);
	}

//compute T
	bn_t mij;
	bn_new(mij);

	hv2 *fkx = malloc(sizeof(hv2));
	
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{

			fq2bn(mij,fq_mat_entry(M,i,j),par->ctx);			
			prf(fkx,skm->fk2[i],skm->fk1[j],par);
			g1_mul(T[i][j],par->g,skm->a);
			g1_mul(T[i][j],T[i][j],mij);
			g1_add(T[i][j],T[i][j],fkx->h1);
			g1_norm(T[i][j],T[i][j]);
		}
	}
	bn_free(mij);
	
	free(fkx);
	return 0;
}


int mf_pgen(lhe_c **sigmax, prfk *tau, skm *skm, bn_t *x, int n, lhe_par *par)
{
	fmpz_t fx1;
	fmpz_init(fx1);
	fq_t fx;
	fq_init(fx,par->ctx);
	fq_poly_t px;
	fq_poly_init(px,par->ctx);
	
	for(int i=0;i<n;i++)
	{
		bn2fq(fx,x[i],par->ctx);
		fq_poly_set_fq(px,fx,par->ctx);
		lhe_enc(sigmax[i],skm->s,px,par);
	}

	bn_t out,t1,t2;
	bn_new(out);
	bn_new(t1);
	bn_new(t2);
	
	bn_zero(tau->k1);
	bn_zero(tau->k2);
	for(int j = 0;j<n;j++)
	{
		mucom(out,sigmax[j],skm->hk->a,skm->hk->b,par->ctx);
		bn_mul(t1,out,skm->fk1[j]->k1);
		bn_mul(t2,out,skm->fk1[j]->k2);
		bn_add(tau->k1,tau->k1,t1);
		bn_add(tau->k2,tau->k2,t2);
		bn_mod_basic(tau->k1,tau->k1,par->q);
		bn_mod_basic(tau->k2,tau->k2,par->q);
		
	}

	fmpz_clear(fx1);
	fq_clear(fx,par->ctx);
	fq_poly_clear(px,par->ctx);
	bn_free(out);
	bn_free(t1);
	bn_free(t2);
	return 0;
}

int mf_comp(sigma_y *sy, fq_mat_t M, g1_t **T, hash2_k *hk, lhe_c **sigmax, lhe_par *par)
{
	int n = fq_mat_nrows(M,par->ctx);
	sy->g = malloc(sizeof(lhe_c)*n);
	sy->s = malloc(sizeof(gt_t)*n);

	fq_poly_t t0;
	fq_poly_t t1;
	fq_poly_init(t0,par->ctx);
	fq_poly_init(t1,par->ctx);

	for(int i=0;i<n;i++)
	{
		sy->g[i] = malloc(sizeof(lhe_c));
		fq_poly_init(sy->g[i]->p0,par->ctx);
		fq_poly_init(sy->g[i]->p1,par->ctx);
		fq_poly_zero(sy->g[i]->p0,par->ctx);
		fq_poly_zero(sy->g[i]->p1,par->ctx);
		
		for(int j=0;j<n;j++)
		{
			fq_poly_scalar_mul_fq(t0,sigmax[j]->p0,fq_mat_entry(M,i,j),par->ctx);
			fq_poly_scalar_mul_fq(t1,sigmax[j]->p1,fq_mat_entry(M,i,j),par->ctx);
			
			fq_poly_add(sy->g[i]->p0,sy->g[i]->p0,t0,par->ctx);
			fq_poly_add(sy->g[i]->p1,sy->g[i]->p1,t1,par->ctx);

		}
	}
	gt_t temp;
	gt_new(temp);
	hv2 *hku = malloc(sizeof(hv2));

	for(int i=0;i<n;i++)
	{
		gt_set_unity(sy->s[i]);
		for(int j=0;j<n;j++)
		{
			hash2_H(hku,sigmax[j],hk,0,par);
			pc_map(temp,T[i][j],hku->h2);
			gt_mul(sy->s[i],sy->s[i],temp);
		}
	}
	fq_poly_clear(t0,par->ctx);
	fq_poly_clear(t1,par->ctx);
	gt_free(temp);
	free(hku);
	return 0;
}

int mf_vrfy(fq_t *y, skm *skm, prfk *tau, sigma_y *sy, int n, lhe_par *par)
{
	int flag = 0;

	gt_t wi,right;
	gt_new(wi);
	gt_new(right);

	bn_t wb,wt,out;
	bn_new(wb);
	bn_new(wt);
	bn_new(out)
	
	
	for(int i=0;i<n;i++)
	{
		bn_mul(wb,tau->k1,skm->fk2[i]->k1);
		bn_mul(wt,tau->k2,skm->fk2[i]->k2);
		bn_add(wb,wb,wt);
		bn_mod_basic(wb,wb,par->q);
		gt_exp(wi,par->gt,wb);
		
		mucom(out,sy->g[i],skm->hk->a,skm->hk->b,par->ctx);
		bn_mul(out,out,skm->a);
		bn_mod_basic(out,out,par->q);
		gt_exp(right,par->gt,out);
		gt_mul(right,right,wi);
		flag = gt_cmp(sy->s[i],right);
		
		if(flag)
			break;
	}

	if(flag==0)
	{
		for(int i=0;i<n;i++)
		{
			fq_init(y[i],par->ctx);
			lhe_dec(y[i],skm->s,sy->g[i],par);
		}
	}
	gt_free(wi);
	gt_free(right);
	bn_free(wb);
	bn_free(wt);
	bn_free(out);
	return flag;
}

