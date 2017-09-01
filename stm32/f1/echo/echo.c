#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

static void uart_setup(void)
{
   rcc_periph_clock_enable(RCC_GPIOA);
   rcc_periph_clock_enable(RCC_USART1);

   // GPIO_USART1_TX - GPIO9
   gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                 GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
   // GPIO10
   gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                 GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);
   // Also: GPIO_USART1_RX - > GPIO10
   /* Setup UART parameters. */
   usart_set_baudrate(USART1, 9600);
   usart_set_databits(USART1, 8);
   usart_set_stopbits(USART1, USART_STOPBITS_1);
   usart_set_mode(USART1, USART_MODE_TX_RX);
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

static void uart_print(uint32_t usart, char *s)
{
   //
   // while (!usart_is_send_ready(usart));
   // usart_send(usart, data);

   while (*s != 0)
     {
        usart_send_blocking(usart, *s);
        s++;
     }

   usart_send_blocking(usart, '\r');
   usart_send_blocking(usart, '\n');
}

int main( void )
{
   uint8_t recv;
   //set STM32 to 72 MHz
   rcc_clock_setup_in_hse_8mhz_out_72mhz();

   uart_setup();

   uart_print(USART1, "setting it up");

   my_delay_1();
   my_delay_1();

   while( 1 )
     {
        if ((recv = usart_recv_blocking(USART1)))
          {
             uart_print(USART1, "");
             usart_send_blocking(USART1, recv);
             uart_print(USART1, "");
          }
        //usart_send_blocking(USART1, recv);
        //my_delay_1();
     }
}
