#ifndef WOODY_H
#define WOODY_H

#include <elf.h>

// FLAGS
#define DEBUG_PRINT 0
#define NO_GUILTY ""

typedef struct data_s {
	Elf64_Ehdr *header;		// Only elf header part
	Elf64_Phdr *pHeader;
	void *map;				// the whole file mapped
	int size;					// whole file size
	unsigned char *encrypted;
	int fd;
} data_t;

#ifdef DEBUG
void print_elf_header(data_t *data);
void print_program_header(data_t *data);
# define WOODY_PRINT_ELF_HEADER(d) print_elf_header(d)
# define WOODY_PRINT_PROGRAM_HEADER(d) print_program_header(d)
#else
# define WOODY_PRINT_ELF_HEADER(d) ((void)0)
# define WOODY_PRINT_PROGRAM_HEADER(d) ((void)0)
#endif

// ELF
void validate_elf(char *file_name, data_t *data);

void modify_header(data_t *data);
void modify_segment(data_t *data);
void modify_section(data_t *data);

// CIPHER
// void compress_data(data_t *data); // If bonus
unsigned char *chacha_encrypt(data_t *data);

// UTILS
void cleanup(data_t *data);
void print_exit(int code, char *str, int fd, char *guilty);

#endif
