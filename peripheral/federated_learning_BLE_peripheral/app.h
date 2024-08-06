/***************************************************************************//**
 * @file
 * @brief Application interface provided to main().
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

#ifndef APP
#define APP

#include <stdint.h>
#include "app_assert.h"
#include "app_log.h"


extern const uint16_t fv_max_len;
extern const uint16_t lv_max_len;
extern uint16_t fv_len;
extern uint16_t lv_len;
extern uint8_t mtu;

typedef enum {
    IDLE,
    ADVERTISING,
    CONNECTED,
    RECEIVING_TRAINING_DATA,
    SYNCED,
    TRAINING_MODEL,
    WAITING_FOR_PARAMETERS,
} app_state_t;

typedef struct {
    app_state_t current_state;
    app_state_t previous_state;
} app_fsm_t;

void app_set_new_state(app_state_t new_state);

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
void app_init(void);

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
void app_process_action(void);

#endif /* APP */


