#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX 1000

#define SPEED_LIGHT 299792458 			/* Speed of light in vacuum m/s (exact) */
#define CHARGE 1.602176634e-19 			/* C elementary charge */
#define PLANK 6.62606896e-34  			/* Planck's constant j⋅s */
#define ELECTRON_MASS 9.1093837015e-31 	/* electron mass kg*/
#define PROTON_MASS 1.6726219236951e-27	/* proton mass kg */
#define E_0 8.854187817e-12    			/* Permittivity of free space (exact)  ^2/N⋅m^2 */
#define AVOGADRO 6.02214179e23			/* Avogadros's number particles/mol */
#define K_B 1.3806504e-23				/* Boltzmann's constant j/K */

#ifndef M_E
#define M_E 2.71828182845904523536028747135266249775L
#endif

enum functions {NONE, SIN, COS, TAN, ASIN, ACOS, ATAN, LOG, LN, SQRT, SINH, COSH, TANH, ASINH, ABS, MOD, CBRT, BIN_DEC, DEC_BIN, POW, MULT, DIV};

struct control{
	char *buf;
	unsigned int len;
};
