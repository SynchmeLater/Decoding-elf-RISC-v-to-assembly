extern "C" __attribute__((section(".text.init"))) void _start() {
    // Initialize stack pointer to top of 128MB RAM
    asm volatile("li sp, 0x88000000");

    int arr1[5] = {1, 2, 3, 4, 5};
    int arr2[5] = {10, 20, 30, 40, 50};
    volatile int result[5];

    // Array addition
    for (int i = 0; i < 5; ++i) {
        result[i] = arr1[i] + arr2[i];
    }

    // Dummy read to prevent "unused variable" warning
    volatile int dummy = result[0];
    (void)dummy;

    // Infinite loop
    while (1) {
        asm volatile("nop");
    }
}