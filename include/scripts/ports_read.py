import sys
import os
import glob
import serial

if sys.platform.startswith('win'):
    ports = ['COM%s' % (i + 1) for i in range(256)]
elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
    # this excludes your current terminal "/dev/tty"
    ports = glob.glob('/dev/tty[A-Za-z]*')
elif sys.platform.startswith('darwin'):
    ports = glob.glob('/dev/tty.*')
else:
    raise EnvironmentError('Unsupported platform')

result = []
for port in ports:
    try:
        s = serial.Serial(port)
        s.close()
        result.append(port)
    except (OSError, serial.SerialException):
        pass

# Get absolute path
absolute_path = os.path.abspath("")

# Get share path
share_path = os.path.join(absolute_path, "share")

# Define share file
ports_file = os.path.join(share_path, "available_ports.txt")

textfile = open(ports_file, "w")
for element in result:
    textfile.write(element + "\n")
textfile.close()

    