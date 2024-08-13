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
SCAN_DISCOVERY_MODE = 0x01  # Generic mode
LOG_LEVEL = logging.DEBUG


class App(BluetoothApp):
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
        self.is_fl_peripheral(evt.data)
        self.log.info("Received advertisement report.")


    def scan_start(self):
        """ Start scanning. """
        self.lib.bt.scanner.set_parameters(SCAN_MODE, SCAN_INTERVAL,
                                           SCAN_WINDOW)
        self.lib.bt.scanner.start(SCAN_PHY, SCAN_DISCOVERY_MODE)
        self.log.info("Scanning started.")
                                           
    def scan_stop(self):
        self.lib.bt.scanner.stop()
        self.log.info("Scanning stopped.")     
        
    def is_fl_peripheral(self, adv_data):
        adv_data = list(adv_data)
        self.log.debug(list(adv_data))
        
def device_name_from_adv(adv_data):
    pass

# Script entry point.
if __name__ == "__main__":
    parser = ArgumentParser(description=__doc__)
    args = parser.parse_args()
    connector = get_connector(args)
    # Instantiate the application.
    app = App(connector)
    # Running the application blocks execution until it terminates.
    app.run()
