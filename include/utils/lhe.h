#include "lhep.h"

//the ciphertext structure of the lhe scheme
typedef struct
{
  fq_poly_t p0;
  fq_poly_t p1;
} lhe_c;

//the key generation algorithm of the linearly homomorphic encryption: LHE.KeyGen
int lhe_keygen(fq_poly_t s, lhe_par *par);

//the encryption algorithm of the linearly homomorphic encryption: LHE.Enc
int lhe_enc(lhe_c *c, fq_poly_t s, fq_poly_t m,lhe_par *par);

//the decryption function of the linearly homomorphic encryption scheme: LEH.Dec
int lhe_dec(fq_t m, fq_poly_t s, lhe_c *c, lhe_par *par);

//the eval function of the linearly homomorphic encryption scheme: LHE.Eval
int lhe_eval(lhe_c *c, lhe_c **C, fq_t *L, int d, lhe_par *par);
