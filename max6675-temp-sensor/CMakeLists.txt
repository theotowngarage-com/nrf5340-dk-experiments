cmake_minimum_required(VERSION 3.13.1)

set(SHIELD adafruit_2_8_tft_touch_v2)


find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(NONE)
zephyr_library_include_directories(configuration)

FILE(GLOB app_sources src/*.c src/images/*.c src/fonts/*.c)
target_sources(app PRIVATE ${app_sources})
