#include "mm42.h"

int mm_setup(g1_t **N, gt_t *PK, fq_mat_t M, mm_par *par)
{
	int n = fq_mat_nrows(M,par->ctx);
	int m = fq_mat_ncols(M,par->ctx);
	

	
//select a random matrix R
	bn_t R[n][m];
	for(int i=0;i<n;i++)
	{   
		for(int j=0;j<m;j++)
		{   
			bn_rand_mod(R[i][j],par->p);
		}
	}

	fq_t mij;
	fq_init(mij,par->ctx);

	bn_t tb;
	bn_new(tb);

	g1_t g1t;
	g1_new(g1t);

	g1_t gr;
	g1_new(gr);
//compute N

	for(int i=0;i<n;i++)
	{   
		for(int j=0;j<m;j++)
		{   
			
			fq_set(mij,fq_mat_entry(M,i,j),par->ctx);
			fq2bn(tb,mij,par->ctx);
			bn_mul(tb,tb,par->delta);//delta*mij
			bn_add(tb,tb,R[i][j]);
			bn_mod_basic(tb,tb,par->p);
			g1_mul(N[i][j],par->G[i],tb);
		}
	}
//compute PKm	

	for(int j=0;j<m;j++)
	{
		g1_set_infty(g1t);
		for(int i=0;i<n;i++)
		{   
			g1_mul(gr,par->G[i],R[i][j]);
			g1_add(g1t,g1t,gr);
		}
		pc_map(PK[j],g1t,par->h);
	}

	fq_clear(mij,par->ctx);
	bn_free(tb);
	g1_free(g1t);
	g1_free(gr);

	
	return 0;
}


int mm_pgen(gt_t VKx, bn_t *x, gt_t *PK, int m)
{
	gt_t temp;
	gt_new(temp);
	gt_set_unity(VKx);
	for(int j=0;j<m;j++)
	{
		gt_exp(temp,PK[j],x[j]);
		gt_mul(VKx,VKx,temp);
	}
	gt_free(temp);
	return 0;
}


int mm_comp(fq_mat_t Y, g1_t Pi, bn_t *x, fq_mat_t M, g1_t **N, mm_par *par)
{
	int n = fq_mat_nrows(M,par->ctx);
	int m = fq_mat_ncols(M,par->ctx);
	
//compute Y=Mx
	fq_mat_t xf;
	fq_mat_init(xf,m,1,par->ctx);

	fq_t temp;
	fq_init(temp,par->ctx);

	for(int j=0;j<m;j++)
	{
		bn2fq(temp,x[j],par->ctx);
		fq_mat_entry_set(xf,j,0,temp,par->ctx);
	}

	fq_mat_mul(Y,M,xf,par->ctx);

//compute pi
	g1_t pt;
	g1_new(pt);

	g1_set_infty(Pi);
	
	for(int i=0;i<n;i++)
	{		
		for(int j=0;j<m;j++)
		{
			g1_mul(pt,N[i][j],x[j]);
			g1_add(Pi,Pi,pt);
		}
	}

//clean
	fq_clear(temp,par->ctx);
	g1_free(pt);
	return 0;
}

int mm_vrfy(fq_mat_t Y, g1_t Pi, gt_t VKx, mm_par *par)
{
	int flag = 1;	
	gt_t left;
	gt_new(left);

	gt_t right;
	gt_new(right);

	bn_t yi;
	bn_new(yi);
	
	g1_t temp;
	g1_new(temp);

	g1_t gy;
	g1_new(gy);
	g1_set_infty(gy);
	
//compute left	
	pc_map(left,Pi,par->h);
	
//compute right
	for(int i=0;i<par->n;i++)
	{
		fq2bn(yi,fq_mat_entry(Y,i,0),par->ctx);
		g1_mul(temp,par->G[i],yi);
		g1_add(gy,gy,temp);
	}

	pc_map(right,gy,par->hd);
	gt_mul(right,right,VKx);

	flag = gt_cmp(left,right);

	gt_free(left);
	gt_free(right);
	bn_free(yi);
	g1_free(temp);
	g1_free(gy);
	
	return flag;
}
