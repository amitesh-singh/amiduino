#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
   setup_default_uart();
   printf("hello world\n");

   return 0;
}
