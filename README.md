# nrf5340-dk-experiments

# Projects
## virtual-leash
Uses nRF5340-DK and android app running onthe phone to signal when two devices are too far apart.

Application on mobile phone measures RSSI every second and transmits to the device.

When power level is below configured setting then red led strip lights up indicating that device is too far. RSSI measurements are sent to custom characteristic as a string.

### What works, and what not
#### Didn't work:
- measuring RSSI to connected device on DK. No API to get RSSI on data packets.
- using Flutter with flutter_blue package. No RSSI information when connected.
- using nRF52840 dongle to get RSSI when connected. Troubles setting up connection. Troubles setting USB interface. No API to get RSSI when connected.
#### Did work:
- using broadcaster-observer to measure RSSI between nRF52840 dongle, and DK. It was however too difficult to figure out how to send data. There were also difficulties setting up phone app to receive broadcasted data.
- using mit app inventor with BluetoothLE extension. Extension API allows reading RSSI while connected, and sending data to custom characteristic.