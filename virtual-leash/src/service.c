#include <errno.h>
#include <stddef.h>
#include <stdint.h> //pointer to integer
#include <string.h>
#include <stdlib.h>
#include <sys/byteorder.h>
#include <sys/printk.h>
#include <zephyr.h>
#include <zephyr/types.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>
#include <bluetooth/uuid.h>
#include <settings/settings.h>

#include "gpio.h"
#include "service.h"
#include "gui.h"

//Zephyr's guide https://docs.zephyrproject.org/latest/reference/bluetooth/gatt.html

//Unique Universal ID of service
#define OTOWN_UUID                      BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x39342d62, 0x3932, 0x662d, 0x6538, 0x313134343332))
#define REMOTE_RSSI_CHARACTERISTIC_UUID BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x63342d31, 0x3836, 0x372d, 0x3166, 0x306331633562))
#define DETACH_CHARACTERISTIC_UUID      BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x1e086d95, 0x7faa, 0x4993, 0x984e, 0xcf234cec373b))

#define DETACH_COMMAND "detach"

//Initial value of the service
static uint8_t otown_value[16];
static char detach_request[16];
static bool detached_safely = false;

static ssize_t read_otown(struct bt_conn *conn, const struct bt_gatt_attr *attr,
    void *buf, uint16_t len, uint16_t offset) {
  //pointer to data in GATT structure
  const char *value = attr->user_data;

  return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
      strlen(value));
}

//Callback function of write command
static ssize_t write_otown(struct bt_conn *conn, const struct bt_gatt_attr *attr,
    const void *buf, uint16_t len, uint16_t offset,
    uint8_t flags) {
  uint8_t *value = attr->user_data;

  if (offset + len > sizeof(otown_value)) {
    return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
  }
  memcpy(value + offset, buf, len);

  //Print value
  printk("Value = %s\n", value);

  //RSSI LED trigger
  int value_int = atoi(value);
  printk("value_int = %d\n", value_int);
  if (value_int < -80) {
    gpio_set_red(true);
  } else {
    gpio_set_red(false);
    gpio_set_green(true);
  }

  //gui_add_point_to_chart(value_int);
  return len;
}

static ssize_t write_detach(struct bt_conn *conn, const struct bt_gatt_attr *attr,
    const void *buf, uint16_t len, uint16_t offset,
    uint8_t flags) {
  uint8_t *value = attr->user_data;

  if (offset + len > sizeof(detach_request)) {
    return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
  }
  memcpy(value + offset, buf, len);

  //Print value
  printk("Detach = %s\n", value);

  if(strncmp(value, DETACH_COMMAND, strlen(DETACH_COMMAND)) == 0) {
    printk("Detaching from phone\n");
    detached_safely = true;
  }

  return len;
}

//Client Characteristic Configuration.
static void vnd_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value) //Client Characterictics Configuration
{
}

/* Primary Service Declaration */
BT_GATT_SERVICE_DEFINE(otown_svc,               //create a struct with _name
    BT_GATT_PRIMARY_SERVICE(OTOWN_UUID),       //Main UUID
    BT_GATT_CHARACTERISTIC(REMOTE_RSSI_CHARACTERISTIC_UUID,  //Charasteristics attribute UUID
        BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,   //Properties
        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, // permissions read/write no security
        read_otown, write_otown, otown_value),  //Callback functions and value
    BT_GATT_CHARACTERISTIC(DETACH_CHARACTERISTIC_UUID,  //Charasteristics attribute UUID
        BT_GATT_CHRC_WRITE,   //Properties
        BT_GATT_PERM_WRITE, // permissions read/write no security
        NULL, write_detach, detach_request),  //Callback functions and value
    BT_GATT_CCC(vnd_ccc_cfg_changed,            //Client Configuration Configuration
        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE_ENCRYPT),
        );

//Advertising address
static const struct bt_data advertising_data[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    // last service advertised UUID
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_128_ENCODE(0x39342d62, 0x3932, 0x662d, 0x6538, 0x313134343332)),
    // this doesn't work, advertising fails -22
    //BT_DATA_BYTES(BT_DATA_NAME_COMPLETE, 'O', ' ', 'T', 'o', 'w', 'n'),
};

static void connected(struct bt_conn *conn, uint8_t err) {
  if (err) {
    printk("Connection failed (err 0x%02x)\n", err);
  } else {
    printk("Connected\n");
  }
  detached_safely = false;
}

static void disconnected(struct bt_conn *conn, uint8_t reason) {
  printk("Disconnected (reason 0x%02x)\n", reason);
  // turn on red leds if remote device did not detach safely before disconnecting
  if(!detached_safely) {
    gpio_set_red(true);
  }
}

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
};

void bt_ready(void) {
  int err;

  printk("Bluetooth initialized\n");

  if (IS_ENABLED(CONFIG_SETTINGS)) {
    settings_load();
  }

  // register connect and disonnect callbacks
  bt_conn_cb_register(&conn_callbacks);

  err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, advertising_data, ARRAY_SIZE(advertising_data), NULL, 0); //Advertising parameters, data to be advertised and used for response
  if (err) {
    printk("Advertising failed to start (err %d)\n", err);
    return;
  }
  printk("Advertising successfully started\n");
}

void bt_service_spin() {
    gpio_set_green(false);
}
