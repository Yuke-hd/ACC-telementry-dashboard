from typing import Final
from pyaccsharedmemory import accSharedMemory
import time
import struct
import serial

REFRESH_INTERVAL: Final[int] = 50 # in ms
COM_PORT: Final[str] = "COM10"
BAUD_RATE: Final[int] = 115200


def fill_leftmost_bits(x: int):
    if x <= 0:
        return 0
    # Create a mask with x leftmost bits set to 1
    mask = (1 << x) - 1

    # Shift the mask to the left by the number of bits in the integer
    mask <<= 16 - x

    # Apply the mask to the value
    result = 0x0000 | mask

    return result


def main():
    try:
        arduinoSerial = serial.Serial(COM_PORT, BAUD_RATE)
    except serial.SerialException:
        print("unable to open port {}".format(COM_PORT))
        return

    asm = accSharedMemory()

    last_ms = time.time() * 1000.0

    try:
        while True:            
            ms = time.time() * 1000.0

            if ms - last_ms > REFRESH_INTERVAL:
                sm = asm.read_shared_memory()
                if sm is None:
                    continue

                rpm = float(sm.Physics.rpm)
                gear = float(fill_leftmost_bits(sm.Physics.gear - 1))
                spd = float(sm.Physics.speed_kmh)
                max_rpm = float(sm.Static.max_rpm)

                if arduinoSerial.writable():
                    arduinoSerial.write(struct.pack("ffff", rpm, gear, spd, max_rpm))
                
                last_ms = ms

    except KeyboardInterrupt:
        asm.close()
    except serial.SerialException:
        print("unable to send data to port {}, is the hardward connected?".format(COM_PORT))
        return


if __name__ == "__main__":
    main()
