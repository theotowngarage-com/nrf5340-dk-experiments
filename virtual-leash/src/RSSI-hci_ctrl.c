// //Controller - from hci_pwr_ctrl
// static struct bt_conn *default_conn;
// static uint16_t default_conn_handle;
// static struct k_thread pwr_thread_data;
// static K_THREAD_STACK_DEFINE(pwr_thread_stack, 320);
// //

// static void read_conn_rssi(uint16_t handle, int8_t *rssi)

// //only read_conn_rssi needed
// void modulate_tx_power(void *p1, void *p2, void *p3)
// {
// 	int8_t txp_get = 0;
// 	uint8_t idx = 0;

// 	while (1) {
// 		if (!default_conn) {
// 			k_sleep(K_SECONDS(5));
// 		} else {
// 			int8_t rssi = 0xFF;
// 			read_conn_rssi(default_conn_handle, &rssi);
// 			printk("Connected (%d) - RSSI = %d\n",
// 			       default_conn_handle, rssi);

// 			k_sleep(K_SECONDS(1));
// 		}
// 	}
// }
// {
// 	struct net_buf *buf, *rsp = NULL;
// 	struct bt_hci_cp_read_rssi *cp;
// 	struct bt_hci_rp_read_rssi *rp;

// 	int err;

// 	buf = bt_hci_cmd_create(BT_HCI_OP_READ_RSSI, sizeof(*cp));
// 	if (!buf) {
// 		printk("Unable to allocate command buffer\n");
// 		return;
// 	}

// 	cp = net_buf_add(buf, sizeof(*cp));
// 	cp->handle = sys_cpu_to_le16(handle);

// 	err = bt_hci_cmd_send_sync(BT_HCI_OP_READ_RSSI, buf, &rsp);
// 	if (err) {
// 		uint8_t reason = rsp ?
// 			((struct bt_hci_rp_read_rssi *)rsp->data)->status : 0;
// 		printk("Read RSSI err: %d reason 0x%02x\n", err, reason);
// 		return;
// 	}

// 	rp = (void *)rsp->data;
// 	*rssi = rp->rssi;

// 	net_buf_unref(rsp);
// }

// static void connected(struct bt_conn *conn, uint8_t err)
// {
// 	char addr[BT_ADDR_LE_STR_LEN];
// 	int ret;
// 	if (err) {
// 		printk("Connection failed (err 0x%02x)\n", err);
// 	} 
// 	else {
// 		default_conn = bt_conn_ref(conn);
// 		ret = bt_hci_get_conn_handle(default_conn,
// 					     &default_conn_handle);
// 		if (ret) {
// 			printk("No connection handle (err %d)\n", ret);
// 		} 
// 		else {
// 			/* Send first at the default selected power */
// 			bt_addr_le_to_str(bt_conn_get_dst(conn),
// 							  addr, sizeof(addr));
// 			printk("Connected via connection (%d)",default_conn_handle);
// 		}
// 	}
// }

// static void disconnected(struct bt_conn *conn, uint8_t reason)
// {
// 	if (default_conn) {
// 		bt_conn_unref(default_conn);
// 		default_conn = NULL;
// 	}
// 	printk("Disconnected (reason 0x%02x)\n", reason);
	
// }

// // k_thread_create(&pwr_thread_data, pwr_thread_stack,
// 	// 		K_THREAD_STACK_SIZEOF(pwr_thread_stack),
// 	// 		modulate_tx_power, NULL, NULL, NULL,
// 	// 		K_PRIO_COOP(10),
// 	// 		0, K_NO_WAIT);
// 	// k_thread_name_set(&pwr_thread_data, "DYN TX");	

// // int8_t rssi = 0xFF;
// 		// read_conn_rssi(default_conn_handle, &rssi);
// 		// printk("Connected (%d) - RSSI = %d\n", default_conn_handle, rssi);
//         // int main(){
//         //     default_conn = NULL;
//         // }