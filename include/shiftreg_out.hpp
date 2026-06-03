#ifndef REGISTRODESOUT_HPP
#define REGISTRODESOUT_HPP

#include <Arduino.h>

// Definiciones de pines shift register (registor de desplazamiento) de SALIDA
#define PIN_DATAOUT  12  //Pin de datos de salida (DataOut)
#define PIN_LATCHOUT 33  //Pin de almacenamiento temporal de salida (Latch)
#define PIN_CLKOUT 15  //Pin de reloj de salida (ClockOut)
#define PIN_OEOUT   27  //Pin de activación de salidas (Output enable)

class ShiftRegOut{
    private:
        void led_write(uint8_t reg4, uint8_t reg3, uint8_t reg2, uint8_t reg1);

    public:
        //Actualmente no se va a usar el constructor de salidas
        ShiftRegOut() = default;

        void begin();
        void write_pattern(uint32_t pattern);

};



   
#endif