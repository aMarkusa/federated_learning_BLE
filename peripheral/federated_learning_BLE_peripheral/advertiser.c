#include "advertiser.h"
#include "sl_bluetooth.h"
#include "app_assert.h"

uint8_t advertising_set_handle = 0xff;
const uint8_t advertising_data_len = 0x05;
uint8_t advertising_data[] = {advertising_data_len - 1, 0x09, 0x46, 0x4C, 0x50};  // Complete local name FLP = Federated learning peripheral

sl_status_t start_advertising(){
    sl_status_t sc;

    sc = sl_bt_advertiser_create_set(&advertising_set_handle);
    app_assert_status(sc);

    // Generate data for advertising
    sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle, sl_bt_advertiser_advertising_data_packet,
                                          advertising_data_len, advertising_data);
    app_assert_status(sc);

    // Set advertising interval to 100ms.
    sc = sl_bt_advertiser_set_timing(
    advertising_set_handle,
    160, // min. adv. interval (milliseconds * 1.6)
    160, // max. adv. interval (milliseconds * 1.6)
    0,   // adv. duration
    0);  // max. num. adv. events
    app_assert_status(sc);
    // Start advertising and enable connections.
    sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                        sl_bt_legacy_advertiser_connectable);
    return sc;
}