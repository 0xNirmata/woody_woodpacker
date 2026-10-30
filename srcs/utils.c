#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#include "Woody.h"

#ifdef DEBUG
static const char* get_type_str(uint16_t type) {
    switch (type) {
        case ET_NONE:   return "NONE (None)";
        case ET_REL:    return "REL (Relocatable file)";
        case ET_EXEC:   return "EXEC (Executable file)";
        case ET_DYN:    return "DYN (Position-Independent Executable / Shared Object)";
        case ET_CORE:   return "CORE (Core file)";
        default:        return "UNKNOWN";
    }
}

static const char* get_machine_str(uint16_t machine) {
    switch (machine) {
        case EM_X86_64: return "Advanced Micro Devices x86-64";
        case EM_386:    return "Intel 80386";
        case EM_ARM:    return "ARM";
        case EM_AARCH64:return "ARM AArch64";
        case EM_RISCV:  return "RISC-V";
        default:        return "UNKNOWN";
    }
}

static const char* get_ptype_str(uint32_t type) {
    switch (type) {
        case PT_NULL:         return "NULL";
        case PT_LOAD:         return "LOAD";
        case PT_DYNAMIC:      return "DYNAMIC";
        case PT_INTERP:       return "INTERP";
        case PT_NOTE:         return "NOTE";
        case PT_SHLIB:        return "SHLIB";
        case PT_PHDR:         return "PHDR";
        case PT_TLS:          return "TLS";
        case PT_GNU_EH_FRAME: return "GNU_EH_FRAME";
        case PT_GNU_STACK:    return "GNU_STACK";
        case PT_GNU_RELRO:    return "GNU_RELRO";
        default:              return "UNKNOWN";
    }
}

static void get_pflags_str(uint32_t flags, char *buf) {
    buf[0] = (flags & PF_R) ? 'R' : ' ';
    buf[1] = (flags & PF_W) ? 'W' : ' ';
    buf[2] = (flags & PF_X) ? 'E' : ' ';
    buf[3] = '\0';
}

void print_elf_header(data_t *data) {
	Elf64_Ehdr *hdr = data->header;

	printf("=========ELF FILE HEADER=========\n");

	// magic bytes
	printf("Magic: ");
	for (int i = 0; i < EI_NIDENT; i++) {
		printf("%02x ", hdr->e_ident[i]);
	}
	printf("\n");

	// class (32 bit vs 64 bit)
	printf("Class:                   %s\n",
			hdr->e_ident[EI_CLASS] == ELFCLASS64 ? "ELF64" :
			hdr->e_ident[EI_CLASS] == ELFCLASS32 ? "ELF32" : "Invalid");

	// data encoding (endianness)
	printf("Data Encoding:          %s\n",
			hdr->e_ident[EI_DATA] == ELFDATA2LSB ? "2's complement, little endian" :
			hdr->e_ident[EI_DATA] == ELFDATA2MSB ? "2's complement, big endian" : "Invalid");

	// file  metadata & offsets
	printf("Type:                       %s\n", get_type_str(hdr->e_type));
	printf("Machine Architecture:       %s\n", get_machine_str(hdr->e_machine));
	printf("Entry Point Address:        0x%016lx\n", hdr->e_entry);
	printf("Start of Program Headers:   %lu (bytes into file)\n", hdr->e_phoff);
	printf("Start of Section Headers:   %lu (bytes into file)\n", hdr->e_shoff);
	printf("Flags:                      0x%x\n", hdr->e_flags);
	printf("Header Size:                %d bytes\n", hdr->e_ehsize);
	printf("Program Header Count:       %d (size: %d bytes)\n", hdr->e_phnum, hdr->e_phentsize);
	printf("Section Header Count:       %d (size: %d bytes)\n", hdr->e_shnum, hdr->e_shentsize);
	printf("Section Header String Index:%d\n", hdr->e_shstrndx);
	printf("\n");
}

void print_program_header(data_t *data) {
	Elf64_Ehdr *hdr = data->header;
	Elf64_Phdr *table = (Elf64_Phdr *)((char *)data->header + hdr->e_phoff);
	char flags[4];

	printf("=========PROGRAM HEADERS=========\n");
	for (int i = 0; i < hdr->e_phnum; i++) {
		Elf64_Phdr *ph = &table[i];

		get_pflags_str(ph->p_flags, flags);

		printf("[%d] %-12s  Flags: %s\n", i, get_ptype_str(ph->p_type), flags);
		printf("    Offset:         0x%016lx\n", ph->p_offset);
		printf("    Virtual Addr:   0x%016lx\n", ph->p_vaddr);
		printf("    Physical Addr:  0x%016lx\n", ph->p_paddr);
		printf("    File Size:      %lu bytes\n", ph->p_filesz);
		printf("    Memory Size:    %lu bytes\n", ph->p_memsz);
		printf("    Align:          0x%lx\n", ph->p_align);
		printf("\n");
	}
}

# define WOODY_PRINT_ELF_HEADER(d) print_elf_header(d)
# define WOODY_PRINT_PROGRAM_HEADER(d) print_program_header(d)
#else
# define WOODY_PRINT_ELF_HEADER(d) ((void)0)
# define WOODY_PRINT_PROGRAM_HEADER(d) ((void)0)
#endif

void cleanup(data_t *data) {
	munmap(data->map, data->size);
	close(data->fd);
}
