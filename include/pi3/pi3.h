#include "hash.h"
/*! \file pi3.h
    \brief File describing the pi3 scheme for verifying the computation

	The input x  of pi3 is not encrypted,and the function F is encrypted as F` by LHE.ENC.So the client pass (x,F`)
  to the server and the server computeF`*x by LHE.EVAL and return an out y to the client.Before passing the
  (x,F`) ,the client will generate a verification key to verify if LHE.DEC(y)=Fx after
  the computation.

*/

/*! \struct vc_k
 * m and d are the row and column of the F passing in and the vck->F going to generate.
 * F is the encrypted function
 * G and T is used to verify
 * s is the secret key
 * hk is the hash key
 */
typedef struct
{
	int m;
	int d;
	lhe_c ***F;
	g1_t *G;
	g2_t *T;
	fq_poly_t s;
	hash_k *hk;
}vc_k;


/*! \struct vc_p
 * c is the encrypted message. in pi3 c is x(no encryption)
 */
typedef struct
{
	bn_t *c;
} vc_p;

/**
 * generate the verification key and encrype the function
 * @param[in] F 	      the input function F
 * @param[in] par       some useful parameters
 * @param[out]vck       output the encryped function vck->F and the verification key: vck->G ,vck ->T lhe key K
 */
int pi3_keygen(vc_k *vck, fq_mat_t F, lhe_par *par);


/**
 * compute the function matrix multiply the message marix. nv=vck->F*vcp->x
 * @param[in] vck     the vck->F is needed here to compute
 * @param[in] vcp     the vcp->x is needed here to compute
 * @param[in] par     some useful parameters
 * @param[out]nv      the outcome of vck->F*vcp->x with size m*1
 */
int pi3_comp(lhe_c **nv, vc_k *vck, vc_p *vcp, lhe_par *par);

/**
 * verify the outcome. output 0 if the outcome is true, otherwise 2
 * @param[in] vck   the vck->G and the vck->T and the key K is needed here to verify
 * @param[in] vcp   the input vcp->x is needed here to verify
 * @param[in] nv    the output nv is needed here to verify
 * @param[in] par   some useful parameters
 */
int pi3_vrfy(fq_t *y, vc_k *vck, vc_p *vcp, lhe_c **nv, lhe_par *par);
