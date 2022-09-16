#include <stdio.h>


int main(void)
{

    unsigned long ptr = 12345678;
    printf("ptr => %lu, at %p, ", ptr, &ptr);
    printf("\n ptr[5] = %lu", &ptr[5]);


    return 0;
}
