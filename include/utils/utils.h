#include <fmpz.h>
#include <fmpz_poly.h>
#include <fq.h>
#include <fq_poly.h>
#include <relic/relic.h>
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

//Initializes relic library.
int sinit();

//Finalizes relic library.
int sclose();

//Converts a bn integer to a fmpz integer.
int bn2fmpz(fmpz_t out, bn_t in);

//Converts a bn integer to a fq integer.
int bn2fq(fq_t out, bn_t in, fq_ctx_t ctx);

////Converts a fq integer to a bn integer.
int fq2bn(bn_t out, fq_t in, fq_ctx_t ctx);

//generate a random matrix over fq
int fq_mat_randz(fq_mat_t F, fq_ctx_t ctx, bn_t p);

//generate a random polynomial that has small coefficients
int fq_poly_rand(fq_poly_t pol, int deg, fq_ctx_t ctx, bn_t bound);


