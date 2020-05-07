#include <stdio.h>
#include <stdint.h>
#include "function.h"

// Operate polynomial under GF(2^8).
// Choose irreducible polynomial x^8 + x^4 + x^3 + x + 1

const uint8_t generator = 0x03; // x + 1

uint8_t expTable[256];

uint8_t logTable[256];

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
	char *P1 = output(p1), *P2 = output(p2);
	char *addition = output(add(p1, p2));
	char *multiple = output(mulByTable(p1, p2));
	char *devision = output(devide(p1, p2, 0));
	char *modulo = output(mod(p1, p2));
	char *inverseP1 = output(inverse(p1)), *inverseP2 = output(inverse(p2));
	
	printf("p1: %s\np2: %s\n", P1, P2);
	printf("p1 add p2: %s\n", addition);
	if (p1 && p2) {
		printf("p1 mul p2: %s\n", multiple);
		printf("p1 devide p2: %s\n", devision);
		printf("p1 mod p2: %s\n", modulo);
	}
	printf("The inverse of p1 (mod x^8 + x^4 + x^3 + x + 1): %s\n", inverseP1);
	printf("The inverse of p2 (mod x^8 + x^4 + x^3 + x + 1): %s\n", inverseP2);

/*
// check the inverse function
	for (int i = 1; i <= 255; i++) {
		uint8_t p = i;
		uint8_t inverseP = inverse(p);
		if (mulByTable(p, inverseP) != 1)
			printf("%hhu has error inverse!\n", p);
	}
*/
	return 0;
}
