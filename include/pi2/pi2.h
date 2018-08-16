#include "hash.h"

/*! \struct vc_k
 * m and d are the row and column of the F passing in and the vck->F going to generate.
 * F is the encrypted function
 * G and T is used to verify
 */
typedef struct
{
  int m;
  int d;
  fq_mat_t F;
  g1_t *G;
  g1_t *T;
} vc_k;


/*! \struct vc_p
 * C is the encrypted x
 * hk is the encrypted y
 */
typedef struct
{
  lhe_c **C;
  fq_poly_t s;
} vc_p;


/**
 * generate the verification key
 * @param[out] vck       output the encryped function vck->F and the verification key: vck->G ,vck ->T lhe key K
 * @param[in] F          the input function F
 * @param[in] par        some parameter in flint lib   
 */
int pi2_keygen(vc_k *vck, fq_mat_t F, lhe_par *par);


/**
 * generate the verification key
 * @param[out] vcp       output the encryped message vcp->C and the secret key
 * @param[in] vck        the publick key
 * @param[in] x          the message x
 * @param[in] par        some parameter in flint lib   
 */
int pi2_pgen(vc_p *vcp, vc_k * vck, bn_t *x, lhe_par *par);

/**
 * compute the encrypted output y
 * @param[out] nv		 the encrypted output y
 * @param[in] vck        the vck->F is needed here to compute
 * @param[in] vcp        the vcp->C is needed here to compute
 * @param[in] par        some parameter in flint lib   
 */
int pi2_comp(lhe_c **nv, vc_k *vck, vc_p *vcp, lhe_par *par);

/**
 * verify the outcome. output 0 if the outcome is true, otherwise 2
 * @param[out] y         the decoded output y
 * @param[in] vck   	 the vck->G and vck->T is needed here to verify
 * @param[in] vcp   	 the vcp->C is needed here to verify
 * @param[in] nv		 the encrypted output y
 * @param[in] par   	 some parameter in flint lib  
 */
int pi2_vrfy(fq_t *y, vc_k *vck, vc_p *vcp, lhe_c **nv, lhe_par *par);
