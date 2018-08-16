#include "pi1.h"
int pi1_keygen(vkf *vkf, fq_mat_t F, cgpp *par)
{
	vkf->m = fq_mat_nrows(F , par->ctx);
	vkf->d=fq_mat_ncols(F,par->ctx);
	fq_mat_init(vkf->F,vkf->m,vkf->d,par->ctx);
	fq_mat_set(vkf->F,F,par->ctx);

	vkf->G=malloc(sizeof(g1_t)*vkf->m);
	vkf->T=malloc(sizeof(g1_t)*vkf->d);

	fq_mat_t R;
	fq_mat_init(R,1,vkf->m,par->ctx);
	bn_t zb;
	bn_new(zb);
	fmpz_t zf;
	fmpz_init(zf);
	fq_t zq;
	fq_init(zq,par->ctx);


//	flint_rand_t state;
//	flint_randinit(state);

	for (int i=0;i<vkf->m;i++)
	{
		bn_rand_mod(zb,par->q);
		bn2fmpz(zf,zb);
//	   	fq_randtest(zq,sate,par->ctx);
		fq_set_fmpz(zq,zf,par->ctx);
		fq_mat_entry_set(R,0,i,zq,par->ctx);//ri=zq
		g1_new(vkf->G[i]);
		g1_mul(vkf->G[i],par->g,zb);  //    
	}
   



	fq_mat_t RF;
	fq_mat_init(RF,1,vkf->d,par->ctx);
	fq_mat_mul(RF,R,F,par->ctx);

//   printf("\n\n this is the random vector RF:\n\n");
//   fq_mat_print_pretty(RF,par->ctx);
   
    
	for (int j=0;j<vkf->d;j++)
	{
		bn_read_str(zb,fq_get_str_pretty(fq_mat_entry(RF,0,j),par->ctx),strlen(fq_get_str_pretty(fq_mat_entry(RF,0,j),par->ctx)),10);
		g1_new(vkf->T[j]);
		g1_mul(vkf->T[j],par->g,zb);   
   } 


	//release the memory
	fq_mat_clear(R,par->ctx);
	bn_free(zb);
	fmpz_clear(zf);
	fq_clear(zq,par->ctx);   
	return 0;
	
}

int pi1_comp(fq_mat_t Y, fq_mat_t F, bn_t *x, cgpp *par)
{
	int d;
	d = fq_mat_ncols(F,par->ctx);
	fq_mat_t X;
	fq_mat_init(X,d,1,par->ctx);
	fmpz_t zf;
	fmpz_init(zf);
	fq_t zq;
	fq_init(zq,par->ctx);
	
	for (int i=0;i<d;i++){
		bn2fmpz(zf,x[i]);
		fq_set_fmpz(zq,zf,par->ctx);
		fq_mat_entry_set(X,i,0,zq,par->ctx);	
	}
	fq_mat_mul(Y,F,X,par->ctx);
//	fq_mat_print_pretty(Y,par->ctx);

	//release the memory
	fq_mat_clear(X,par->ctx);
	fmpz_clear(zf);
	fq_clear(zq,par->ctx); 

	return 0;
}


int pi1_vrfy(vkf *vkf,fq_mat_t Y, bn_t *x, cgpp *par){
	int flag = -1;
	g1_t left;
	g1_new(left);
	g1_set_infty(left);

	g1_t gy;
	g1_new(gy);
	g1_set_infty(gy);

	g1_t right;
	g1_new(right);
	g1_set_infty(right);
	
	g1_t tx;
	g1_new(tx);
	g1_set_infty(tx);

//	bn_t zb;
//	bn_new(zb);

	bn_t yi;
	bn_new(yi);
	for(int i=0;i<vkf->m;i++){
		bn_read_str(yi,fq_get_str_pretty(fq_mat_entry(Y,i,0),par->ctx),strlen(fq_get_str_pretty(fq_mat_entry(Y,i,0),par->ctx)),10); 
		//bn_mul(zb,x[i],yi);
		g1_mul(gy,vkf->G[i],yi);
		g1_add(left,left,gy);
	}

	for(int j=0;j<vkf->d;j++){
		g1_mul(tx,vkf->T[j],x[j]);
		g1_add(right,right,tx);		
	}

	flag = g1_cmp(left,right);
	
//	printf("\nflag = %d\n",flag);
	return flag;
}

