uint8_t strToDecimal(char a[9]); // string of binary to 1-byte decimal

char* output(uint8_t); // 1-byte decimal to polynomial form string

uint8_t add(uint8_t a, uint8_t b); // minus is equivalent to add

uint8_t mul(uint8_t a, uint8_t b, int mode); // mode0: normal, mode1: used for degree(a) + degree(b) = 8


int degree(uint8_t a);

uint8_t devide(uint8_t a, uint8_t b, int mode); // a devide b. mode0: normal, mode1: degree8 devide degree less than 8.

uint8_t mod(uint8_t a, uint8_t b);

uint8_t inverse(uint8_t a); // use extended euclidean to compute the inverse of a (mod x^8 + x^4 + x^3 + x + 1)


