#include "gatt_handlers.h"
#include "app.h"



sl_status_t user_read_request_handler(sl_bt_msg_t* evt, uint8_t conn_handle) {
    sl_status_t sc = SL_STATUS_OK;
    uint8_t att_error_code = 0;
    size_t data_len;
    uint8_t data_buffer[256];
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

    app_assert_status(sc);  // FIXME: If the default case runs this will stop the app

    uint16_t bytes_sent = 0;  // the amount of bytes sent
    sc = sl_bt_gatt_server_send_user_read_response(conn_handle, characteristic, att_error_code,
                                                    data_len, data_buffer, &bytes_sent);

    app_assert_status(sc);
    app_log_info("GATT Server User Read Response sent.\n");
    
    return sc;
}

sl_status_t user_write_request_handler(sl_bt_msg_t* evt, uint8_t conn_handle) {
    sl_status_t sc = SL_STATUS_OK;
    uint8_t att_error_code = 0;
    size_t data_len;
    uint8_t data_buffer[256];
    uint16_t characteristic = evt->data.evt_gatt_server_user_write_request.characteristic;

    switch (characteristic) {
        case gattdb_model:
            break;
        case gattdb_model_params:
            break;
        case gattdb_hyperparams:
            break;
        
        default:
            break;
    }

    sl_bt_gatt_att_opcode_t att_opcode = evt->data.evt_gatt_server_user_write_request.att_opcode;

    switch (att_opcode)
    {
    case sl_bt_gatt_write_request:
        sl_bt_gatt_server_send_user_write_response(conn_handle, characteristic, att_error_code);
        break;
    
    default:
        att_error_code = 0x0A;
        sc = SL_STATUS_BT_ATT_INVALID_HANDLE;
        break;
    }

    app_assert_status(sc);  // FIXME: If the default case runs this will stop the app
    app_log_info("GATT Server User Write Response sent.\n");

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