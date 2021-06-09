# nrf5340-dk-experiments

# Projects
## virtual-leash
Uses nRF5340-DK running with ZephyrOS and android app built on MIT App inventor. This system signals when two devices are too far apart.

Application on mobile phone measures RSSI every second and transmits to the device.

When power level is below configured setting then red led strip lights up indicating that device is too far. RSSI measurements are sent to custom characteristic as a string.

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
###What caused struggles
- Kconfig
- Lack of Bluetooth HAL descriptions/explanations in Zephyr's documentation
- Setting up Zephyr Vanilla (main branch) toolchain

###Used IDEs/SDKs/Tools and other notes
####Segger IDE
- easy to setup & use
- in-built debugging tools
- Based on Eclipse
- Cross-platform IDE
####Visual Studio Code
- No default debugging, configuration hard to find
- Versatile tool (in-built command line, extensions, docker control)
####Zephyr-build docker image
- Easy and quick setup of Zephyr SDK
- Can be used in combination with VSCode
