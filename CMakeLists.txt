cmake_minimum_required(VERSION 3.20)

project(
    samd21-hal
    DESCRIPTION "SAMD21 Hardware Abstraction Layer"
    LANGUAGES C CXX ASM)
set(PROJECT_VERSION "0.2.0")

option(SAMD21_HAL_BUILD_TESTS "Enables library test cases" OFF)

# Tools
include(${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/application.cmake)

# Documentation
#add_subdirectory(docs)

add_library(cmsis INTERFACE)
target_include_directories(cmsis INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/CMSIS/CMSIS/Core/Include)

# add_library(atsamd21e18a-dfp INTERFACE)
# target_include_directories(atsamd21e18a-dfp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/ATSAMD21E18A_DFP)

add_library(samx21-sources INTERFACE)
target_sources(samx21-sources INTERFACE
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
    src/hal/sysctrl.c
    src/hal/sercom_usart.c
    src/hal/sercom_spi.c
    src/hal/sercom_common.c

    src/common/ringbuffer.c
    src/common/swtimer.c
    src/common/scheduler.c
)

add_library(samx21-api INTERFACE)
target_include_directories(samx21-api INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

function(samx21_hal TARGET CHIPNAME)

    add_library(${TARGET} INTERFACE)

    string(SUBSTRING ${CHIPNAME} 0 8 CHIPFAMILY)
    string(TOLOWER ${CHIPNAME} CHIPNAME_LOW)
    string(SUBSTRING ${CHIPNAME_LOW} 2 -1 CHIPNAME_LOW_NOAT)

    target_sources(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CHIPFAMILY}/src/startup_${CHIPNAME_LOW_NOAT}.c)
    target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CHIPFAMILY}/include)
    target_link_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CHIPFAMILY}/ld)
    set_target_properties(${TARGET} PROPERTIES DEFAULT_LINKERSCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/lib/${CHIPFAMILY}/ld/${CHIPNAME_LOW_NOAT}_flash.ld")

    target_compile_definitions(${TARGET} INTERFACE "__${CHIPNAME}__")
    target_compile_options(${TARGET} INTERFACE "-mcpu=cortex-m0plus" "-gdwarf-2" "-mthumb")
    target_link_options(${TARGET} INTERFACE "-mcpu=cortex-m0plus" "-mthumb")

    target_link_libraries(${TARGET} INTERFACE   cmsis
                                                samx21-api
                                                samx21-sources)

endfunction()

if(SAMD21_HAL_BUILD_TESTS)
    samx21_hal(samd21-hal-test-lib ATSAMD21E18A)
    add_executable(samd21-hal-test EXCLUDE_FROM_ALL
        tests/main.c
    )
    target_link_libraries(samd21-hal-test PRIVATE samd21-hal-test-lib)
    set_target_properties(samd21-hal-test PROPERTIES LINK_DEPENDS "$<TARGET_PROPERTY:samd21-hal-test-lib,DEFAULT_LINKERSCRIPT>")
    target_link_options(samd21-hal-test PRIVATE -T$<TARGET_PROPERTY:samd21-hal-test-lib,DEFAULT_LINKERSCRIPT>)
    target_link_options(samd21-hal-test PRIVATE -Wl,-Map=samd21-test.map -Wl,--gc-sections)
    set_target_properties(samd21-hal-test PROPERTIES SUFFIX ".elf")
endif()
