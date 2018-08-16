#include "pi3.h"

//the vc_keygen algorithm: generating the evaluation key and public verification key of the scheme
int pi3_keygen(vc_k *vck, fq_mat_t F, lhe_par *par)
{

	//choose secret key for the lhe scheme
	lhe_keygen(vck->s,par);

	//encrypting the function F
	fq_t zq;
	fq_init(zq,par->ctx);
	fq_poly_t zqp;
	fq_poly_init(zqp,par->ctx);

	vck->F=malloc(sizeof(lhe_c*)*vck->m);
	for(int i=0;i<vck->m;i++)
	{
		vck->F[i]=malloc(sizeof(lhe_c)*vck->d);
	}

	for(int i=0;i<vck->m;i++)
	{
		for(int j=0;j<vck->d;j++)
		{
			fq_set(zq,fq_mat_entry(F,i,j),par->ctx);
			fq_poly_set_fq(zqp,zq,par->ctx);
			vck->F[i][j] = malloc(sizeof(lhe_c));
			lhe_enc(vck->F[i][j],vck->s,zqp,par);
		}
	}

  // generate the hash key
	hash_k *zhk;
	vck->hk=malloc(sizeof(*zhk));
	hash_keygen(vck->hk,par);
	g2_t **hvx = malloc(sizeof(g2_t*)*vck->m);
	for(int i=0;i<vck->m;i++)
	{
		hvx[i] = malloc(sizeof(g2_t)*vck->d);
	}
	for(int i=0;i<vck->m;i++)
	{
		for(int j=0;j<vck->d;j++)
		{
			hash_H(hvx[i][j],vck->F[i][j],vck->hk,0,par);
		}
	}


	vck->G = malloc(sizeof(g1_t)*vck->m);
	vck->T = malloc(sizeof(g2_t)*vck->d);

//choose random R
	bn_t *R = malloc(sizeof(bn_t)*vck->m);
	for(int i=0;i<vck->m;i++)
	{	
		bn_new(R[i]);
		bn_rand_mod(R[i],par->q);
	}
//compute G	
	for (int i=0;i<vck->m;i++)
	{
		g1_new(vck->G[i]);
		g1_mul(vck->G[i],par->g,R[i]);
		g1_norm(vck->G[i],vck->G[i]);
	}
//compute T			
	g2_t tj;
	g2_new(tj);
	for (int j=0;j<vck->d;j++)
	{
		g2_set_infty(vck->T[j]);
		for(int i=0;i<vck->m;i++)
		{
			g2_mul(tj,hvx[i][j],R[i]);
			g2_add(vck->T[j],vck->T[j],tj);
			g2_norm(vck->T[j],vck->T[j]);
		}
	}

	for(int i=0;i<vck->m;i++)
	{	
		bn_free(R[i]);
	}
	free(R);
	g2_free(tj);
	return 0;
}


//the vc_comp algorithm
int pi3_comp(lhe_c **nv, vc_k *vck, vc_p *vcp, lhe_par *par)
{
  //lhe_c *L=malloc(sizeof(lhe_c)*vck->d);
	fq_t *L = malloc(sizeof(fq_t)*vck->d);

	for (int j=0;j<vck->d;j++)
	{
		fq_init(L[j],par->ctx);
		bn2fq(L[j],vcp->c[j],par->ctx);
	}	


	for (int i=0;i<vck->m;i++)
	{
		lhe_eval(nv[i],vck->F[i],L,vck->d,par);
	}

	for (int j=0;j<vck->d;j++)
	{
		fq_clear(L[j],par->ctx);
	}
	free(L);
	return 0;
}

//the vc_vrfy algorithm
int pi3_vrfy(fq_t *y, vc_k *vck, vc_p *vcp, lhe_c **nv, lhe_par *par)
{
	int flag = -1;
	g2_t hv;
	g2_new(hv);

	gt_t zgt;
	gt_new(zgt);

	gt_t left;
	gt_new(left);
	gt_set_unity(left);

	gt_t right;
	gt_new(right);
	gt_set_unity(right);


	for (int i=0;i<vck->m;i++)
	{

		gt_set_unity(zgt);
		g2_set_infty(hv);
		hash_H(hv,nv[i],vck->hk,1,par);
		g2_norm(hv,hv);
		pc_map(zgt,vck->G[i],hv);
		gt_mul(left,left,zgt);
	}

	for (int i=0;i<vck->d;i++)
	{
		gt_set_unity(zgt);
		pc_map(zgt,par->g,vck->T[i]);
		gt_exp(zgt,zgt,vcp->c[i]);
		gt_mul(right,right,zgt);
	}

	flag=gt_cmp(left,right);

	if(flag==0)
	{
		for(int i=0;i<vck->m;i++)
		{
			lhe_dec(y[i],vck->s,nv[i],par);
		}
	}
	g2_free(hv);
	gt_free(zgt);
	gt_free(left);
	gt_free(right);
  return flag;
}
