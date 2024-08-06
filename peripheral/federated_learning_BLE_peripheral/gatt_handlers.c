#include "gatt_handlers.h"
#include "app.h"

uint8_t att_error_code;
size_t data_len;
uint16_t sent_len;
sl_status_t sc;

sl_status_t user_read_request_handler(sl_bt_msg_t *evt, uint8_t conn_handle) {
    bool case_match = true;
    att_error_code = 0;
    sent_len = 0;
    uint8_t data_buffer[255];
    uint16_t characteristic = evt->data.evt_gatt_server_user_read_request.characteristic;

    switch (characteristic) {
        case gattdb_fv_max_len:
            data_len = sizeof(fv_max_len);
            sc = uint16_to_uint8(&fv_max_len, data_buffer, data_len);
            break;
        case gattdb_lv_max_len:
            data_len = sizeof(lv_max_len);
            sc = uint16_to_uint8(&lv_max_len, data_buffer, data_len);
            break; 
        case gattdb_fv_len:
            data_len = sizeof(fv_len);
            sc = uint16_to_uint8(&fv_len, data_buffer, data_len);
            break;
        case gattdb_lv_len:
            data_len = sizeof(lv_len);
            sc = uint16_to_uint8(&lv_len, data_buffer, data_len);
            break;
        default:
            att_error_code = 0x0A;
            sc = SL_STATUS_BT_ATT_INVALID_HANDLE;
            break;
    }

    app_assert_status(sc);
    app_log_info("GATT Server User Read Response sent.\n");
    sc = sl_bt_gatt_server_send_user_read_response(conn_handle, characteristic, att_error_code,
                                                    data_len, data_buffer, &sent_len);
    
    return sc;
}

sl_status_t uint16_to_uint8(const uint16_t* original_value, uint8_t* final_value, size_t data_len){
    uint8_t bytes[data_len];

    for (uint16_t i = 0; i < data_len; i += 2) {
        uint8_t byte_1 = (original_value[i] >> 8) & 0xFF;
        uint8_t byte_2 = original_value[i] & 0xFF;

        bytes[i] = byte_1;
        bytes[i+1] = byte_2;
    }

    memcpy((void*)final_value, (const void*)bytes, data_len);

    return SL_STATUS_OK;
}