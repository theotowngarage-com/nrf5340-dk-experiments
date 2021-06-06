// #include <zephyr/types.h>
// #include <stddef.h>
// #include <string.h>
// #include <errno.h>
// #include <sys/printk.h>
// #include <sys/byteorder.h>
// #include <zephyr.h>

// #include <bluetooth/bluetooth.h>
// #include <bluetooth/hci.h>
// #include <bluetooth/conn.h>
// #include <bluetooth/uuid.h>
// #include <bluetooth/gatt.h>

// #include "auth_service.h"

// //Zephyr's guide https://docs.zephyrproject.org/latest/reference/bluetooth/gatt.html

// //Unique Universal ID of service
// static struct bt_uuid_128 otown_uuid = BT_UUID_INIT_128(
// 	0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
// 	0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);

// static struct bt_uuid_128 vnd_auth_uuid = BT_UUID_INIT_128(
// 	0xf2, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
// 	0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);

// //Initial value of the service
// static uint8_t otown_value[] = { 'O', ' ', 'T', 'o', 'w', 'n' };

// //Callback function of read command
// static ssize_t read_otown(struct bt_conn *conn, const struct bt_gatt_attr *attr,
// 			void *buf, uint16_t len, uint16_t offset)
// {
//     //pointer to data in GATT structure
// 	const char *value = attr->user_data;

// 	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
// 				 strlen(value));
// }
// //Callback function of write command
// static ssize_t write_otown(struct bt_conn *conn, const struct bt_gatt_attr *attr,
// 			 const void *buf, uint16_t len, uint16_t offset,
// 			 uint8_t flags)
// {
// 	uint8_t *value = attr->user_data;

// 	if (offset + len > sizeof(otown_value)) {
// 		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
// 	}

// 	memcpy(value + offset, buf, len);

// 	return len;
// }
// //Client Characteristic Configuration.
// static void vnd_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value) //Client Characterictics Configuration
// {
	
// }

// /* Primary Service Declaration */
// BT_GATT_SERVICE_DEFINE(otown_svc, //create a struct with _name
// 	BT_GATT_PRIMARY_SERVICE(&otown_uuid),
//     BT_GATT_CHARACTERISTIC(&vnd_auth_uuid.uuid,
// 			       BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
// 			       BT_GATT_PERM_READ_AUTHEN |
// 			       BT_GATT_PERM_WRITE_AUTHEN,
// 			       read_otown, write_otown, otown_value),
// 	BT_GATT_CCC(vnd_ccc_cfg_changed,
// 		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE_ENCRYPT),
// 	);

// static void connected(struct bt_conn *conn, uint8_t err)
// {
// 	if (err) {
// 		printk("Connection failed (err 0x%02x)\n", err);
// 	} else {
// 		printk("Connected\n");
// 	}
// }

// static void disconnected(struct bt_conn *conn, uint8_t reason)
// {
// 	printk("Disconnected (reason 0x%02x)\n", reason);
// }

// static struct bt_conn_cb conn_callbacks = {
// 	.connected = connected,
// 	.disconnected = disconnected,
// };

// static void bt_ready(void)
// {
// 	int err;

// 	printk("Bluetooth initialized\n");

// 	if (IS_ENABLED(CONFIG_SETTINGS)) {
// 		settings_load();
// 	}

// 	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0); //Advertising parameters, data to be advertised and used for response
// 	if (err) {
// 		printk("Advertising failed to start (err %d)\n", err);
// 		return;
// 	}

// 	printk("Advertising successfully started\n");
// }

// //Functioned called when Autharisation is requested
// static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
// {
// 	char addr[BT_ADDR_LE_STR_LEN];

// 	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

// 	printk("Passkey for %s: %06u\n", addr, passkey);
// }

// static void auth_cancel(struct bt_conn *conn)
// {
// 	char addr[BT_ADDR_LE_STR_LEN];

// 	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

// 	printk("Pairing cancelled: %s\n", addr);
// }

// //callback in case of autherisation
// static struct bt_conn_auth_cb auth_cb_display = {
// 	.passkey_display = auth_passkey_display,
// 	.passkey_entry = NULL,
// 	.cancel = auth_cancel,
// };