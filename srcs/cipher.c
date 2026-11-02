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
static unsigned char *get_next_pheader(Elf64_Phdr *phdr, program_position_t program) {
	if (program.current_phdr_position >= program.program_hdr_number) {
		return NULL;
	}
	return ((unsigned char *)phdr + program.jump_size);
}

static void fill_position(program_position_t *program, data_t *data) {
	program->jump_size = data->header->e_phentsize;
	program->program_hdr_number = data->header->e_phnum;
	program->current_phdr_position = 0;
}

unsigned char *encryption(data_t *data) {
	Elf64_Phdr *current = data->pHeader;
	program_position_t position = {0};

	fill_position(&position, data);

	for (; position.current_phdr_position < position.program_hdr_number; position.current_phdr_position++) {
		if (is_pt_load(current)) {
			printf("It's an PT_LOAD\n");
			// cipher chacha20
		}
		current = (Elf64_Phdr *)get_next_pheader(current, position);
	}
	return NULL;
}
