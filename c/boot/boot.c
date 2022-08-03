void _start(void) {
    static const char msg[] = "Hello AArch64 from QEMU!\n";

    asm volatile(
        "mov x0, #1\n"          // fd = stdout
        "ldr x1, =msg\n"        // address of msg
        "mov x2, %0\n"          // length
        "mov x8, #64\n"         // syscall: write
        "svc #0\n"
        "mov x8, #93\n"         // syscall: exit
        "mov x0, #0\n"
        "svc #0\n"
        :
        : "r"(sizeof(msg) - 1)
        : "x0", "x1", "x2", "x8"
    );
}
