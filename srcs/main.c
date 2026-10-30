#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Woody.h"

void print_exit(int code, char *str, int fd, char *guilty)
{
	printf("%s\n", str);
	if (guilty)
		perror(guilty);
	if (fd != 0)
		close(fd);
	exit (code);
}

int main(int ac, char *av[]) {
	(void)av;

	data_t data = {0};

	if (ac != 2)
		print_exit(1, "[ERROR] One arguments required", 0, NULL);

	validate_elf(av[1], &data);

	data.pHeader = (data.map + 64);

	WOODY_PRINT_ELF_HEADER(&data);
	WOODY_PRINT_PROGRAM_HEADER(&data);

	printf("%ld\n", sizeof(Elf64_Phdr));

	cleanup(&data);
	return 0;
}
