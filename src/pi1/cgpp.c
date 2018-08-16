#include "cgpp.h"


int cgpp_new(cgpp *par){
	g1_get_ord(par->q);
	bn2fmpz(par->qf,par->q);
	//define the context, F_q
 	fq_ctx_init(par->ctx,par->qf,1,"ctx"); 
	g1_new(par->g);
	g1_get_gen(par->g);
	return 0;
}


int cgpp_free(cgpp *par){
	bn_clean(par->q);
	fmpz_clear(par->qf);
	fq_ctx_clear(par->ctx);
	g1_free(par->g);
	return 0;
}
