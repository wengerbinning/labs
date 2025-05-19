// kernel.c
void write_char(char c, int pos) {
    char *video = (char*) 0xB8000;
    video[pos * 2] = c;
    video[pos * 2 + 1] = 0x07;  // 白字黑底
}

void main() {
    const char *msg = "Hello, Bare-Metal!";
    for (int i = 0; msg[i] != 0; i++) {
        write_char(msg[i], i);
    }

    while (1);  // 防止程序结束
}
