#include <stdio.h>
#include <string.h>
#include <stdint.h>

int fs_file_read (const char *file, char *buffer, size_t size) {
	FILE *fp;
	int ret;

	memset(buffer, 0, size);
	fp = fopen(file, "r");
	if (!fp) {
		return -1;
	}

	ret  = fread(buffer, (size - 1), 1, fp);

	fclose(fp);

	return ret;
}

int main(int argc, char *argv[]) {
	uint8_t idx;
	char buffer[1024];
	int ret;

	ret = fs_file_read(argv[1], buffer, sizeof(buffer));
	printf("read %d | %s", ret, buffer);

	return 0;
}
