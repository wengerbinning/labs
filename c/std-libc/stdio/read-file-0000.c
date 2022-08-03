#include <stdio.h>






int main(int argc, char argv[])  {
    FILE *file;
    char buffer[1024];

	fs_file_read(argv[1], buffer, sizeof(buffer));

    return 0;   
}
