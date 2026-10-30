#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#include "Woody.h"

static int bopen(char *file_name) {
	int fd = open(file_name, O_RDONLY);
	if (fd < 0) {
		print_exit(1, "[ERROR] function open failed", 0, "open");
	}
	return fd;
}

static void fmap(data_t *data) {

	data->size = lseek(data->fd, (long)0, SEEK_END);
	if (data->size == -1) {
		print_exit(1, "[ERROR] failed to go through the file", data->fd, "lseek");
	}
	if (lseek(data->fd, 0, SEEK_SET) == -1) {
		print_exit(1, "[ERROR] failed to reset cursor", data->fd, "lseek");
	}

	data->map = mmap(NULL, data->size, PROT_READ | PROT_WRITE, MAP_PRIVATE, data->fd, 0);
	if (data->header == MAP_FAILED) {
		print_exit(1, "[ERROR] failed to map file", data->fd, "mmap");
	}

	data->header = (Elf64_Ehdr *)data->map;
}

static int is_ELF64(Elf64_Ehdr *header) {
	if (memcmp(header->e_ident, ELFMAG, SELFMAG) != 0) {
		return -1;
	}
	if (header->e_ident[EI_CLASS] != ELFCLASS64) {
		return -1;
	}
	if (header->e_machine == EM_386) {
		return -1;
	}
	return 0;
}

void validate_elf(char *file_name, data_t *data) {
	data->fd = bopen(file_name);
	fmap(data);

	if (is_ELF64(data->header) == -1) {
		munmap(data->map, data->size);
		print_exit(1, "[ERROR] the files isn't a ELF format file", data->fd, NULL);
	}


	close(data->fd);
}
