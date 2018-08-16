#include "mvmp.h"

typedef struct
{
	g1_t *A;
	g1_t *B;
	bn_t *a;
	bn_t *b;
}prfk;


int prfm_keygen(prfk *K, int n, int d, mvm_par *par);

int prfm(g1_t fk, prfk *K, int i, int j);
