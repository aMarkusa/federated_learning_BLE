/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "em_common.h"
#include "sl_bluetooth.h"
#include "app.h"
#include "advertiser.h"
#include "gatt_handlers.h"


// The advertising set handle allocated from Bluetooth stack.
static uint8_t conn_handle = 0xff;
app_fsm_t app_fsm;

const uint16_t fv_max_len = 1000;
const uint16_t lv_max_len = 1000;
uint16_t fv_len = 0;
uint16_t lv_len = 0;
uint8_t mtu = 247;


/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{
  	app_fsm.current_state = IDLE;
	app_fsm.previous_state = IDLE;
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application code here!                              //
  // This is called infinitely.                                              //
  // Do not call blocking functions from here!                               //
  /////////////////////////////////////////////////////////////////////////////
}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;
  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_bt_evt_system_boot_id:
		app_log_info("System booted.\n");
      	sc = start_advertising();
      	app_assert_status(sc);
		app_log_info("Advertising started.\n");
      	break;

    case sl_bt_evt_connection_opened_id:
		conn_handle = evt->data.evt_connection_opened.connection;
		app_log_info("Connection opened.\n");
        app_set_new_state(CONNECTED);
      	break;

    case sl_bt_evt_connection_closed_id:
		app_log_info("Connection closed.\n");
		app_set_new_state(IDLE);	
      	break;

	case sl_bt_evt_gatt_mtu_exchanged_id:
		mtu = evt->data.evt_gatt_mtu_exchanged.mtu;
		break;

	case sl_bt_evt_gatt_server_user_read_request_id:
		app_log_info("GATT Server User Read Request received.\n");
		sc = user_read_request_handler(evt, conn_handle);
		app_assert_status(sc);
		break;
    default:
      break;
  }
}

void app_set_new_state(app_state_t new_state) {
	app_fsm.previous_state = app_fsm.current_state;
	app_fsm.current_state = new_state;
}
