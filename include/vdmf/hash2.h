#include "lhe.h"

//the key structure of the hash 
typedef struct
{
	bn_t a;
	bn_t b;
	g1_t **K1;
	g2_t **K2;
}hash2_k;

typedef struct
{
	g1_t h1;
	g2_t h2;
}hv2;

// the hash key generation algorithm
int hash2_keygen(hash2_k *hk, lhe_par *par);

//compute mu(b,a)
int mucom(bn_t out, lhe_c *c, bn_t a, bn_t b, fq_ctx_t ctx);

//compute the hash digest
int hash2_H(hv2 *hv, lhe_c *c, hash2_k *hk, int mode, lhe_par *par);

