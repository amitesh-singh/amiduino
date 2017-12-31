#include <util/delay.h>
#include "i2csoft.h"
#include "uart.h"
#include <stdlib.h>


#define INA219_ADDRESS                          (0x40)    // Default address (A0&A1 gnd)

#define INA219_REG_CONFIG                       (0x00)
#define INA219_CONFIG_RESET                     (0x8000)  // Reset Bit

#define INA219_CONFIG_BVOLTAGERANGE_MASK        (0x2000)  // Bus Voltage Range Mask
#define INA219_CONFIG_BVOLTAGERANGE_16V         (0x0000)  // 0-16V Range
#define INA219_CONFIG_BVOLTAGERANGE_32V         (0x2000)  // 0-32V Range

#define INA219_CONFIG_GAIN_MASK                 (0x1800)  // Gain Mask
#define INA219_CONFIG_GAIN_1_40MV               (0x0000)  // Gain 1, 40mV Range
#define INA219_CONFIG_GAIN_2_80MV               (0x0800)  // Gain 2, 80mV Range
#define INA219_CONFIG_GAIN_4_160MV              (0x1000)  // Gain 4, 160mV Range
#define INA219_CONFIG_GAIN_8_320MV              (0x1800)  // Gain 8, 320mV Range

#define INA219_CONFIG_BADCRES_MASK              (0x0780)  // Bus ADC Resolution Mask
#define INA219_CONFIG_BADCRES_9BIT              (0x0080)  // 9-bit bus res = 0..511
#define INA219_CONFIG_BADCRES_10BIT             (0x0100)  // 10-bit bus res = 0..1023
#define INA219_CONFIG_BADCRES_11BIT             (0x0200)  // 11-bit bus res = 0..2047
#define INA219_CONFIG_BADCRES_12BIT             (0x0400)  // 12-bit bus res = 0..4097

#define INA219_CONFIG_SADCRES_MASK              (0x0078)  // Shunt ADC Resolution and Averaging Mask
#define INA219_CONFIG_SADCRES_9BIT_1S_84US      (0x0000)  // 1 x 9-bit shunt sample
#define INA219_CONFIG_SADCRES_10BIT_1S_148US    (0x0008)  // 1 x 10-bit shunt sample
#define INA219_CONFIG_SADCRES_11BIT_1S_276US    (0x0010)  // 1 x 11-bit shunt sample
#define INA219_CONFIG_SADCRES_12BIT_1S_532US    (0x0018)  // 1 x 12-bit shunt sample
#define INA219_CONFIG_SADCRES_12BIT_2S_1060US   (0x0048)  // 2 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_4S_2130US   (0x0050)  // 4 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_8S_4260US   (0x0058)  // 8 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_16S_8510US  (0x0060)  // 16 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_32S_17MS    (0x0068)  // 32 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_64S_34MS    (0x0070)  // 64 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_128S_69MS   (0x0078)  // 128 x 12-bit shunt samples averaged together

#define INA219_CONFIG_MODE_MASK                 (0x0007)  // Operating Mode Mask
#define INA219_CONFIG_MODE_POWERDOWN            (0x0000)
#define INA219_CONFIG_MODE_SVOLT_TRIGGERED      (0x0001)
#define INA219_CONFIG_MODE_BVOLT_TRIGGERED      (0x0002)
#define INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED  (0x0003)
#define INA219_CONFIG_MODE_ADCOFF               (0x0004)
#define INA219_CONFIG_MODE_SVOLT_CONTINUOUS     (0x0005)
#define INA219_CONFIG_MODE_BVOLT_CONTINUOUS     (0x0006)
#define INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS (0x0007)

#define INA219_REG_SHUNTVOLTAGE                 (0x01)  // SHUNT VOLTAGE REGISTER (R)
#define INA219_REG_BUSVOLTAGE                   (0x02)  // BUS VOLTAGE REGISTER (R)
#define INA219_REG_POWER                        (0x03)  // POWER REGISTER (R)
#define INA219_REG_CURRENT                      (0x04)  // CURRENT REGISTER (R)
#define INA219_REG_CALIBRATION (0x05) // CALIBRATION REGISTER (R/W)

/*

   ina219_begin(INA219_ADDRESS);
   ina219_calibrate(Mode_16V_400mA);
   lastVoltage = ina219_getBusVoltage();
   lastCurrent = ina219_getCurrent_mA();

#define INA219_ADDRESS (0x40) 
typedef enum INA219_Calibration {
Mode_32V_2A,
Mode_32V_1A,
Mode_16V_400mA,
} INA219_Calibration_t;

float ina219_getBusVoltage() {
uint16_t value = readRegister(INA219_REG_BUSVOLTAGE);
int16_t milliVolt = (int16_t)((value >> 3) * 4);
return milliVolt / 1000.0;
}
case Mode_16V_400mA:
ina219_calValue = 8192;
ina219_currentDivider_mA = 20;
ina219_powerDivider_mW = 1;
writeRegister(INA219_REG_CALIBRATION, ina219_calValue);
config = INA219_CONFIG_BVOLTAGERANGE_16V |
INA219_CONFIG_GAIN_1_40MV |
INA219_CONFIG_BADCRES_12BIT |
INA219_CONFIG_SADCRES_12BIT_1S_532US |
INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
writeRegister(INA219_REG_CONFIG, config);
break;

#define INA219_REG_SHUNTVOLTAGE                 (0x01)  // SHUNT VOLTAGE REGISTER (R)
#define INA219_REG_BUSVOLTAGE                   (0x02)  // BUS VOLTAGE REGISTER (R)
#define INA219_REG_POWER                        (0x03)  // POWER REGISTER (R)
#define INA219_REG_CURRENT                      (0x04)  // CURRENT REGISTER (R)
#define INA219_REG_CALIBRATION (0x05) // CALIBRATION REGISTER (R/W)
#define INA219_REG_CONFIG                       (0x00)
#define INA219_CONFIG_RESET (0x8000) // Reset Bit
 */

//this is tested on i2c module to calculate current
// i2c address is 64 (0x40 in hex)
// 
void i2c_scan()
{
   i2c_begin();
   uint8_t ack = 1;
   ack = i2c_start((64 << 1) + 1);
   i2c_stop();

   if (ack == 0)
     {
        uart_puts("got it.\r\n");
     }
}

uint16_t read_ina219(uint8_t reg)
{
   uint16_t value = 0;

   i2c_begin();

   i2c_start((64 << 1) + 1); //master wants to write
   i2c_writeByte(reg); //master writes 1 reg byte to slave
   i2c_stop();

   _delay_ms(1);

   i2c_begin();
   i2c_start((64 << 1) + 0); //master wants to read
   value |= i2c_readByte(0) << 8;
   value |= i2c_readByte(1);
   i2c_stop();

   return value;
}

void write_ina219(uint8_t reg, uint16_t val)
{
  i2c_begin();
  i2c_writeByte((64 << 1) + 1);
  i2c_writeByte(reg);
  i2c_writeByte((val >> 8) & 0xFF);
  i2c_writeByte(val & 0xFF);
  i2c_stop();
}

static void
_newline()
{
  uart_puts("\r\n");
}

int main(void)
{
   // uart_puts("Looking for i2c device INA219.\r\n");
   //wait for 200ms 
   _delay_ms(200);
   i2c_scan();

   //The below two {}s the module into Mode_16V_400mA mode.

     {
        uint16_t ina219_calValue = 8192;
      
      //  i2c_begin();
      //  i2c_start((64 << 1) + 1);
      //  i2c_writeByte(INA219_REG_CALIBRATION);

      //  i2c_writeByte(ina219_calValue >> 8 || 0xFF);
      //  i2c_writeByte(ina219_calValue & 0xFF);
      //  i2c_stop();
        write_ina219(INA219_REG_CALIBRATION, ina219_calValue);
     }
   _delay_ms(10);


     {
        //i2c_begin();
        //i2c_start((64 << 1) + 1);
        //i2c_writeByte(INA219_REG_CONFIG);
      
        uint16_t config = INA219_CONFIG_BVOLTAGERANGE_16V |
           INA219_CONFIG_GAIN_1_40MV |
           INA219_CONFIG_BADCRES_12BIT |
           INA219_CONFIG_SADCRES_12BIT_1S_532US |
           INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
        write_ina219(INA219_REG_CONFIG, config);
        
      //  i2c_writeByte(config >> 8 || 0xFF);
      //  i2c_writeByte(config & 0xFF);
      
     }

   _delay_ms(10);
   //read the Bus Voltage
     {
        uint16_t value = read_ina219(INA219_REG_BUSVOLTAGE);
        int16_t milliVolt = (int16_t)((value >> 3) * 4);
        char str[5];
        itoa(milliVolt, str, 10);
        uart_puts(str);
     }
    
     _newline();
     _delay_ms(10);
    //ShuntVoltage
    {
      uint16_t value;
      value = read_ina219(INA219_REG_SHUNTVOLTAGE);
      value -= 18;    // Biasing
      value = (value = 18)/100.0;
      char str[5];
      itoa(value, str, 10);
      uart_puts(str);
    }
    _newline();

    _delay_ms(10);

///read the current in mA
    
      uint16_t value;
      uint16_t ina219_calValue = 8192;
      uint8_t ina219_currentDivider_mA = 20;
     // uint8_t ina219_powerDivider_mW = 1;

      write_ina219(INA219_REG_CALIBRATION, ina219_calValue);

    // Now we can safely read the CURRENT register!
       value = read_ina219(INA219_REG_CURRENT);
  
              
       
     
    
  uint16_t valueDec = (float)value / ina219_currentDivider_mA - 1.8;

  //this overflows the text section
  //char str[5];
  //itoa(valueDec, str, 10);
  //uart_puts(str);

   while (1)
     {
     }

   return 0;
}
