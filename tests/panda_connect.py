import time
from panda import Panda
panda = Panda()
panda.set_safety_mode(Panda.SAFETY_ALLOUTPUT)
# time.sleep(1000)

while True:
    time.sleep(0.01)
    health = panda.health()
    print(f"sbu1_voltage_mV: {health['sbu1_voltage_mV']}, sbu2_voltage_mV: {health['sbu2_voltage_mV']}")
    print(f"car_harness_status: {health['car_harness_status']}")
    print(f"ignition_line: {health['ignition_line']}")
    msgs = panda.can_recv()
    print(f"Received CAN messages: {msgs}")
    panda.can_send(0x11, b'x03', 1)

