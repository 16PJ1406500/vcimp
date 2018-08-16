#include "pep.h"

/*! \struct pk_a
 * B is the polynomial defined as X^2+b0
 * R is the remainder polynomial
 */
typedef struct
{
  fq_poly_t B;
  fq_poly_t R;
} pk_a;

/*! \struct ek_a
 * d is the degree of polynomial
 * A is the evaluation polynomial
 */
typedef struct
{
  int d;
  fq_poly_t A;
  fq_poly_t Q;
} ek_a;

/*! \struct vk_x
 * B is the polynomial defined as X^2+b0
 * R is the remainder polynomial
 * pi is the proof
 */
typedef struct
{
  g1_t B;
  g2_t R;
  g2_t pi;
} vk_x;

/**
 * reduce the coefficients of a polynomial
 * @param[out] m         the out polynomial
 * @param[in] par        some parameter in flint lib
 */
int msg_modp(fq_poly_t m, pep *par);

/**
 * generate the verification key
 * @param[out] vck       the publick key
 * @param[out] eka       the evaluation key
 * @param[in] A          the evaluation polynomial
 * @param[in] par        some parameter in flint lib   
 */
int vc_keygen(pk_a *pka, ek_a *eka, fq_poly_t A, pep *par);

/**
 * generate the verification key
 * @param[out] vkx       the public verification key
 * @param[in] pka        the publick key
 * @param[in] x          the message x
 * @param[in] par        some parameter in flint lib   
 */
int vc_pgen(pk_a *pka, vk_x *vkx, bn_t x, pep *par);

/**
 * generate the verification key
 * @param[out] y         the output y
 * @param[out] vkx       the public verification key
 * @param[in] x          the message
 * @param[in] eka        the evaluation key
 * @param[in] par        some parameter in flint lib   
 */
int vc_comp(g2_t y, vk_x *vkx, bn_t x, ek_a *eka, pep *par);

/**
 * generate the verification key
 * @param[in] y          the output y
 * @param[in] vkx        the public verification key
 * @param[in] par        some parameter in flint lib   
 */
int vc_vrfy(g2_t y, vk_x *vkx, pep *par);
