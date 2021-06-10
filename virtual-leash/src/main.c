/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
//Zephyr bluetooth stack: https://docs.zephyrproject.org/latest/guides/bluetooth/bluetooth-arch.html

#include <zephyr.h>
#include <settings/settings.h>
#include <bluetooth/bluetooth.h>
#include "gpio.h"
#include "service.h"
#include "gui.h"

// logging module configuration
#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(app);

void on_gui_event(gui_event_t *event) {
  switch (event->evt_type) {
  case GUI_EVT_BUTTON_PRESSED:
    LOG_INF("Button pressed");
    //app_ble_send_button_state(event->button_checked);
    break;
  }
}

void main(void) {
  int err;
  err = bt_enable(NULL);
  if (err) {
    LOG_ERR("Bluetooth init failed (err %d)\n", err);
    return;
  }

  gpio_init();

  LOG_INF("Configuring GUI");
  gui_config_t gui_config = {.event_callback = on_gui_event};
  gui_init(&gui_config);

  LOG_INF("Initialize service, start advertising & initialize callback");
  bt_ready();

  LOG_INF("Application started");
  while (1) {
    k_sleep(K_MSEC(50));
    bt_service_spin();
  }
}