#include "mvm.h"

int mvm_keygen(gt_t PKm, g1_t **W, prfk *K, fq_mat_t M, mvm_par *par)
{   
	int n = fq_mat_nrows(M,par->ctx);
	int d = fq_mat_ncols(M,par->ctx);

	//generate K	
	prfm_keygen(K,n,d,par);

	bn_t alpha;
	bn_new(alpha);
	bn_rand_mod(alpha,par->p);

	bn_t Mij;
	bn_new(Mij);

	g1_t Fij;
	g1_new(Fij);
	
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<d;j++)
		{
			fq2bn(Mij,fq_mat_entry(M,i,j),par->ctx);
			prfm(Fij,K,i,j);
			g1_mul(W[i][j],par->g,alpha);
			g1_mul(W[i][j],W[i][j],Mij);
			g1_add(W[i][j],W[i][j],Fij);
		}
	}
	
	gt_exp(PKm,par->gt,alpha);
	
	bn_free(alpha);
	bn_free(Mij);
	g1_free(Fij);
	
	return 0;
}

int mvm_pgen(gt_t *VKx, int n, int d, prfk *K, bn_t *x, mvm_par *par)
{
	g1_t ro;
	g1_new(ro);

	g1_t temp;
	g1_new(temp);

	g1_t A;
	g1_new(A);
	g1_t B;
	g1_new(B);
	g1_set_infty(A);
	g1_set_infty(B);

	g1_t ta;
	g1_t tb;
	g1_new(ta);
	g1_new(tb);



	for(int j=0;j<d;j++)
	{
		g1_mul(ta,K->A[j],x[j]);
		g1_mul(tb,K->B[j],x[j]);
		g1_add(A,A,ta);
		g1_add(B,B,tb);	
	}

	for(int i=0;i<n;i++)
	{
		g1_mul(ta,A,K->a[i]);
		g1_mul(tb,B,K->b[i]);
		g1_add(ro,ta,tb);
		pc_map(VKx[i],ro,par->h);
	}

	g1_free(A);
	g1_free(B);
	g1_free(ta);
	g1_free(tb);
	g1_free(ro);
	return 0;
}

int mvm_comp(fq_mat_t y, g1_t *V, g1_t **W, fq_mat_t M, bn_t *x, mvm_par *par)
{
	int n = fq_mat_nrows(M,par->ctx);
	int d = fq_mat_ncols(M,par->ctx);

	//compute y=Mx
	fq_mat_t xf;
	fq_mat_init(xf,d,1,par->ctx);

	fq_t temp;
	fq_init(temp,par->ctx);

	for(int j=0;j<d;j++)
	{
		bn2fq(temp,x[j],par->ctx);
		fq_mat_entry_set(xf,j,0,temp,par->ctx);
	}

	fq_mat_mul(y,M,xf,par->ctx);

	//compute V
	g1_t tg;
	g1_new(tg);

	for(int i=0;i<n;i++)
	{
		g1_new(V[i]);
		g1_set_infty(V[i]);

		for(int j=0;j<d;j++)
		{
			g1_mul(tg,W[i][j],x[j]);
			g1_add(V[i],V[i],tg);
		}
	}

	fq_mat_clear(xf,par->ctx);
	fq_clear(temp,par->ctx);
	g1_free(tg);
	return 0;
}

int mvm_vrfy(gt_t PKm, gt_t *VKx,fq_mat_t y, g1_t *V, mvm_par *par)
{
	int flag = 0;
	int n = fq_mat_nrows(y,par->ctx);
	gt_t left;
	gt_t right;
	gt_new(left);
	gt_new(right);
	bn_t yi;
	bn_new(yi);
	
	for(int i=0;i<n;i++)
	{
		pc_map(left,V[i],par->h);
		fq2bn(yi,fq_mat_entry(y,i,0),par->ctx);
		gt_exp(right,PKm,yi);
		gt_mul(right,right,VKx[i]);
		flag = gt_cmp(left,right);
		if(flag)
			break;
	}

	gt_free(left);
	gt_free(right);
	bn_free(yi);
	return flag;
}
