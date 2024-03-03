cmake_minimum_required(VERSION 3.20)

project(
    samd21-hal
    DESCRIPTION "SAMD21 Hardware Abstraction Layer"
    LANGUAGES C CXX ASM)
set(PROJECT_VERSION "0.1.0")

option(SAMD21_HAL_BUILD_TESTS "Enables library test cases" OFF)

# Tools
include(${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/application.cmake)

# Documentation
#add_subdirectory(docs)

add_library(cmsis INTERFACE)
target_include_directories(cmsis INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/CMSIS/CMSIS/Core/Include)

add_library(atsamd21e18a-dfp INTERFACE)
target_include_directories(atsamd21e18a-dfp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/ATSAMD21E18A_DFP)

add_library(samd21-hal-api INTERFACE)
target_include_directories(samd21-hal-api INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(samd21-hal-sources INTERFACE)
target_sources(samd21-hal-sources INTERFACE
    src/hal/ac.c
    src/hal/adc.c
    src/hal/dsu.c
    src/hal/eic.c
    src/hal/gclk.c
    src/hal/gpio.c
    src/hal/pm.c
    src/hal/evsys.c
    src/hal/rtc.c
    src/hal/nvic.c
    src/hal/nvmctrl.c
    src/hal/tc.c
    src/hal/tcc.c
    src/hal/wdt.c
    src/hal/startup.c
    src/hal/sysctrl.c
    src/hal/sercom_usart.c

    src/common/ringbuffer.c
    src/common/swtimer.c
    src/common/scheduler.c
)

add_library(samd21-hal-options INTERFACE)
target_compile_options(samd21-hal-options INTERFACE "-mcpu=cortex-m0plus" "-gdwarf-2" "-mthumb")
target_link_options(samd21-hal-options INTERFACE "-mcpu=cortex-m0plus" "-mthumb")
target_link_directories(samd21-hal-options INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ld)

if(SAMD21_HAL_BUILD_TESTS)

    add_library(samd21-hal-test-lib STATIC)
    target_link_libraries(samd21-hal-test-lib PRIVATE cmsis
                                                  atsamd21e18a-dfp
                                                  samd21-hal-api
                                                  samd21-hal-sources
                                                  samd21-hal-options)

    add_executable(samd21-hal-test EXCLUDE_FROM_ALL
        tests/main.c
    )
    target_link_libraries(samd21-hal-test PRIVATE samd21-hal-test-lib)
    target_compile_options(samd21-hal-test PRIVATE "-mcpu=cortex-m0plus" "-gdwarf-2" "-mthumb")
    target_link_options(samd21-hal-test PRIVATE "-mcpu=cortex-m0plus" "-mthumb")
    target_link_options(samd21-hal-test PRIVATE -TATSAMD21E18A.ld)
    target_link_options(samd21-hal-test PRIVATE -Wl,-Map=samd21-test.map -Wl,--gc-sections)
    set_target_properties(samd21-hal-test PROPERTIES SUFFIX ".elf")
endif()
