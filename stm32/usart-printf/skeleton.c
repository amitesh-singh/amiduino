#include <stdio.h>
#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

int _write(int fd, char *ptr, int len);

int _write(int fd, char *ptr, int len)
{
  if (fd != 1)
  {
    return -1;
  }

  int i = 0;

  for (; i < len; ++i)
  {
    usart_send_blocking(USART1, ptr[i]);
  }

  return i;
}

static void uart_setup(void)
{
   rcc_periph_clock_enable(RCC_GPIOA);
   rcc_periph_clock_enable(RCC_USART1);

   // GPIO_USART1_TX - GPIO9
   gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
                 GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
   // Also: GPIO_USART1_RX - > GPIO10
   /* Setup UART parameters. */
   usart_set_baudrate(USART1, 9600);
   usart_set_databits(USART1, 8);
   usart_set_stopbits(USART1, USART_STOPBITS_1);
   usart_set_mode(USART1, USART_MODE_TX);
   usart_set_parity(USART1, USART_PARITY_NONE);
   usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

   /* Finally enable the USART. */
   usart_enable(USART1);
}

static void my_delay_1( void )
{
   int i = 72e6/2/4;

   while( i > 0 )
     {
        i--;
        __asm__( "nop" );
     }
}

int main( void )
{
   int x = 10;
   //set STM32 to 72 MHz
   rcc_clock_setup_in_hse_8mhz_out_72mhz();

   uart_setup();

   printf("setting it up\r\n");
   printf("Hello STM32 world!\r\n");
   my_delay_1();
   my_delay_1();
   printf("x value is: %d\r\n", x);
   while( 1 )
     {
     }
}
