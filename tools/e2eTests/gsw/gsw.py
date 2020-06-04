#!/usr/bin/env python3

"""
Proof of concept:
Pseudo-GSW to perform a simple end-to-end test of a dockerized cFS
"""

from enum import IntEnum
import socket
import time
import sys

class Test_status(IntEnum):
    PASS = 0
    FAIL = 1

SAMPLE_APP_NOOP_PACKET = bytearray([
        0x18, 0x82, 0xC0, 0x00, 0x00,
        0x01, 0x00, 0x00
    ])

ENABLE_TELEMETRY_PACKET_TEMPLATE = bytearray([
        0x18, 0x80, 0xC0, 0x00, 0x00,
        0x12, 0x06, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00
    ])

CFS_PORT = 1234
GSW_PORT = 1235

try:
    # Host name shall match the cFS service name in docker-compose.yml
    CFS_IP = socket.gethostbyname('cfs')
except:
    print('Error: cFS IP is unknown')
    exit(1)

try:
    # Host name shall match the GSW service name in docker-compose.yml
    GSW_IP = socket.gethostbyname('gsw') 
except:
    print('Error: GSW IP is unknown')
    exit(1)


def craft_telemetry_packet() -> bytearray:
    """
    Craft telemetry packet from enable telemetry packet template
    """

    packet = ENABLE_TELEMETRY_PACKET_TEMPLATE

    i = 8 # IP starts at index 8
    for c in GSW_IP:
       packet[i] = int(hex(ord(c)), 16)
       i += 1

    return packet

def enable_telemetry() -> bool:
    """
    Try to enable the telemetry
    """

    print('Enable telemetry (cFS IP: ' + CFS_IP + ', GSW IP: ' + GSW_IP + ')')

    testSocket.sendto(craft_telemetry_packet(), (CFS_IP, CFS_PORT))

    if telemetry_contains("telemetry output enabled for IP {}".format(GSW_IP), 10):
        return True 
        
    return False

def telemetry_contains(expected_event, max_attempts) -> bool:
    """
    Check whether the telemetry contains an expected event or not
    """

    print ('Listening to cFS telemetry (expected event: "' + expected_event + '")')    

    attempts = 0
    while attempts < max_attempts:
        try:
            datagram, _ = testSocket.recvfrom(4096)
            actual_event = ''.join(map(chr, datagram))
            #print(attempts, ': ', actual_event)
            if expected_event in actual_event:
                return True
        except:
            pass

        attempts += 1
        time.sleep(1)

    return False 

def assert_sample_app_noop():
    """
    Send a NOOP command (SAMPLE_APP) and ensure that it is acknowledged by cFS
    """
    
    print ('SAMPLE_APP: Sending NOOP command')
    testSocket.sendto(SAMPLE_APP_NOOP_PACKET, (CFS_IP, CFS_PORT))

    if telemetry_contains('SAMPLE: NOOP command', 10):
        return Test_status.PASS
    
    return Test_status.FAIL


#
# Main
#
if __name__ == "__main__":

    # Setup
    time.sleep(5) # Warming up
    testSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    testSocket.bind(('', GSW_PORT))
    testSocket.settimeout(.1)

    # Test - SAMPLE_APP NOOP command is acknowledged by cFS
    if enable_telemetry():
        test_status = assert_sample_app_noop()

    # Teardown
    testSocket.close()

    # Exit
    print('Test status: ', end = '')
    if test_status == Test_status.PASS:
        print('PASS')
        sys.exit(0)
    else:
        print('FAIL')
        sys.exit(1)