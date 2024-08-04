#include "gatt_handlers.h"
#include "app.h"

uint8_t att_error_code;
uint8_t* data
uint8_t value_len;
uint16_t sent_len;
sl_status_t sc;

sl_status_t user_read_request_handler(sl_bt_msg_t *evt, uint8_t conn_handle) {
    att_error_code = 0;
    sent_len = 0;
    uint16_t characteristic = evt->data.evt_gatt_server_user_read_request.characteristic;

    switch (characteristic)
    {
    case gattdb_fv_max_len:
        value_len = sizeof(fv_max_len);
        sc = sl_bt_gatt_server_send_user_read_response(conn_handle, characteristic, att_error_code,
        sizeof(fv_max_len), &fv_max_len, &sent_len);
        break;
    case gattdb_lv_max_len:

        break;
    
    default:
        break;
    }

    return sc;
}