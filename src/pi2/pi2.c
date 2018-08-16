#include "pi2.h"

// the vc_keygen algorithm: generating the evaluation key and public verification key of the scheme
int pi2_keygen(vc_k *vck, fq_mat_t F, lhe_par *par)
{
	vck->m=fq_mat_nrows(F,par->ctx);
	vck->d=fq_mat_ncols(F,par->ctx);

	fq_mat_init(vck->F,vck->m,vck->d,par->ctx);
	fq_mat_set(vck->F,F,par->ctx);

	vck->G=malloc(sizeof(g1_t)*vck->m);
	vck->T=malloc(sizeof(g1_t)*vck->d);

	fq_mat_t R;
	fq_mat_init(R,1,vck->m,par->ctx);
	bn_t zb;
	bn_new(zb);
	fmpz_t zf;
	fmpz_init(zf);
	fq_t zq;
	fq_init(zq,par->ctx);

	for (int i=0;i<vck->m;i++)
	{
		bn_rand_mod(zb,par->q);
		bn2fmpz(zf,zb);
   
		fq_set_fmpz(zq,zf,par->ctx);
		fq_mat_entry_set(R,0,i,zq,par->ctx);
		g1_new(vck->G[i]);
		g1_mul(vck->G[i],par->g,zb);      
	}
   
	fq_mat_t RF;
	fq_mat_init(RF,1,vck->d,par->ctx);
	fq_mat_mul(RF,R,F,par->ctx);
	    
	for (int j=0;j<vck->d;j++)
	{
		fq2bn(zb,fq_mat_entry(RF,0,j),par->ctx);
		g1_new(vck->T[j]);
		g1_mul(vck->T[j],par->g,zb);   
	} 

	fq_mat_clear(R,par->ctx);
	bn_free(zb);
	fmpz_clear(zf);
	fq_clear(zq,par->ctx);   
	return 0;
	}


// the vc_pgen algorithm
int pi2_pgen(vc_p *vcp, vc_k * vck, bn_t *x, lhe_par *par)
{
	// choose secret key for the lhe scheme
	lhe_keygen(vcp->s,par);

	// encrypting the message vector x
	fmpz_t zf;
	fmpz_init(zf);
	fq_t zq;
	fq_init(zq,par->ctx);

	fq_poly_t zqp;
	fq_poly_init(zqp,par->ctx);

	vcp->C=malloc(sizeof(lhe_c*)*vck->d);

	for (int j=0;j<vck->d;j++)
	{
		bn2fmpz(zf,x[j]);
		fq_set_fmpz(zq,zf,par->ctx);
		fq_poly_set_fq(zqp,zq,par->ctx);
		vcp->C[j]=malloc(sizeof(lhe_c));
		lhe_enc(vcp->C[j],vcp->s,zqp,par);
	}

	return 0;
}





// the vc_comp algorithm
int pi2_comp(lhe_c **nv, vc_k *vck, vc_p *vcp, lhe_par *par)
{
	fq_t *L=malloc(sizeof(fq_t)*vck->d);

	for (int j=0;j<vck->d;j++)
	{
		fq_init(L[j],par->ctx);
	}

	for (int i=0;i<vck->m;i++)
	{
		for (int j=0;j<vck->d;j++)
		{
			fq_set(L[j],fq_mat_entry(vck->F,i,j),par->ctx);  
		}
	nv[i]=malloc(sizeof(lhe_c));
	lhe_eval(nv[i],vcp->C,L,vck->d,par);     
	}    
	return 0;
}

// the vc_vrfy algorithm
int pi2_vrfy(fq_t *y, vc_k *vck, vc_p *vcp, lhe_c **nv, lhe_par *par)
{
	int flag=-1;
	fq_t zq;
	g1_t temp;
	g1_t right;
	g1_t left;
	bn_t temp1;
	bn_t temp2;
	for (int l=0;l<2*par->ni;l++)
	{
		g1_new(left);
		g1_set_infty(left);

		for (int i=0;i<vck->m;i++)
		{
			g1_new(temp);
			g1_set_infty(temp);
			fq_init(zq,par->ctx);
			if (l < par->ni)
				fq_poly_get_coeff(zq,nv[i]->p0,l,par->ctx);
			else 
				fq_poly_get_coeff(zq,nv[i]->p1,l-par->ni,par->ctx);
		bn_new(temp1);
		fq2bn(temp1,zq,par->ctx);
		g1_mul(temp,vck->G[i],temp1);
		g1_add(left,left,temp);
		}

		g1_new(right);
		g1_set_infty(right);

		for (int j=0;j<vck->d;j++)
		{
			g1_new(temp);
			g1_set_infty(temp);
			fq_init(zq,par->ctx);
			if (l < par->ni)
				fq_poly_get_coeff(zq,vcp->C[j]->p0,l,par->ctx);
			else
				fq_poly_get_coeff(zq,vcp->C[j]->p1,l-par->ni,par->ctx);
			bn_new(temp2);
			fq2bn(temp2,zq,par->ctx);
			g1_mul(temp,vck->T[j],temp2);
			g1_add(right,right,temp);
		}

		flag=g1_cmp(left,right);
		if (flag == 2) return 2;
	}
 
	if(flag==0)
	{
		for(int i=0;i<vck->m;i++)
		{
			fq_init(y[i],par->ctx);
			lhe_dec(y[i],vcp->s,nv[i],par);
		}
	}
 
  return 0;
}
