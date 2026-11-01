#ifndef CHACHA20_H
#define CHACHA20_H

#include <stdint.h>

// vector or matrix chacha20
typedef struct {
	uint32_t words[16];
} Chacha20;

void chacha20_qround(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);

#endif
