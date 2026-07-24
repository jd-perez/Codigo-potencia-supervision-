# Firmware de Supervisión de Semáforos (Arduino NANO)
SP32
## 1. Visión General del Sistema (High-Level Overview)
Este firmware opera como el núcleo de un sistema de diagnóstico y control para semáforos de corriente alterna. Su objetivo principal es comparar el estado lógico comandado (lo que el sistema *quiere* hacer) con la retroalimentación física de la red eléctrica (lo que *realmente* está sucediendo) para detectar focos fundidos, triacs en cortocircuito o triacs abiertos. 

El código está diseñado para integrarse estrictamente con la topología de hardware definida en **pcb_diagram_v2.jpg**, gestionando la comunicación no bloqueante, el filtrado de ruido eléctrico (debouncing) y la traducción de errores en bruto a diagnósticos legibles por humanos.


![Diagrama de Arquitectura de Hardware del Sistema](pcb_diagram_v2.jpg)

---

## 2. Componentes Estructurales (Hardware y Flujo de Datos)
El sistema se distribuye en placas independientes que manejan diferentes niveles de voltaje y responsabilidades. 

*   **Tarjeta de Control (Maestro):** Contiene un ESP32 que emite los estados lógicos a través de registros de desplazamiento 74HC595 (Salidas). 
*   **Tarjeta de Supervisión (Esclavo):** Ubicada en la zona de *Bajo Voltaje* en **pcb_diagram_v2.jpg**. Utiliza un segundo ESP32 para leer el estado físico del *Circuito "Foco fundido"* a través de registros 74HC165 (Entradas).
*   **Bus de Comunicación:** Ambas tarjetas se sincronizan a través de un bus I2C a 3.3V referenciado como el "Puerto PC".

---

## 3. Mecánica Granular: Clases y Funciones Clave

### A. `TrafficController` (El Cerebro del Sistema)
Esta clase orquesta todas las operaciones de entrada/salida y ejecuta la máquina de estados lógicos. Es responsable de enmascarar los retrasos del hardware sin bloquear el procesador.

*   **Máscara No Bloqueante (`TRANSITION_BLIND_SPOT`):** Cuando una luz cambia de color, los capacitores del hardware tardan en cargarse/descargarse. En lugar de usar `delay()` (lo que congelaría el bus I2C y colapsaría el ESP32), el sistema usa `millis()` para saltarse la lectura del hardware durante 600ms, permitiendo que las tareas de fondo continúen ejecutándose.
    ```cpp
    // Si no han pasado 600ms desde el cambio de luz, salimos de la función sin leer el hardware
    if (millis() - state_change_time < TRANSITION_BLIND_SPOT) {
        return; 
    }
    ```
*   **Ventana de Rebote (`ZERO_CROSS_DEBOUNCE_MS`):** Una vez superada la máscara de transición, si se detecta un error, el sistema espera 30ms (filtrando el ruido del cruce por cero de la corriente alterna) antes de confirmar y enviar una alerta definitiva.

### B. `ShiftRegIn` (Adquisición de Datos - 74HC165)
Esta clase captura el estado eléctrico en tiempo real de los semáforos. 
*   **Lógica de Captura:** Envía un pulso en bajo (`LOW`) de 5 microsegundos al pin `PIN_IN_PL` (LATCH) para "congelar" el estado físico actual de los pines externos en la memoria interna del chip.
*   **Desplazamiento de Bits:** Genera 32 pulsos de reloj para extraer secuencialmente los datos y empaquetarlos en una variable `uint32_t`.
    ```cpp
    physical_state <<= 1; 
    if (digitalRead(PIN_IN_DATA)) { 
        physical_state |= 1; 
    }
    ```

### C. `ShiftRegOut` (Control de Potencia - 74HC595)
Encargada de proyectar el patrón lógico ordenado hacia la "Tarjeta de Distribución".
*   **Optimización de Bus:** Incluye un condicional que verifica `if (pattern != last_written_state)`. Si el estado deseado es idéntico al actual, omite la escritura, ahorrando ciclos de reloj.
*   **Escritura Secuencial:** Divide la variable de 32 bits (`uint32_t`) en cuatro bytes de 8 bits (`uint8_t`) utilizando máscaras de bits (`& 0xFF`) y las envía en cadena usando `shiftOut()`.

### D. `ServerNotifier` (Traducción y Registro de Diagnósticos)
Traduce el índice binario de hardware crudo (del bit 2 al 31) en información utilizable para mantenimiento.
*   **Ingeniería Inversa de Índices:** Utiliza divisiones y módulos matemáticos para determinar exactamente qué fase (1 a 10) y qué color de luz falló, formateando la salida para la consola serial o servidor remoto.
    ```cpp
    int phase_group = 9 - ((software_bit_index - 2) / 3);
    int color_index = (software_bit_index - 2) % 3; 
    ```

---

## 4. Límites de Hardware y Advertencias de Seguridad 

Si estás ensamblando la topología mostrada en **pcb_diagram_v2.jpg**, debes respetar las siguientes reglas físicas para evitar daños permanentes en el microcontrolador:

1.  **Límites de Lógica 3.3V:** El ESP32 de la "Tarjeta de Supervisión" se encuentra estrictamente en la sección de *Bajo Voltaje*. Los registros 74HC165 **deben** estar alimentados a 3.3V. Inyectar señales de 5V provenientes de los optoacopladores directamente a los pines del ESP32 freirá los puertos GPIO instantáneamente.
2.  **Pines de Strapping (Arranque):** Al rutear la PCB, está terminantemente prohibido conectar el LATCH, CLOCK o DATA de los registros de desplazamiento a los puertos **GPIO 0, 2, 5, 12 o 15**. Estos son pines de strapping; si el 74HC165 los jala a un estado incorrecto al momento de encender la placa, el ESP32 no arrancará.
3.  **Resistencias Pull-up del Bus I2C:** El "Puerto PC" que une ambas tarjetas a través de la línea "Datos I2C (3.3V)" **requiere** resistencias pull-up físicas (ej. 3.3kΩ o 4.7kΩ) hacia la línea de 3.3V. Las resistencias internas del ESP32 son demasiado débiles para mantener la integridad de la señal a través de un cable externo interconectando dos placas distintas.