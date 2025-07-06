################################################################################
# Library
################################################################################
samx21_hal(samd21-hal-test-lib ATSAMD21E18A)

################################################################################
# Test Application
################################################################################
add_executable(samd21-hal-test EXCLUDE_FROM_ALL
    main.c
)
target_link_libraries(samd21-hal-test PRIVATE samd21-hal-test-lib)
set_target_properties(samd21-hal-test PROPERTIES LINK_DEPENDS "$<TARGET_PROPERTY:samd21-hal-test-lib,DEFAULT_LINKERSCRIPT>")
target_link_options(samd21-hal-test PRIVATE -T$<TARGET_PROPERTY:samd21-hal-test-lib,DEFAULT_LINKERSCRIPT>)
target_link_options(samd21-hal-test PRIVATE -Wl,-Map=samd21-test.map -Wl,--gc-sections)
set_target_properties(samd21-hal-test PROPERTIES SUFFIX ".elf")
