#!/home/markus/anaconda3/envs/fl_host/bin/python
"""
Empty NCP-host Example Application.
"""

# Copyright 2021 Silicon Laboratories Inc. www.silabs.com
#
# SPDX-License-Identifier: Zlib
#
# The licensor of this software is Silicon Laboratories Inc.
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

import os.path
import sys
import logging
import threading
from time import sleep

# TODO: Add FSM
# TODO: Implement L2CAP

sys.path.append(os.path.join(os.path.dirname(__file__), "../.."))
from common.util import ArgumentParser, BluetoothApp, get_connector

# Characteristic values
GATTDB_DEVICE_NAME = b"FLC"
GATTDB_MANUFACTURER_NAME_STRING = b"Silicon Labs"

# FIXME: These should be attributes
SCAN_MODE = 0x00  # Passive mode
SCAN_INTERVAL = 800  # 500ms
SCAN_WINDOW = 800  # 500ms
SCAN_PHY = 0x01  # 1M Phy
SCAN_DISCOVERY_MODE = 0x02  # Observation mode  # FIXME: Why does this have to be obs mode?
LOG_LEVEL = logging.INFO
FEDERATED_LEARNING_PERIPHERAL_NAME = "FLP"
SCAN_DURATION_S = 5


class App(BluetoothApp):
    def __init__(self, connector):
        self.advertising_flps = []
        super().__init__(connector)
        
    """ Application derived from generic BluetoothApp. """
    def bt_evt_system_boot(self, evt):
        """ Bluetooth event callback

        This event indicates that the device has started and the radio is ready.
        Do not call any stack command before receiving this boot event!
        """
        self.adv_handle = None
        self.scan_start()
        self.log.setLevel(LOG_LEVEL)

    def bt_evt_connection_opened(self, evt):
        """ Bluetooth event callback """
        self.log.info(f"Connection opened to {evt.address}")

    def bt_evt_connection_closed(self, evt):
        """ Bluetooth event callback """
        self.log.info(f"Connection closed with reason {evt.reason:#x}: '{evt.reason}'")
        
    def bt_evt_scanner_legacy_advertisement_report(self, evt):
        if self.is_fl_peripheral(evt.data):
            address = evt.address
            if address not in self.advertising_flps:
                self.log.info(f"Received advertisement from FLP at {address}")
                self.advertising_flps.append(address)
        


    def scan_start(self):
        """ Start scanning. """
        self.lib.bt.scanner.set_parameters(SCAN_MODE, SCAN_INTERVAL,
                                           SCAN_WINDOW)
        self.lib.bt.scanner.start(SCAN_PHY, SCAN_DISCOVERY_MODE)
        self.log.info(f"Scanning started.")
        threading.Timer(SCAN_DURATION_S, self.stop_scanning_for_flp).start()
                                           
    def scan_stop(self):
        self.lib.bt.scanner.stop()
        self.log.info("Scanning stopped.")     
        
    def is_fl_peripheral(self, adv_data):
        self.log.debug(f"Raw adv data: {adv_data}")
        adv_data = list(adv_data)
        if device_name_from_adv(adv_data) == FEDERATED_LEARNING_PERIPHERAL_NAME:
            return True
        else:
            return False
    
    def stop_scanning_for_flp(self):
        self.log.info(f"Scan duration of {SCAN_DURATION_S} seconds has elapsed.")
        self.scan_stop()
        self.log.info(f"Discovered a total of {len(self.advertising_flps)} FLPs.")
        
def device_name_from_adv(adv_data):
    adv_data_len = len(adv_data)
    i = 0
    while i < adv_data_len:
        adv_type_len = adv_data[i]
        adv_type = adv_data[i+1]
        if adv_type == 0x09:
            device_name = adv_data[(i+2):(i+2+adv_type_len):]
            device_name = ''.join(map(chr, device_name))
            return device_name
        else:
            i = i + adv_type_len + 1
    
    return None
        

# Script entry point.
if __name__ == "__main__":
    parser = ArgumentParser(description=__doc__)
    args = parser.parse_args()
    connector = get_connector(args)
    # Instantiate the application.
    app = App(connector)
    # Running the application blocks execution until it terminates.
    # app.daemon = True
    app.start()
    while 1:
        sleep(2)
        print("hello")
