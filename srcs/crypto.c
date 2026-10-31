#include "Woody.h"

#include <elf.h>
#include <stdbool.h>
#include <stdio.h>

bool is_pt_load(Elf64_Phdr *phdr) {
	if (phdr != NULL && phdr->p_type == PT_LOAD) {
		printf("p_type: %d\n", phdr->p_type);
		return true;
	}
	return false;
}

/*
** get_next_pheader
** Description:
**		get the next pheader by adding jump_size ( pheader size )
**		
**
** Parameter :
** - phNum: total number of program header
** - phdr: actual pheader pointer
** - current_phdr_position: where we are in the different segments
** - jump_size: the size of a pheader
**
** Return:
** - NULL if at the end or next pheader
*/
Elf64_Phdr *get_next_pheader(unsigned int phNum, Elf64_Phdr *phdr, unsigned int current_phdr_position, int jump_size) {
	if (current_phdr_position >= phNum) {
		return NULL;
	}
	return (Elf64_Phdr *)((unsigned char *)phdr + jump_size);
}

unsigned char *chacha_encrypt(data_t *data) {
	int current_phdr_position;
	int phdr_total = data->header->e_phnum;
	Elf64_Phdr *current = data->pHeader;

	for (current_phdr_position = 0; current_phdr_position < data->header->e_phnum; current_phdr_position++) {
		if (is_pt_load(current)) {
			printf("It's an PT_LOAD\n");
		}
		current = get_next_pheader(phdr_total, current, current_phdr_position, data->header->e_phentsize);
	}
	return NULL;
}
