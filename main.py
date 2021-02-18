import time
import serial
import threading


class SMA:
    def __init__(self, window):
        self.window = window
        self.values = []

    def append(self, value):
        if len(self.values) > self.window - 1: self.values.pop(0)
        self.values.append(value)

    def calculate(self):
        return sum(self.values)/self.window

    def __str__(self):
        return self.calculate()

def parse_serial_line(line):
    """
    Not currently used. Kept for reference
    :param line:
    :return:
    """

    sensors = {}

    for s in line.split(";"):

        x = s.split(":")
        try:
            sensors[x[0]] = int(x[1])
        except:
            pass
            # print('?', x)

    return sensors

def read_sensor_package(bytes_serial):
    """
    Read a sensor from serial bytes. Expected format is 5 bytes:
        1, 2 : Package start 0x59 for YY
        3 : unsigned integer for sensor index
        4, 5 : unsigned integer for reading
    :return:
        sensor_index, reading
    """

    if bytes_serial[0] == 0x59 and bytes_serial[1] == 0x59:  # check for 'YY'
        # print(bytes_serial)
        sensor_index = bytes_serial[2]  # sensor index
        reading = bytes_serial[4] + bytes_serial[3] * 256  # 2 bytes for reading
        return sensor_index, reading
    else:
        return -1, None


def read_serial(serial, sensors):
    """
    TODO: Upgrade to a class
    :param serial:
    :param sensors:
    :return:
    """

    while True:

        # Read strings line by line
        # ser_bytes = ser.readline()
        # line = ser_bytes[0:len(ser_bytes) - 2].decode("ascii")
        # sensors = parse_serial_line(line)
        # print(f"\r {sensors}", end="")
        # ser.flushInput()

        # Read by bytes
        counter = serial.in_waiting  # count the number of bytes of the serial port
        bytes_to_read = 5
        if counter > bytes_to_read - 1:
            bytes_serial = serial.read(bytes_to_read)
            # ser.reset_input_buffer()  # reset buffer

            sensor_index, sensor_reading = read_sensor_package(bytes_serial)

            if sensor_index >= 0:
                if sensor_index not in sensors:
                    sensors[sensor_index] = SMA(2)
                if sensor_reading > 0:
                    sensors[sensor_index].append(sensor_reading)



if __name__ == "__main__":

    _serial = serial.Serial('/dev/cu.usbserial-1430', 115200, timeout=1)
    _serial.flushInput()

    sensors = {}

    serial_thread = threading.Thread(target=read_serial, args=(_serial, sensors), daemon=True)
    serial_thread.start()

    while True:

        for k, v in sensors.items():
            print(f"{k}:{v.calculate()} ", end="")
        print()

        time.sleep(0.1)
