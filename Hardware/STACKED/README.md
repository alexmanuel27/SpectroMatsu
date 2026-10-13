# SpectroMatsu: revisión del esquemático para montaje apilado

Diseño: **Ing. Alex M. Rivera**. Revisión de trabajo: **12 de octubre de 2026**.

**Es posible usar el XIAO SAMD21 que ya tienes.** Esta propuesta conserva el acondicionamiento del Hamamatsu y la alimentación de bajo ruido. La energía procede exclusivamente del teléfono. El original de `Hardware/BOARD/` permanece intacto.

## Decisiones de esta versión

Confirmadas por Alex el 12 de octubre de 2026:

- Un único LED blanco Nichia NF2W757G-F1 Optisolis; sin emisores UV adicionales.
- ADC **externo** para el espectro. El ADC original es ADS1115BQDGSRQ1. Su reutilización experimental o sustitución por un SAR rápido está pendiente de elección; el camino A0 del esquema anterior queda como referencia de banco, no como adquisición elegida.
- Conservar las referencias de fabricante del proyecto original como inventario de partida. Los circuitos integrados reutilizados llevan ahora el campo `MP` correspondiente. Esto no convierte los valores de la columna `Value` en valores verificados: por ejemplo, LQW18AS4N7G00D es 4,7 nH y no puede ocupar el lugar del inductor de potencia de 4,7 µH.
- Partir del contorno de las PCB del turbidímetro; la carcasa se modificará después según el montaje resultante.
- Resolver aquí la alimentación. La adaptación de la app queda para cuando esté listo el equipo.

`sensor.kicad_pcb` y `controller.kicad_pcb` son **bases mecánicas sin componentes ni pistas**. Copian los 12 elementos de Edge.Cuts de cada placa correspondiente del turbidímetro, incluida la abertura de la cubeta y sus alivios. La base conserva su muesca lateral de cable; la superior conserva el borde de su versión. El círculo de 44 mm es una referencia de espacio de carcasa en Dwgs.User, no el diámetro de la PCB. Ambas conservan el espesor inicial de 1,6 mm. La cabida del Hamamatsu, conectores y óptica aún no se ha comprobado.

Fuentes locales del contorno: `Turbimeter_Citizen_science/hardware/esque.kicad_pcb` (SHA-256 `8bd9a92d6d309960262c510c466a97a1d8dc27ad9ddf35589ad6c859297c53d9`) y `hardware/controller/controller.kicad_pcb` (SHA-256 `6be95625d6478e1cf778109f03a96f28e3bd224011977e391eb54ef60566907f`). No se han modificado esas fuentes.

Abre **spectromatsu.kicad_pro** o **spectromatsu.kicad_sch**. Es un proyecto jerárquico conectado eléctricamente, con tres hojas para **dos placas físicas**:

- `sensor.kicad_sch`: piso inferior, C12880MA, TLV9001, TXS0108E, CAT4104, MCP9808, conector del módulo AS7331 y ADS1115 para diagnóstico lento.
- `controller.kicad_sch`: piso superior, XIAO SAMD21, protección de entrada analógica, I²C y conexión USB.
- `power.kicad_sch`: también en el piso superior, TPS61088 y LT3045. Una hoja eléctrica adicional no significa una tercera placa.

**Es una revisión para estudiar y probar en banco, no un diseño liberado para fabricar.** No se han cambiado la carcasa, la posición de la cubeta, la PCB antigua, la app ni el firmware. Ya existen las bases mecánicas de ambas placas; faltan colocación y trazado, una vez cerrada la adquisición y la fuente.

## Hallazgos en el esquema original

La comprobación original produjo 121 incidencias: 25 errores y 96 advertencias. Muchas corresponden a bibliotecas ausentes y símbolos importados; el número no equivale a 121 fallos físicos.

| Bloque | Conexión original verificada en la netlist | Cambio en esta propuesta |
| --- | --- | --- |
| U5, TLV9001 | VIDEO llega a la entrada inversora, pin 3; la salida, pin 4, vuelve a la entrada no inversora, pin 1 | VIDEO al pin 1; realimentación negativa entre pines 4 y 3 |
| U2, C12880MA | El pin 7 TRG aparece en una red llamada GAIN | TRG identificado como salida; el C12880MA no tiene entrada GAIN |
| U1, TXS0108E | OE, pin 10, sin conexión | D8 controla OE; resistencia de 100 kΩ mantiene apagado el traductor al arrancar |
| U10, ADS1115 | Alimentado a 3,3 V, conectado a I²C por el lado B del traductor alimentado a 5 V; recibe VIDEO sin adaptación | I²C directamente a 3,3 V; entrada desde VIDEO/2; retenido para diagnóstico lento |
| U9, LT3045 | SET y OUTS abiertos; PGFB a masa; faltan capacidad de salida y ajuste de tensión adecuados | SET con 49,9 kΩ y condensador, OUTS a salida, PGFB a IN, salida con 22 µF nominales y 100 nF |
| U11, TPS61088 | Inductor entre salida y SW; SS a masa; COMP abierto; no hay condensador BOOT–SW | Inductor VIN–SW, condensadores de arranque y bootstrap, red de compensación propuesta |
| Ajuste del elevador | R7=56,2 kΩ arriba y R4=22,1 kΩ abajo: unos 4,27 V nominales, aunque la red se llama +5.5V | 200 kΩ arriba y 56,2 kΩ abajo: unos 5,49 V nominales en PWM |
| U7, CAT4104 | VIN conectado a PWM, EN/PWM a 5 V, GND mediante 768 Ω y RSET —llamado RESET— directamente a masa | VIN a USB, GND a masa, EN al XIAO y RSET mediante 6,34 kΩ; un canal a unos 20 mA |
| Lista de compra | Mezcla CAT4101/CAT4104; valores de resistencias y condensadores no coinciden con el esquema | La selección de esta revisión se refiere explícitamente a CAT4104V; comprobar la referencia real comprada |
| Inductores | L2 dice 4,7 µH pero su referencia LQW18AS4N7G00D corresponde a otro orden de inductancia; las referencias RF y los encapsulados no establecen un inductor de potencia apropiado | L2 requiere selección de un inductor de potencia apantallado, con corriente de saturación adecuada |
| Bibliotecas y huellas | Rutas absolutas de Linux; el TPS61088 remite a una huella denominada MSP430 | Biblioteca local portable; U2 y U11 se dejan sin huella hasta verificar geometría y encapsulado |

## Contraste con otros proyectos

La electrónica adicional tiene una función real. No se ha descartado por el número de componentes:

- [GroupGets / Pure Engineering, breakout v2](https://groupgets-files.s3.amazonaws.com/hamamatsu/uspectrometer/C12880MA%20Breakout%20Board%20v2%20-%20Datasheet%20-%201.3.pdf), hoja 3: incluye conversión a 5,5 V, regulación posterior a 5 V, buffer analógico, buffers lógicos y controlador LED. Apoya conservar una fuente acondicionada; no demuestra que cualquier elevador o regulador sea obligatorio.
- [Ejemplo Arduino de GroupGets](https://github.com/groupgets/c12880ma/blob/master/arduino_c12880ma_example/arduino_c12880ma_example.ino): usa el ADC interno y lectura por software, con pausas durante la conversión. Es una referencia de funcionamiento, no una validación del ruido o de los límites temporales de nuestro circuito.
- [Utzinger: Teensy, TRG y DMA](https://github.com/uutzinger/C12880MA): muestra otra estrategia, con adquisición sincronizada. La propuesta XIAO requiere desarrollar y comprobar su propia sincronización.
- [Espectrofotómetro portátil, HardwareX 2020](https://pmc.ncbi.nlm.nih.gov/articles/PMC9041175/): emplea Nano y Optisolis. Demuestra que un Nano es una alternativa posible, aunque aquí se elige el XIAO disponible y su USB nativo.
- [Sistema óptico con fibra, HardwareX 2024](https://pmc.ncbi.nlm.nih.gov/articles/PMC11046214/): utiliza ADS1115. Por tanto, no es correcto afirmar que este ADC nunca pueda usarse con el sensor. Su velocidad exige una estrategia de lectura diferente a seguir continuamente cada píxel a 200 kHz.

## Adquisición y selección del micro

Se elige **XIAO SAMD21**, no una variante ESP32 del XIAO. **La siguiente asignación describe el esquema previo de banco y deberá cambiar para el ADC externo.** No se afirma que un SAR SPI adicional quepa sin reasignar señales:

| Pin XIAO | Función |
| --- | --- |
| D0 / A0, PA02 | VIDEO adaptado al ADC interno |
| D1, PA04 | CLK por TCC0 |
| D2 | ST |
| D3 | EOS, a través del traductor |
| D4 / D5 | SDA / SCL a 3,3 V |
| D6, PB08 | TRG, a través del traductor; EIC8 |
| D7 | Encendido del LED |
| D8 | OE del TXS0108E |
| D9 | Libre |
| D10 | Habilitación del elevador |

El ADC interno del [SAMD21](https://ww1.microchip.com/downloads/en/DeviceDoc/SAM-D21DA1-Family-Data-Sheet-DS40001882G.pdf) queda descartado como adquisición principal por decisión de Alex. El micro conserva el trabajo de generación de reloj, sincronización y transferencia USB. Deben comprobarse adquisición, referencia, impedancia de entrada, asentamiento del TLV9001 y fase de muestreo para el ADC externo elegido. No se ha escrito ese firmware en esta revisión.

El [Hamamatsu](https://www.hamamatsu.com/content/dam/hamamatsu-photonics/sites/documents/99_SALES_LIBRARY/ssd/c12880ma_c16767ma_kacc1226e.pdf) especifica CLK de 0,2–5 MHz y 288 píxeles. El [ADS1115](https://www.ti.com/product/ADS1115) llega a 860 muestras/s: no sigue un píxel por ciclo en ese barrido continuo. Aquí queda disponible como diagnóstico de tensión estática, con su conversor apagado durante el barrido rápido.

La entrada analógica previa usa buffer a 5 V, divisor 1 kΩ/1 kΩ, resistencia serie de 4,7 kΩ y diodos Schottky. El divisor limita a 2,625 V para una entrada de 5,25 V. Comprobar tolerancias y secuencia de alimentación; los diodos no son un aislador frente a alimentación inversa. Este circuito no se trasladará sin revisar al SAR: un ADC de condensador conmutado exige comprobar su tiempo de adquisición y el circuito que lo excita. El ADS8326 es un candidato de estudio de 16 bits, 200 kS/s a 2,7 V y 250 kS/s a 5 V, no una selección final ni una compra solicitada. [Ficha de TI](https://www.ti.com/lit/ds/symlink/ads8326.pdf).

## Alimentación e iluminación desde el teléfono

Se conserva **USB → TPS61088 → ~5,5 V → LT3045 → ~5 V analógicos**. El LED se alimenta desde USB con corriente regulada. No hay batería ni lámpara de descarga.

El [LT3045](https://www.analog.com/media/en/technical-documentation/data-sheets/LT3045.pdf) necesita su conexión OUTS, ajuste SET y al menos 10 µF efectivos de salida, no solo un condensador nominal cualquiera. La propuesta usa 22 µF nominales: seleccionar el componente comprobando pérdida de capacidad por polarización y requisitos de ESR/ESL. La corriente límite propuesta ronda 100 mA; esperar al establecimiento de SET antes de medir.

**Los valores START del TPS61088 son propuestas de banco, no una compensación validada.** Recalcular y simular para VIN real del teléfono, carga ligera, inductancia y capacidad efectivas. Comprobar arranque, estabilidad, eficiencia y rizado. La corriente límite del inductor no limita el consumo USB: el arranque y la carga inicial de los condensadores necesitan evaluación, y puede ser necesario un interruptor de carga con control de corriente/pendiente. Mantener PWR_EN bajo no garantiza aislar la salida de un elevador.

Objetivo inicial de diseño: **≤100 mA a 5 V (~0,5 W)** en funcionamiento, pendiente de medidas y del presupuesto USB declarado. Es un objetivo, no una especificación demostrada. Verificar también enumeración USB, corriente en reposo y conexión/desconexión. La entrada y el pigtail deben resolver correctamente los roles USB-C/OTG; este esquema no incluye una terminación CC nueva.

Presupuesto provisional referido a la entrada USB de 5 V; son asignaciones de diseño, no un cálculo de máximos garantizados:

| Bloque | Asignación |
| --- | ---: |
| LED blanco, un canal | 20 mA fijados inicialmente |
| C12880MA | 20 mA típicos a CLK=5 MHz; no es un máximo especificado |
| XIAO y auxiliares de 3,3 V existentes | Reserva de 25 mA; depende del firmware y módulos montados |
| ADC externo nuevo y referencia, si se elige SAR | Reserva adicional de 5 mA; pendiente del circuito elegido |
| Buffer, divisor, controlador LED, regulación y pérdidas de conversión | Reserva de 15 mA; presupone eficiencia razonable del elevador a carga ligera |
| **Total presupuestado** | **85 mA / 0,425 W** |

Los 15 mA finales representan consumo auxiliar y corriente adicional de conversión; no se vuelve a sumar la corriente completa de carga del regulador. El [Hamamatsu](https://www.hamamatsu.com/content/dam/hamamatsu-photonics/sites/documents/99_SALES_LIBRARY/ssd/c12880ma_c16767ma_kacc1226e.pdf) aporta el valor típico del sensor; el [LT3045](https://www.analog.com/en/products/lt3045.html) consume unos 2,2 mA propios. No hay fundamento para afirmar que el equipo necesariamente supere 100 mA. Tampoco se puede garantizar aún ese techo: falta medir el XIAO, seleccionar el ADC y comprobar las pérdidas en PWM forzado. El arranque y la carga de condensadores se evalúan aparte. Los 100 mA son un objetivo de este diseño, no una capacidad universal de todos los teléfonos.

LED candidato: **[Nichia NF2W757G-F1 Optisolis, 5000 K](https://led-ld.nichia.co.jp/jp/product/led_product_data.html?kbn=0&type=NF2W757G-F1+%28Optisolis%29)** sobre una pequeña placa portadora, empezando a unos 20 mA con CAT4104V y RSET=6,34 kΩ. El [CAT4104](https://www.onsemi.com/pdf/datasheet/cat4104-d.pdf) se configura con un solo canal; los demás se dejan abiertos. Medir corriente real y espectro del blanco. Mantener luz continua durante cada exposición; no usar PWM sin sincronización.

El LED blanco no proporciona todo el rango 340–850 nm ni iluminación UV. J6 queda como interfaz heredada sin poblar en esta versión; el AS7331 no forma parte de la adquisición del espectro visible. No se añaden otros LEDs. La banda útil se determinará midiendo el blanco.

## Validación y pendientes

Ejecuta desde cualquier carpeta:

```sh
python3 '/Users/alex/Nube /repos/SpectroMatsu/Hardware/STACKED/check_schematic.py'
```

La revisión pasa ERC sin errores ni advertencias. El control comprueba también alimentación/pines del sensor, realimentación negativa, polaridad de protección, correspondencia J1/J2, control LED y conexiones críticas de los reguladores. **ERC no valida prestaciones analógicas ni estabilidad de la fuente.**

Antes del diseño de PCB quedan por cerrar: compensación e irrupción USB, adquisición sincronizada y ruido, referencias físicas de componentes/huellas U2 y U11, presupuesto real de consumo y adaptación de la óptica. Después se ajustarán ambas placas a la carcasa y cubeta existentes. La memoria W25Q16 y el RTC antiguos permanecen en rev0; no están en esta propuesta conectada al teléfono, que almacenará y fechará las mediciones. Si se requiere funcionamiento autónomo, habrá que reincorporarlos y revisar pines.
