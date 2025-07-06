cmake_minimum_required(VERSION 3.20)

project(
    samd21-hal
    DESCRIPTION "SAMD21 Hardware Abstraction Layer"
    LANGUAGES C CXX ASM)
set(PROJECT_VERSION "0.4.0")

################################################################################
# Options
################################################################################
option(SAMD21_HAL_BUILD_TESTS "Enables library test cases" OFF)
option(SAMD21_HAL_BUILD_DOCS "Enables documentation build" OFF)

# Tools
# TODO: ideally this is included in the toolchain file
include(${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/application.cmake)

################################################################################
# CMSIS
################################################################################
add_library(cmsis INTERFACE)
target_include_directories(cmsis INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/CMSIS/CMSIS/Core/Include)

################################################################################
# Sources
################################################################################
add_library(samx21-sources INTERFACE)
target_sources(samx21-sources INTERFACE
    # Input output
    src/hal/ac.c
    src/hal/adc.c
    src/hal/eic.c
    src/hal/gpio.c

    # Timing
    src/hal/rtc.c
    src/hal/tc.c
    src/hal/tcc.c

    # Memory
    src/hal/nvmctrl.c

    # System
    src/hal/evsys.c
    src/hal/gclk.c
    src/hal/pm.c
    src/hal/dsu.c
    src/hal/wdt.c
    src/hal/sysctrl.c
    src/hal/nvic.c

    # Communication
    src/hal/sercom_usart.c
    src/hal/sercom_spi.c
    src/hal/sercom_common.c

    # Utilities
    src/common/ringbuffer.c
    src/common/swtimer.c
    src/common/scheduler.c
)

################################################################################
# API
################################################################################
add_library(samx21-api INTERFACE)
target_include_directories(samx21-api INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

function(samx21_hal TARGET CHIPNAME)
    add_library(${TARGET} INTERFACE)

    # Determine chip family and name
    string(SUBSTRING ${CHIPNAME} 0 8 CHIPFAMILY)
    string(TOLOWER ${CHIPNAME} CHIPNAME_LOW)
    string(SUBSTRING ${CHIPNAME_LOW} 2 -1 CHIPNAME_LOW_NOAT)

    # Set properties from Device family pack
    target_sources(${TARGET} INTERFACE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/${CHIPFAMILY}/src/startup_${CHIPNAME_LOW_NOAT}.c)
    target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/${CHIPFAMILY}/include)
    target_link_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/${CHIPFAMILY}/ld)
    set_target_properties(${TARGET} PROPERTIES DEFAULT_LINKERSCRIPT "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/${CHIPFAMILY}/ld/${CHIPNAME_LOW_NOAT}_flash.ld")

    target_compile_definitions(${TARGET} INTERFACE "__${CHIPNAME}__" "__${CHIPFAMILY}__")
    target_compile_options(${TARGET} INTERFACE "-mcpu=cortex-m0plus" "-mthumb")
    target_link_options(${TARGET} INTERFACE "-mcpu=cortex-m0plus" "-mthumb")

    target_link_libraries(${TARGET} INTERFACE   cmsis
                                                samx21-api
                                                samx21-sources)
endfunction()

################################################################################
# Testing
################################################################################
if(SAMD21_HAL_BUILD_TESTS)
    add_subdirectory(tests/example)
endif()

################################################################################
# Documentation
################################################################################
if (SAMD21_HAL_BUILD_DOCS)
    add_subdirectory(docs)
endif()
