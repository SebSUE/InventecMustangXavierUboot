/*
 * Copyright 2015 Broadcom Corporation.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

void usage(const char *prog)
{
	fprintf(stderr, "Usage: %s <bmp file or gzipped bmp file>\n", prog);
}

int main(int argc, char *argv[])
{
	FILE *fp;
	struct stat bmp_stat;
	int i, j, size, n;
	unsigned char *p;

	if (argc < 2) {
		usage(argv[0]);
		exit(-1);
	}

	stat(argv[1], &bmp_stat);
	size = bmp_stat.st_size;

	fp = fopen(argv[1], "rb");
	if (!fp) {
		perror(argv[1]);
		exit(-1);
	}

	p = malloc(size);
	if (!p) {
		fprintf(stderr, "Out of memory, need %d bytes!", size);
		exit(-1);
	}

	n = fread(p, 1, size, fp);
	fclose(fp);

	if (n != size) {
		fprintf(stderr,
			"Read error, %d bytes read, %d bytes expected!",
			n, size);
		exit(-1);
	}

	printf("/*\n"
	       " * Automatically generated by \"tools/bmp_raw\"\n"
	       " *\n"
	       " * DO NOT EDIT\n"
	       " *\n"
	       " */\n\n\n"
	       "#ifndef __BMP_RAW_H__\n"
	       "#define __BMP_RAW_H__\n\n");

	printf("unsigned char bmp_data[] = {\n");
	printf("\t");
	j = 8;
	for (i = 0; i < size; i++) {
		printf(" 0x%02X", p[i]);
		j += 6;
		if (i != size - 1) {
			printf(",");
			if (j >= 70) {
				printf("\n\t");
				j = 8;
			}
		}
	}
	printf("\n"
		"};\n\n"
		"#endif /* __BMP_RAW_H__ */\n"
		);

	return 0;
}