cmake_minimum_required(VERSION 3.20)

project(dual_sensor_thermometer C)

# --- НАСТРОЙ ПОД СВОЙ МК ---
# MCU и частота бери из .cproj
set(MCU atmega328p)
set(F_CPU 8000000UL)

# --- ТУЛЧЕЙН ИЗ ATMEL STUDIO ---
set(AVR_TOOLCHAIN "C:/Program Files (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain")
set(AVR_BIN "${AVR_TOOLCHAIN}/bin")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER "${AVR_BIN}/avr-gcc.exe")
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# жёстко переопределяем, как линковать, чтобы не было --out-implib
set(CMAKE_C_LINK_EXECUTABLE
        "\"${CMAKE_C_COMPILER}\" <OBJECTS> -o <TARGET> ${CMAKE_C_FLAGS} ${CMAKE_EXE_LINKER_FLAGS}"
)

set(AVR_OBJCOPY "${AVR_BIN}/avr-objcopy.exe")
set(AVR_SIZE    "${AVR_BIN}/avr-size.exe")

set(CMAKE_C_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os -std=gnu11 -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS "-mmcu=${MCU}")

# --- HEADER-пути (чтобы #include "..." находились) ---
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/bsp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hal"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/handlers"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/display"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/temperature"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/tests"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/utils"
)

# --- ИСХОДНИКИ ---
file(GLOB SRC_FILES
        "src/main.c"
        "src/init.c"
        "src/bsp/*.c"
        "src/hal/*.c"
        "src/handlers/*.c"
        "src/lib/display/*.c"
        "src/lib/temperature/*.c"
        "src/utils/*.c"
        # "src/tests/*.c"  # тесты пока не линкуем в прошивку
)

# ВАЖНО: имя таргета без .elf, суффикс добавится сам
add_executable(${PROJECT_NAME} ${SRC_FILES})

# --- после сборки делаем HEX и показываем размер ---
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${AVR_OBJCOPY} -O ihex -R .eeprom
        $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
        COMMAND ${AVR_SIZE} -C --mcu=${MCU} $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Создаю HEX и показываю размер"
)

