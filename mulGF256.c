#include <stdio.h>
#include <stdint.h>

// Operate polynomial under GF(2^8).
// Choose irreducible polynomial x^8 + x^4 + x^3 + x + 1

const uint8_t generator = 0x03; // x + 1

uint8_t expTable[256];

uint8_t logTable[256];

uint8_t mul(uint8_t a, uint8_t b) {
	uint8_t res = 0;

	for (; b; b >>= 1) {
		if (b & 1)
			res ^= a;
		if (a & 0x80)
			a = (a << 1) ^ 0x1b;
		else
			a <<= 1;
	}
	return res;
}

void createExpTable() {
	expTable[0] = 1;
	expTable[1] = generator;
	for (int i = 1; i <= 255; i++)
		expTable[i + 1] = mul(expTable[i], generator);
}

void createLogTable() {
	logTable[0] = 0;
	for (int i = 1; i <= 255; i++)
		logTable[expTable[i]] = i;
}

uint8_t mulByTable(uint8_t a, uint8_t b) {
	if (a & b)
		return expTable[(logTable[a] + logTable[b]) % 255];
	else
		return 0;
}

int main() {
	createExpTable();
	createLogTable();

//	for (int i = 0; i <= 255; i++)
//		printf("%hhu ", expTable[i]);
//	putchar('\n');
//	for (int i = 0; i <= 255; i++)
//		printf("%hhu ", logTable[i]);
//	putchar('\n');

	uint8_t p1, p2;
	scanf("%hhu%hhu", &p1, &p2);
	printf("multiple directly: %hhu\n", mul(p1, p2));
	printf("multiple by looking up the table: %hhu\n", mulByTable(p1, p2));

	return 0;
}
