#include "utils.h"
typedef struct
{
	bn_t p;
	fmpz_t pf;
	fq_ctx_t ctx;
	g1_t g;
	g2_t h;
	gt_t gt;
}pep;

//generate the parameters
int pep_new(pep *par);

//free the structure
int pep_free(pep *par);
