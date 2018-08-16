#include "prf.h"

typedef struct
{
	fq_poly_t s;
	hash2_k *hk;
	bn_t a;	
	prfk **fk1;
	prfk **fk2;
}skm;

typedef struct
{
	lhe_c **g;
	gt_t *s;
}sigma_y;

int mf_keygen(g1_t **T, skm *skm, fq_mat_t M, lhe_par *par);

int mf_pgen(lhe_c **sigmax, prfk *tau, skm *skm, bn_t *x, int n, lhe_par *par);

int mf_comp(sigma_y *sy, fq_mat_t M, g1_t **T, hash2_k *hk, lhe_c **sigmax, lhe_par *par);

int mf_vrfy(fq_t *y, skm *skm, prfk *tau, sigma_y *sy, int n, lhe_par *par);

