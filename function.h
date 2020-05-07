#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t strToDecimal(char a[9]); // string of binary to 1-byte decimal

char* output(uint8_t); // 1-byte decimal to polynomial form string

uint8_t add(uint8_t a, uint8_t b); // minus is equivalent to add

uint8_t mul(uint8_t a, uint8_t b, int mode); // mode0: normal, mode1: used for degree(a) + degree(b) = 8

const uint8_t monoPoly[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

int degree(uint8_t a);

uint8_t devide(uint8_t a, uint8_t b, int mode); // a devide b. mode0: normal, mode1: degree8 devide degree less than 8.

uint8_t mod(uint8_t a, uint8_t b);

uint8_t inverse(uint8_t a); // use extended euclidean to compute the inverse of a (mod x^8 + x^4 + x^3 + x + 1)


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

char* output(uint8_t a) {
	char* res = malloc(45);
	int index = 0;

	if (!a) { // 0
		res[0] = '0';
		res[1] = '\0';
		return res;
	}

	for (int i = 7; i >= 1; i--) {
		if (a & monoPoly[i]) {
			res[index] = 'x';
			res[index + 1] = '^';
			res[index + 2] = i + '0';
			res[index + 3] = ' ';
			res[index + 4] = '+';
			res[index + 5] = ' ';
			index += 6;
		}
	}
	if (a & monoPoly[0]) {
		res[index] = '1';
		index += 4;
	}
	res[index - 3] = '\0';
	return res;
}

uint8_t add(uint8_t a, uint8_t b) {
	return a ^ b;
}

uint8_t mul(uint8_t a, uint8_t b, int mode) {
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

int degree(uint8_t a) {
	for (int i = 7; i >= 0; i--) {
		if (a & monoPoly[i])
			return i;
	}
	return 0;
}

uint8_t devide(uint8_t a, uint8_t b, int mode) {
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
		return a ^ (mul(b, devide(a, b, 0), 0));
	else
		return 0;
}

uint8_t inverse(uint8_t a) {
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
	m[2] = m[0] ^ (mul(m[1], q[2], 0));

	while (poly[index] != 1) {
		index++;
		q[index] = devide(poly[index - 2], poly[index - 1], 0); // poly(i-2) devide poly(i-1)
		poly[index] = poly[index - 2] ^ (mul(poly[index - 1], q[index], 0)); // poly(i-2) mod poly(i-1)
		m[index] = m[index - 2] ^ (mul(m[index - 1], q[index], 0));
	}
	return m[index];
}
