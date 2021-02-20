#include <common.h>

const unsigned long __stack_chk_guard = 0xDEADBEEF0C0FFEE0UL;
const char message[] = "stack-protector: U-Boot stack is corrupted\n";

void __stack_chk_fail(void)
{
	puts(message);
	panic("Resetting CPU ...\n");
}
