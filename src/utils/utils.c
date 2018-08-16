#include "utils.h"

int sinit()
{
	if (core_init() != STS_OK)
	{
		core_clean();
		return 1;
	}
	if (pc_param_set_any() != STS_OK)
	{
		THROW(ERR_NO_CURVE);
		core_clean();
		return 1;
	}
	return 0;
}

int sclose()
{
	core_clean();

	return 0;
}


int bn2fmpz(fmpz_t out, bn_t in)
{
	int len = bn_size_str(in,10);
	char *str = malloc(sizeof(char)*len);

	bn_write_str(str,len,in,10);
	fmpz_set_str(out,str,10);
	free(str);
	return 0;
}

int bn2fq(fq_t out, bn_t in, fq_ctx_t ctx)
{
	fmpz_t tf;
	fmpz_init(tf);

	int len = bn_size_str(in,10);
	char *str = malloc(sizeof(char)*len);

	bn_write_str(str,len,in,10);
	fmpz_set_str(tf,str,10);
	fq_set_fmpz(out,tf,ctx);
	fmpz_clear(tf);
	free(str);
	return 0;
}


int fq2bn(bn_t out, fq_t in, fq_ctx_t ctx)
{	
	bn_read_str(out,fq_get_str_pretty(in,ctx),strlen(fq_get_str_pretty(in,ctx)),10);
	return 0;
}

int fq_mat_randz(fq_mat_t F, fq_ctx_t ctx, bn_t p)
{
	int m = fq_mat_nrows(F,ctx);
	int n = fq_mat_ncols(F,ctx);
	
	bn_t zb;
	bn_new(zb);
	fq_t zq;
	fq_init(zq,ctx);

	for (int i=0;i<m;i++)
	{
		for (int j=0;j<n;j++)
		{
			bn_rand_mod(zb,p);
			bn2fq(zq,zb,ctx);
			fq_mat_entry_set(F,i,j,zq,ctx);        
		}
	}

	bn_free(zb);
	fq_clear(zq,ctx);
   return 0;
}

int fq_poly_rand(fq_poly_t pol, int deg, fq_ctx_t ctx, bn_t bound)
{
	fq_poly_init(pol,ctx);
	bn_t z1;
	bn_new(z1);

	fq_t z2;
	fq_init(z2,ctx);

	for (int n=0;n<deg;n++)
	{
		bn_rand_mod(z1,bound);
		bn2fq(z2,z1,ctx);  
		fq_poly_set_coeff(pol,n,z2,ctx);
	}

	bn_free(z1);
	fq_clear(z2,ctx);
	return 0;
}

