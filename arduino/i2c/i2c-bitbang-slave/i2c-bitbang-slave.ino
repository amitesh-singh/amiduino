
// INT0
#define sda PD2
// INT1
#define scl PD3

#define SLAVE_ADDR 0x3F

enum STATES {
     _IDLE,
     START,
     RECV_ADDR,
     STOP,
};

volatile STATES slaveState = _IDLE;
volatile uint8_t slaveAddr = 0;


void sda_low()
{
   DDRD |= (1 << sda);
   PORTD &= ~(1 << sda);
}

void sda_high()
{
   DDRD &= ~(1 << sda);
}

uint8_t sda_read()
{
   return bit_is_set(PIND, sda);
}

void scl_low()
{
   DDRD |= (1 << scl);
   PORTD &= ~(1 << scl);
}

void scl_high()
{
   DDRD &= ~(1 << scl);
}

uint8_t scl_read()
{
   return bit_is_set(PIND, scl);
}

volatile uint8_t i = 0;
ISR(INT1_vect)
{
   Serial.println("int1 vect");
   if (slaveState == RECV_ADDR)
     {
        //assuming clock is high
        if (i <= 7) // MSB - LSB
          {
             Serial.println("addr bit ..");
             slaveAddr <<= 1;
             if (sda_read())
               {
                  slaveAddr |= 1 ;
                  Serial.println("1");
               }
               else
               Serial.println("0");
          }
        else if (i == 8) //9th bit
          {
             Serial.println("9th bit ..");
             Serial.println(slaveAddr);
             if (slaveAddr == SLAVE_ADDR && bit_is_set(PIND, sda))
               {
                  Serial.println("sending ack");
                  //sda is pull high by master
                  sda_low();
                  scl_low();
               }
             else
               {
                Serial.println("entering into idle state now");
                  //do nothing?
                  slaveState = _IDLE;
                  //master will call stop
                  i = 0;
                  slaveAddr = 0;

                  EICRA |= (1 << ISC01); // falling edge logical change on PD2
                  EIMSK |= (1 << INT0); //enable interrupt on PD2

                  EICRA &= ~((1 << ISC11) | (1 << ISC10)) ; //rising edge
                  EIMSK &= ~(1 << INT1);
               }
          }
        ++i;
     }
}

//sda low - use this to detect START condition
ISR(INT0_vect)
{
   Serial.println("INT0_vec init");
   //called only when sda is low
   // check if scl is also low -- this is our START condition
   if (slaveState == _IDLE && !bit_is_set(PIND, scl))
     {
        slaveState = START;
        //disable the interrupt on SDA
        EIMSK &= ~(1 << INT0);
        EICRA &= ~(1 << ISC01);

        //enable the interrupt on SCL

        EICRA |= (1 << ISC11) | (1 << ISC10) ; //rising edge
        EIMSK |= (1 << INT1);
        i = 0;
        slaveState = RECV_ADDR;
        Serial.println("RECT ADDR state enter");
        return;
     }
}


void setup()
{
   Serial.begin(9600);

   delay(2000);
   cli();

   EICRA |= (1 << ISC01); // falling edge logical change on PD2
   EIMSK |= (1 << INT0); //enable interrupt on PD2
   i = 0;

   Serial.println("i2c slave is started - idle satte");
   sei();
}

void loop()
{
}
