# test04 code prints to serial the following:
#    print (sec, accel, peaks[current_peak-1], peaks_sec[current_peak-1],
#           valleys[current_valley-1], valleys_sec[current_valley-1], amplitude_avg)
# we capture this here and save
import serial
import time
import csv

# Typing dmesg | tail will shows you which /dev/ node the micro:bit was assigned
shoeleft_port = '/dev/ttyUSB0'
shoeright_port = '/dev/ttyUSB1'
accel_port = '/dev/ttyACM0'

# Filename to save to
filename = "walking_data_shoeright.csv"

import os.path
if os.path.isfile(filename):
    print("File {0} exists. Rename!".format(filename))
    raise Exception("File {0} exists. Rename!".format(filename))


# shoeleft and shoeright are assumed to emit data at same speeds
#ser_shoeleft = serial.Serial(port=shoeleft_port,
#                         baudrate=57600,
#                         bytesize=serial.EIGHTBITS,
#                         parity=serial.PARITY_NONE,
#                         stopbits=serial.STOPBITS_ONE,
#                         timeout=1)

ser_shoeright = serial.Serial(port=shoeright_port,
                         baudrate=57600,
                         bytesize=serial.EIGHTBITS,
                         parity=serial.PARITY_NONE,
                         stopbits=serial.STOPBITS_ONE,
                         timeout=1)

#ser_accel = serial.Serial(port=accel_port,
#                         baudrate=115200,
#                         bytesize=serial.EIGHTBITS,
#                         parity=serial.PARITY_NONE,
#                         stopbits=serial.STOPBITS_ONE,
#                         timeout=1)

eol = b'\r'
leneol = len(eol)

def readline(ser):
    
    line = bytearray()
    while True:
        c = ser.read(1)
        if c:
            line += c
            if line[-leneol:] == eol:
                break
        else:
            break
    return bytes(line)

#ser_shoeleft.flushInput()
ser_shoeright.flushInput()
#ser_accel.flushInput()

while True:
    try:
#        ser_shoeleft_bytes = readline(ser_shoeleft)[2:-2].strip()
        ser_shoeright_bytes = readline(ser_shoeright)[2:-2].strip()
#        ser_shoeaccel_bytes = readline(ser_accel)[2:-2].strip()
        
#        ser_shoeaccel_bytes = ser_shoeaccel_bytes.decode("utf-8").split(',')
#        ser_shoeleft_bytes = ser_shoeleft_bytes.decode("utf-8").split(',')[:-1]
        #print (ser_shoeright_bytes.decode("utf-8").split(','))
        ser_shoeright_bytes = ser_shoeright_bytes.decode("utf-8").split(',')[:-1]
        if ( len(ser_shoeright_bytes) == 4 ):
            decoded_bytes = [float(x) for x in ser_shoeright_bytes]
            #print (len(decoded_bytes))
            if decoded_bytes[0] != 0:
                decoded_bytes.insert(0, time.time())
                #print(ser_bytes, decoded_bytes)
                with open(filename,"a") as f:
                    writer = csv.writer(f,delimiter=",")
                    writer.writerow(decoded_bytes)
    except ValueError:
        print("Cannot convert to floats", ser_shoeright)
    except:
        print("Keyboard Interrupt")
        break
