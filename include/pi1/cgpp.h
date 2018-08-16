#ifndef _UTILS_H_
#define _UTILS_H_
#include "utils.h"
#endif

//define the parameter for pi1
typedef struct
{
	bn_t q;
	fmpz_t qf;
	fq_ctx_t ctx;
	g1_t g;

}cgpp;

//generate the parametr
int cgpp_new(cgpp *par);

//free the structure
int cgpp_free(cgpp *par);
