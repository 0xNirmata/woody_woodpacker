#include "Woody.h"

#include <elf.h>
#include <stdbool.h>
#include <stdio.h>

bool is_pt_load(Elf64_Phdr *phdr) {
	if (phdr->p_type == PT_LOAD) {
		return true;
	}
	return false;
}

/*
** get_next_pheader
** Description:
**		get the next pheader by adding 56 ( offset )
**
** Parameter :
** - phNum: total number of program header
** - phdr: actual pheader pointer
** - current_phdr_position: where we are in the different segments
**
** Return:
** - NULL if at the end or next pheader
*/
Elf64_Phdr *get_next_pheader(int phNum, Elf64_Phdr *phdr, int current_phdr_position) {
	if (current_phdr_position >= phNum) {
		return NULL;
	}
	return (phdr + 56);
}

unsigned char *chacha_encrypt(data_t *data) {
	int current_phdr_position;
	int phdr_total = data->header->e_phnum;
	Elf64_Phdr *current = data->pHeader;

	for (current_phdr_position = 0; current_phdr_position <= data->header->e_phnum; current_phdr_position++) {
		if (is_pt_load(current)) {
			printf("It's an PT_LOAD\n");
		}
		current = get_next_pheader(phdr_total, current, current_phdr_position);
	}
	return NULL;
}
