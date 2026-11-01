#include "chacha20.h"

static uint32_t rotl32(uint32_t x, int offset)
{
	return ((x << offset) | (x >> (32 - offset))) & 0xFFFFFFFF;
}

void chacha20_qround(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
	*a += *b; *d ^= *a; *d = rotl32(*d, 16);
	*c += *d; *b ^= *c; *b = rotl32(*b, 12);
	*a += *b; *d ^= *a; *d = rotl32(*d, 8);
	*c += *d; *b ^= *c; *b = rotl32(*b , 7);
}

#ifdef CHACHA_TEST
#include <stdio.h>
#include <unistd.h>

void    ft_putnbr_hex(unsigned int n)
{
	char    *base;

	base = "0123456789abcdef";
	if (n >= 16)
		ft_putnbr_hex(n / 16);
	write(1, &base[n % 16], 1);
}

void    ft_puthex(unsigned int n)
{
	write(1, "0x", 2);
	ft_putnbr_hex(n);
}

int main(void) {
	uint32_t a = 0x11111111;
	uint32_t b = 0x01020304;
	uint32_t c = 0x9b8d6f43;
	uint32_t d = 0x01234567;

	chacha20_qround(&a, &b, &c, &d);

	ft_puthex(a); printf("\n");
	ft_puthex(b); printf("\n");
	ft_puthex(c); printf("\n");
	ft_puthex(d); printf("\n");
}
#endif
