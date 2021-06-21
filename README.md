# nrf5340-dk-experiments
Projects were built and tested with nRF SDK 1.5.0 and zephyr 2.4.99.
More information about the virtual-leash project can be found on hackster.io in project: (will be revealed after contest is over)

# Projects
## virtual-leash
Uses nRF5340-DK running with ZephyrOS and android app built on MIT App inventor. This system signals when two devices are too far apart.

Application on mobile phone measures RSSI every second and transmits to the device.

When power level is below configured setting then red led strip lights up indicating that device is too far. RSSI measurements are sent to custom characteristic as a string.

## max6675-temp-sensor
Uses nRF5340-DK with Adafruit 2.8" Touch display to read and graph temperature.
Sensor is connected to the same SPI port as display (SPI4) using different chip select pin.
Connection:
SCK - P1.15
MISO - P1.14
MOSI - P1.13
CS - P1.01

Before using sensor MAX6675 driver needs to be added to Zephyr with patch in zephyr-patch folder.
Driver uses standard sensor API to read temperature values. First sensor_sample_fetch() method is called, then sensor_channel_get() reads temperature
to structure with 2 integer components. Field val1 contains integer part of temperature register, and val2 contains fractions part.
Temperature returned by the driver has 0.25°C resolution, that corresponds to LSB of val2.

NOTE: sensor that was used for experiments was quite off. Possibly 1 or 2 point calibration is needed to achieve good absolute accuracy.

### What works, and what not
#### Didn't work:
- measuring RSSI to connected device on DK. No API to get RSSI on data packets. hci_pwr_ctl bluetooth example available on ZephyrOS is unfortunately not working on nRF SDK due to a known bug.
- using Flutter with flutter_blue package. No standard library for obtaining RSSI information when connected. 
- using nRF52840 dongle to get RSSI when connected. Troubles setting up connection. Troubles setting USB interface. No API to get RSSI when connected.
- Changing the name of bluetooth Services & characteristics.
#### Did work:
- Using broadcaster-observer to measure RSSI between nRF52840 dongle, and DK. It was however too difficult to figure out how to send data. There were also difficulties setting up phone app to receive broadcasted data.
- nRF52840 dongle as an bluetooth peripheral
- nRF5340DK as a central
- nRF5340DK as a peripheral (final version)
- Using mit app inventor with BluetoothLE extension. Extension API allows reading RSSI while connected, and sending data to custom characteristic.
- LVGL library for display GUI
### What caused struggles
- Kconfig
- Lack of Bluetooth HAL descriptions/explanations in Zephyr's documentation
- Setting up Zephyr Vanilla (main branch) toolchain

### Used IDEs/SDKs/Tools and other notes
#### Segger IDE
- easy to setup & use
- in-built debugging tools
- Based on Eclipse
- Cross-platform IDE
#### Visual Studio Code
- No default debugging, configuration hard to find
- Versatile tool (in-built command line, extensions, docker control)
#### Zephyr-build docker image
- Easy and quick setup of Zephyr SDK
- Can be used in combination with VSCode
