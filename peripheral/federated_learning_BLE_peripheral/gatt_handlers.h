#ifndef GATT_HANDLERS
#define GATT_HANDLERS


#include "gatt_db.h"
#include "sl_status.h"
#include "sl_bluetooth.h"


sl_status_t user_read_request_handler(sl_bt_msg_t *evt, uint8_t conn_handle);
sl_status_t user_write_request_handler(sl_bt_msg_t *evt, uint8_t conn_handle);
sl_status_t uint16_to_uint8(const uint16_t* original_value, uint8_t* final_value, size_t data_len);


#endif /* GATT_HANDLERS */
