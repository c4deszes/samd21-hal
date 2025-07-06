# SAMD21 HAL

Hardware Abstraction Layer for SAMD21 microcontrollers.

## Integration

The library can be included as a submodule or via CPM.cmake, in either case the master branch can
be used.

A function is registered which when used will create a new interface library with everything
included. See the example below on how to use it.

### Example

```cmake
include(tools/cmake/CPM.cmake)
CPMAddPackage("gh:c4deszes/samd21-hal#master")

samx21_hal(samd21 ATSAMD21E18A)

add_executable(MyApp src/main.c)
target_link_libraries(MyApp PUBLIC samd21)
target_link_options(MyApp PRIVATE -T$<TARGET_PROPERTY:samd21,DEFAULT_LINKERSCRIPT>)
```

## Peripherals

| Name | Abbreviation | Status |
|-|-|-|
| Analog Comparator | AC | Non functional |
| Analog-to-Digital Converter | ADC | Single shot mode available |
| Digital-to-Analog Converter | DAC | Not implemented |
| Direct Memory Access Controller | DMAC | Not implemented |
| Device Signature Unit | DSU | Only supports CRC and Serial number |
| External Interrupt Controller | EIC | Non functional |
| Event System | EVSYS | No interrupt support |
| Generic Clock Controller | GCLK | Complete |
| I/O Pin Controller | GPIO | Complete |
| Inter-IC Sound Controller | I2S | Not implemented |
| Nonvolatile Memory Controller | NVMCTRL | No interrupts, no locking |
| Peripheral Touch Controller | PTC | Not implemented |
| Power Manager | PM | No interrupt support, cannot control all peripherals |
| Real Time Clock | RTC | Non functional |
| SERCOM USART | SERCOM_USART | Only supports async mode, fixed 8/N/1 |
| SERCOM SPI | SERCOM_SPI | Implemented |
| SERCOM I2C | SERCOM_I2C | Not implemented |
| System Controller | SYSCTRL | Only supports DFLL, OSC8M, OSC32K |
| Timer Counter | TC | Only supports capture mode |
| Timer Counter for Control | TCC | Only supports basic count up to period and interrupt |
| USB Controller | USB | Not implemented |
| Watchdog | WDT | No windowed mode support |

## Components

- Ringbuffer: implements ringbuffers for 8, 16 and 32 bit unsigned integers
- Software Timer: implements a count down mechanism
- Scheduler: implements a round robin scheduler
