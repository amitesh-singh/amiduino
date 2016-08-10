from wiki: https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus

Below is an example of bit-banging the SPI protocol as an SPI master with CPOL=0, CPHA=0, and eight bits per transfer. The example is written in the C programming language. Because this is CPOL=0 the clock must be pulled low before the chip select is activated. The chip select line must be activated, which normally means being toggled low, for the peripheral before the start of the transfer, and then deactivated afterwards. Most peripherals allow or require several transfers while the select line is low; this routine might be called several times before deselecting the chip.

/*
 * Simultaneously transmit and receive a byte on the SPI.
 *
 * Polarity and phase are assumed to be both 0, i.e.:
 *   - input data is captured on rising edge of SCLK.
 *   - output data is propagated on falling edge of SCLK.
 *
 * Returns the received byte.
 */
uint8_t SPI_transfer_byte(uint8_t byte_out)
{
    uint8_t byte_in = 0;
    uint8_t bit;

    for (bit = 0x80; bit; bit >>= 1) {
        /* Shift-out a bit to the MOSI line */
        write_MOSI((byte_out & bit) ? HIGH : LOW);

        /* Delay for at least the peer's setup time */
        delay(SPI_SCLK_LOW_TIME);

        /* Pull the clock line high */
        write_SCLK(HIGH);

        /* Shift-in a bit from the MISO line */
        if (read_MISO() == HIGH)
            byte_in |= bit;

        /* Delay for at least the peer's hold time */
        delay(SPI_SCLK_HIGH_TIME);

        /* Pull the clock line low */
        write_SCLK(LOW);
    }

    return byte_in;
}


Good explaination about SPI modes
CPOL -> CKL polarity
CPHA -> CKL phase.

http://dlnware.com/theory/SPI-Transfer-Modes

10:19:52 PM) _ami_: In case of HC595, the byte MISO  would be always what i just sent on MOSI?
(10:20:40 PM) _ami_: byte read at MISO*
(10:22:59 PM) Amkei [~Amkei@unaffiliated/amkei] entered the room.
(10:25:37 PM) _ami_: Also  i don't find in hc595's DS about CPOL & SPHA values. i know its CPOL=0 and CPHA=0 but where it is mentioned? 
(10:26:25 PM) alx741 left the room (quit: Ping timeout: 265 seconds).
(10:28:31 PM) LeoNerd: Since you're not reading from the 595, it half doesn't matter
(10:28:54 PM) LeoNerd: You can use 0/0 or 1/1; either way, you'll clock data out of the master on the rising edge of SCK, which is what the 595 wants
