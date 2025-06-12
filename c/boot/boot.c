

void _putc(char c) {
    *((volatile char *)(0xB80003f8U)) = c;
}

void print(char *s) {
    while (*s != '\0') {
        _putc(*s++);
    }
}

void __start() {
    print("Hello, world!\n");
    while (1) {
        print("\aPlease enter ...");
    }
}