#include <stdio.h>
#include <stdint.h>

// Operate polynomial under GF(2^8).
// Choose irreducible polynomial x^8 + x^4 + x^3 + x + 1

const uint8_t generator = 0x03; // x + 1

uint8_t expTable[256];

uint8_t logTable[256];

uint8_t strToDecimal(char a[9]) {
	uint8_t res = 0;
	int length = 0;

	while (a[length]) length++;

	res |= (a[0] - '0');
	for (int i = 1; i < length; i++) {
		res <<= 1;
		res |= (a[i] - '0');
	}
	return res;
}

uint8_t add(uint8_t a, uint8_t b) {
	return a ^ b;
}

uint8_t mul(uint8_t a, uint8_t b, int mode) { // mode1: normal, mode2: used for degree(a) + degree(b) = 8
	uint8_t res = 0;

	for (; b; b >>= 1) {
		if (b & 1)
			res ^= a;
		if (a & 0x80) {
			if (mode == 0) a = (a << 1) ^ 0x1b;
			else a <<= 1;
		}
		else
			a <<= 1;
	}
	return res;
}

void createExpTable() {
	expTable[0] = 1;
	expTable[1] = generator;
	for (int i = 1; i <= 255; i++)
		expTable[i + 1] = mul(expTable[i], generator, 0);
}

void createLogTable() {
	logTable[0] = 0;
	for (int i = 1; i <= 255; i++)
		logTable[expTable[i]] = i;
}

uint8_t mulByTable(uint8_t a, uint8_t b) {
	if (a | b)
		return expTable[((logTable[a] + logTable[b]) % 255)];
	else
		return 0;
}

const uint8_t monoPoly[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

int degree(uint8_t a) {
	for (int i = 7; i >= 0; i--) {
		if (a & monoPoly[i])
			return i;
	}
	return 0;
}

uint8_t devide(uint8_t a, uint8_t b, int mode) { // a devide b. mode0: normal, mode1: degree8 devide degree less than 8.
	if (!a || !b) return 0;
	if (b == 1) return a;

	uint8_t res = 0;
	int degreeA;
	if (mode == 0) degreeA = degree(a);
	else degreeA = 8;
	int degreeB = degree(b);

	while (degreeA >= degreeB || a > b) {
		int degreeQ = degreeA - degreeB;

		res |= (1 << degreeQ);
		a ^= (b << degreeQ);
		degreeA = degree(a);
	}
	return res;
}

uint8_t mod(uint8_t a, uint8_t b) {
	if (b)
		return a ^ (mulByTable(b, devide(a, b, 0)));
	else
		return 0;
}

uint8_t inverse(uint8_t a) { // use extended euclidean to compute the inverse of a (mod x^8 + x^4 + x^3 + x + 1)
	if (!a) return 0;
	if (a == 1) return 1;

	uint8_t poly[10];
	uint8_t q[10] = {0, 0};
	uint8_t m[10] = {0, 1};

	poly[0] = 0x1b; // it is x^8 + x^4 + x^3 + x + 1 cut off the leftest bit
	poly[1] = a;

	// init
	int index = 2;
	q[2] = devide(poly[0], poly[1], 1); // x^8 + x^4 + x^3 + x + 1 devide a
	poly[2] = poly[0] ^ (mul(poly[1], q[2], 1)); // x^8 + x^4 + x^3 + x + 1 mod a
	m[2] = m[0] ^ (mulByTable(m[1], q[2]));

	while (poly[index] != 1) {
		index++;
		q[index] = devide(poly[index - 2], poly[index - 1], 0); // poly(i-2) devide poly(i-1)
		poly[index] = poly[index - 2] ^ (mulByTable(poly[index - 1], q[index])); // poly(i-2) mod poly(i-1)
		m[index] = m[index - 2] ^ (mulByTable(m[index - 1], q[index]));
	}
	return m[index];
}
int main() {
	createExpTable();
	createLogTable();
/*
	for (int i = 0; i <= 255; i++)
		printf("%hhu ", expTable[i]);
	putchar('\n');
	for (int i = 0; i <= 255; i++)
		printf("%hhu ", logTable[i]);
	putchar('\n');
*/

	char input1[9], input2[9];
	scanf("%s%s", input1, input2);
	
	uint8_t p1 = strToDecimal(input1);
	uint8_t p2 = strToDecimal(input2);
	
	printf("p1: %hhu, p2: %hhu\n", p1, p2);
	printf("p1 add p2: %hhu\n", add(p1, p2));
	if (p1 && p2) {
		printf("p1 mul p2: %hhu\n", mulByTable(p1, p2));
		printf("p1 devide p2: %hhu\n", devide(p1, p2, 0));
		printf("p1 mod p2: %hhu\n", mod(p1, p2));
	}
	printf("The inverse of p1 (mod x^8 + x^4 + x^3 + x + 1): %hhu\n", inverse(p1));
	printf("The inverse of p2 (mod x^8 + x^4 + x^3 + x + 1): %hhu\n", inverse(p2));

/*
// check the inverse function
	for (int i = 1; i <= 255; i++) {
		printf("%d\n", i);
		uint8_t p = i;
		uint8_t inverseP = inverse(p);
		if (mulByTable(p, inverseP) != 1)
			printf("%hhu has error inverse!\n", p);
	}
*/
	return 0;
}
