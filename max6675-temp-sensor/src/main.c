#include <sys/printk.h>
#include <zephyr.h>

#include "gui.h"
#include <drivers/sensor.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(hello_world);

void on_gui_event(gui_event_t *event) {
  switch (event->evt_type) {
  case GUI_EVT_BUTTON_PRESSED:
    LOG_INF("Button pressed");
    //app_ble_send_button_state(event->button_checked);
    break;
  }
}

void main(void) {
  LOG_INF("Configuring GUI");
  gui_config_t gui_config = {.event_callback = on_gui_event};
  gui_init(&gui_config);

  LOG_INF("Configuring MAX6675");
  const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, maxim_max6675)));

  if (dev == NULL) {
    printk("Could not get MAX6675 device\n");
    return;
  }

  printk("dev %p name %s\n", dev, dev->name);

  while (true) {
    struct sensor_value temp;
    //LOG_INF("Fetching sample");
    int ret = sensor_sample_fetch(dev);
    if (ret) {
      LOG_ERR("sensor_sample_fetch %d", ret);
    }
    //LOG_INF("Reading value");
    ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
    if (ret) {
      LOG_ERR("sensor_channel_get %d", ret);
    }
    LOG_INF("Temperature: %d.%d", temp.val1, 25 * temp.val2);
    gui_add_point_to_chart(temp.val1 + .25f * temp.val2);
    k_sleep(K_MSEC(1000));

    //LOG_INF("Hello log info");
    //LOG_DBG("Hello log debug");
    //int x = 5;
    //int y = 2;
    //int result = multiply(x, y);
    //LOG_INF("Multiply result %d", result);
  }
}