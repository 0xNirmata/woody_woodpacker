
#include "chacha20.h"

static uint32_t rotl32(uint32_t x, int offset)
{
	return ((x << offset) | (x >> (32 - offset)));
}

void chacha20_qround(uint32_t *a, uint32_t *b,
					uint32_t *c, uint32_t *d)
{
	*a += *b;
	*d ^= *a;
	*d = rotl32(*d, 16);

	*c += *d;
	*b ^= *c;
	*b = rotl32(*b, 12);

	*a += *b;
	*d ^= *a;
	*d = rotl32(*d, 8);

	*c += *d;
	*b ^= *c;
	*b = rotl32(*b, 7);
}

#ifdef CHACHA_TEST

#include <stdio.h>
#include <unistd.h>

#define QROUND(state, a, b, c, d) \
	chacha20_qround(&(state)->words[a], &(state)->words[b], \
					&(state)->words[c], &(state)->words[d])

uint32_t _a = 0xea2a92f4;
uint32_t _b = 0xcb1cf8ce;
uint32_t _c = 0x4581472e;
uint32_t _d = 0x5881c4bb;

static Chacha20 CHACHA_QROUND_INITIAL_TEST_VECTOR = {
	.words = {
		0x879531e0, 0xc5ecf37d, 0x516461b1, 0xc9a62f8a,
		0x44c20ef3, 0x3390af7f, 0xd9fc690b, 0x2a5f714c,
		0x53372767, 0xb00a5631, 0x974c541a, 0x359e9963,
		0x5c971061, 0x3d631689, 0x2098d9d6, 0x91dbd320
	}
};

static Chacha20 CHACHA_QROUND_EXPECTED_TEST_VECTOR = {
	.words = {
		0x879531e0, 0xc5ecf37d, 0xbdb886dc, 0xc9a62f8a,
		0x44c20ef3, 0x3390af7f, 0xd9fc690b, 0xcfacafd2,
		0xe46bea80, 0xb00a5631, 0x974c541a, 0x359e9963,
		0x5c971061, 0xccc07c79, 0x2098d9d6, 0x91dbd320
	}
};

static bool chacha20_compare(const Chacha20 *a, const Chacha20 *b)
{
	bool result = true;

	for (int i = 0; i < 16; i++)
	{
		if (a->words[i] != b->words[i])
		{
			printf("Difference at index %d: 0x%08x != 0x%08x\n",
				i, a->words[i], b->words[i]);
			result = false;
		}
	}

	return result;
}

void test_chacha20_qround(void)
{
	uint32_t a = 0x11111111;
	uint32_t b = 0x01020304;
	uint32_t c = 0x9b8d6f43;
	uint32_t d = 0x01234567;

	chacha20_qround(&a, &b, &c, &d);

	if (a == _a && b == _b && c == _c && d == _d)
		printf("quarter round passed\n");
	else
		printf("quarter round failed\n");
}

void test_chacha20_qround_on_state(void)
{
	QROUND(&CHACHA_QROUND_INITIAL_TEST_VECTOR, 2, 7, 8, 13);

	if (chacha20_compare(&CHACHA_QROUND_INITIAL_TEST_VECTOR,
						&CHACHA_QROUND_EXPECTED_TEST_VECTOR))
		printf("chacha20 state passed\n");
	else
		printf("chacha20 state failed\n");
}

int main(void)
{
	test_chacha20_qround();
	test_chacha20_qround_on_state();
	return 0;
}

#endif
