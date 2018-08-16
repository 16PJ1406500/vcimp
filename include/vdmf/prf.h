#include "hash2.h"

typedef struct 
{
	bn_t k1;
	bn_t k2;
}prfk;

int prfkg(prfk *fk, bn_t mod);

int prf(hv2 *fkx, prfk *fk2, prfk *fk1, lhe_par *par);
