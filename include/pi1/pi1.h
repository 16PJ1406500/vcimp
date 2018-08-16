#include "cgpp.h"

typedef struct
{
	int m;
	int d;
	fq_mat_t F;
	g1_t *G;
	g1_t *T;
}vkf;

int pi1_keygen(vkf *vkf, fq_mat_t F, cgpp *par);

int pi1_comp(fq_mat_t Y, fq_mat_t F, bn_t *x, cgpp *par);

int pi1_vrfy(vkf *vkf,fq_mat_t Y, bn_t *x, cgpp *par);
