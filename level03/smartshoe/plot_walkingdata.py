#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Wed Sep 11 16:15:17 2019

@author: benny

process the serial data of the smartshoe
"""
WITH_TITLE = False

# Shoe LEFT data
import os
from os.path import expanduser
home = expanduser("~")

SKIP1 = 200
FILENAME1 = home + os.sep + 'git/skills4smartex/level03/smartshoe/walkingdata/walking_data_shoeleft_toewalk.csv'
SKIP2 = 200
FILENAME2 = home + os.sep + 'git/skills4smartex/level03/smartshoe/walkingdata/walking_data_shoeleft_normalwalk.csv'
SKIP3 = 570
FILENAME3 = home + os.sep + 'git/skills4smartex/level03/smartshoe/walkingdata/walking_data_shoeleft_irregwalk.csv'


# CSV structure is (time_sec_recording, ms, pressure ADC reading

import csv
import numpy as np
import matplotlib.pyplot as plt

def do_read_left(filename, skip=0):
    timesec = []
    ms = []
    ADC_pressure = []

    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            timesec += [row[0].replace(',','.')]
            ms += [row[1].replace(',','.')]
            ADC_pressure += [row[2].replace(',','.')]
            line_count += 1
            if line_count > 1:
                if abs(float(timesec[-1])-float(timesec[-2])) > 1 \
                    or float(timesec[-1]) == float(timesec[-2]):
                    # correct errors
                    timesec = timesec[:-1]
                    ms = ms[:-1]
                    ADC_pressure = ADC_pressure[:-1]
        print(f'Processed {line_count} lines.')

    #convert to numpy arrays
    timesec = np.array(timesec, float)
    ADC_pressure = np.array(ADC_pressure, float)

    # offset time with start time
    skipval = timesec[skip]
    timesec = timesec - skipval

    return timesec[skip:], ADC_pressure[skip:], skipval


# make rolling average of an array a
def rolling_window(a, window):
    shape = a.shape[:-1] + (a.shape[-1] - window + 1, window)
    strides = a.strides + (a.strides[-1],)
    return np.lib.stride_tricks.as_strided(a, shape=shape, strides=strides)

toewalk_data = do_read_left(FILENAME1, skip=SKIP1)
walk_data = do_read_left(FILENAME2, skip=SKIP2)
irregwalk_data = do_read_left(FILENAME3, skip=SKIP3)


plt.figure()
plt.plot(walk_data[0], walk_data[1], 'b-', label='walk')
plt.plot(toewalk_data[0], toewalk_data[1], 'r-', label='toewalk')
plt.plot(irregwalk_data[0], irregwalk_data[1], 'g-', label='irregular walk')
if WITH_TITLE:
    plt.title('Pressure ADC data left shoe during walking')
#plt.xlim(20, 26)
plt.xlim(45, 52)
plt.xlabel("time [s]")
plt.ylabel("ADC Pressure")
plt.legend()

RFILENAME1 = home + os.sep + 'git/skills4smartex/level03/smartshoe/walkingdata/walking_data_shoeright_toewalk.csv'
RFILENAME2 = home + os.sep + 'git/skills4smartex/level03/smartshoe/walkingdata/walking_data_shoeright_normalwalk.csv'
RFILENAME3 = home + os.sep + 'git/skills4smartex/level03/smartshoe/walkingdata/walking_data_shoeright_irregwalk.csv'


def do_read_right(filename, skipval=0):
    timesec = []
    ms = []
    ADC_pressure = []
    distance = []

    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            timesec += [row[0].replace(',','.')]
            ms += [row[1].replace(',','.')]
            ADC_pressure += [row[2].replace(',','.')]
            distance += [row[4].replace(',','.')]
            if distance[-1] == -1: 
                distance[-1] = distance[-2]
            line_count += 1
            if line_count > 1:
                if abs(float(timesec[-1])-float(timesec[-2])) > 1 \
                    or float(timesec[-1]) == float(timesec[-2]):
                    # correct errors
                    timesec = timesec[:-1]
                    ms = ms[:-1]
                    ADC_pressure = ADC_pressure[:-1]
                    distance = distance[:-1]
        print(f'Processed {line_count} lines.')

    #convert to numpy arrays
    timesec = np.array(timesec, float)
    ADC_pressure = np.array(ADC_pressure, float)
    distance = np.array(distance, float)

    # offset time with start time
    timesec = timesec - skipval

    return timesec, ADC_pressure, distance

rtoewalk_data = do_read_right(RFILENAME1, skipval=toewalk_data[2])
rwalk_data = do_read_right(RFILENAME2, skipval=walk_data[2])
rirregwalk_data = do_read_right(RFILENAME3, skipval=irregwalk_data[2])


plt.figure()
plt.plot(rwalk_data[0], rwalk_data[1], 'b-', label='walk')
plt.plot(rtoewalk_data[0], rtoewalk_data[1], 'r-', label='toewalk')
plt.plot(rirregwalk_data[0], rirregwalk_data[1], 'g-', label='irregular walk')
if WITH_TITLE:
    plt.title('Pressure ADC data right shoe during walking')
#plt.xlim(20, 26)
plt.xlim(45, 52)
plt.xlabel("time [s]")
plt.ylabel("ADC Pressure [-]")
plt.legend()

""" 
Now we plot right walk and distance sensor

"""

plt.figure()
plt.plot(rwalk_data[0], rwalk_data[1], 'b-', label='walk - P')
plt.plot(rwalk_data[0], rwalk_data[2], 'b:', label='walk - d')
plt.plot(rtoewalk_data[0], rtoewalk_data[1], 'r-', label='toewalk - P')
plt.plot(rtoewalk_data[0], rtoewalk_data[2], 'r:', label='toewalk - d')
if WITH_TITLE:
    plt.title('Pressure ADC data and distance data (mm) right shoe during walking')
#plt.xlim(20, 26)
plt.xlim(45, 49.5)
plt.ylim(0, 850)
plt.xlabel("time [s]")
plt.ylabel("ADC Pressure [-] / Distance [mm]")
plt.legend()


""" 
Now we plot right walk and acceleration hip data

"""

SKIP1 = 200
FILENAME1 = home + os.sep + 'git/skills4smartex/level03/smartshoe/walkingdata/with_hip_accel_data/walking_data_shoeleft_acctoewalk.csv'
SKIP2 = 200
FILENAME2 = home + os.sep + 'git/skills4smartex/level03/smartshoe/walkingdata/with_hip_accel_data/walking_data_shoeleft_accnormalwalk.csv'

RFILENAME1 = home + os.sep + 'git/skills4smartex/level03/smartshoe/walkingdata/with_hip_accel_data/walking_data_shoeright_acctoewalk.csv'
RFILENAME2 = home + os.sep + 'git/skills4smartex/level03/smartshoe/walkingdata/with_hip_accel_data/walking_data_shoeright_accnormalwalk.csv'

AFILENAME1 = home + os.sep + 'git/skills4smartex/level03/smartshoe/walkingdata/with_hip_accel_data/walking_data_accelhip_acctoewalk.csv'
AFILENAME2 = home + os.sep + 'git/skills4smartex/level03/smartshoe/walkingdata/with_hip_accel_data/walking_data_accelhip_accnormalwalk.csv'



toewalk_data = do_read_left(FILENAME1, skip=SKIP1)
walk_data = do_read_left(FILENAME2, skip=SKIP2)

rtoewalk_data = do_read_right(RFILENAME1, skipval=toewalk_data[2])
rwalk_data = do_read_right(RFILENAME2, skipval=walk_data[2])


def do_read_accel(filename, skipval=0):
    timesec_test = []
    timesec = []
    accel = []
    stepspermin = []
    amplitude_avg = []

    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            timesec += [row[0].replace(',','.')]
            timesec_test += [row[1].replace(',','.')]
            accel += [row[2].replace(',','.')]
            stepspermin += [row[4].replace(',','.')]
            amplitude_avg += [row[9].replace(',','.')]
            line_count += 1
            if line_count > 1:
                if abs(float(timesec_test[-1])-float(timesec_test[-2])) > 1 \
                    or float(timesec_test[-1]) == float(timesec_test[-2]):
                    timesec = timesec[:-1]
                    timesec_test = timesec_test[:-1]
                    accel = accel[:-1]
                    stepspermin = stepspermin[:-1]
                    amplitude_avg = amplitude_avg[:-1]
        print(f'Processed {line_count} lines.')

    #convert to numpy arrays
    timesec = np.array(timesec, float)
    accel = np.array(accel, float)
    stepspermin = np.array(stepspermin, float)
    #if CORRECT_WRONG_STEPSPERMIN:
    #    stepspermin = 60/stepspermin*60
    amplitude_avg = np.array(amplitude_avg, float)

    # offset time with start time
    print ('skip', skipval, timesec[0], timesec[0]-skipval)
    timesec = timesec - skipval

    return timesec, accel, stepspermin, amplitude_avg


atoewalk_data = do_read_accel(AFILENAME1, skipval=toewalk_data[2])
awalk_data = do_read_accel(AFILENAME2, skipval=walk_data[2])


plt.figure()
plt.plot(atoewalk_data[0], atoewalk_data[1], 'r-', label='Toewalk Accel')
plt.plot(rtoewalk_data[0], rtoewalk_data[1], 'r-.', label='Toewalk P')
plt.plot(awalk_data[0], awalk_data[1], 'b-', label='Walk Accel')
plt.plot(rwalk_data[0], rwalk_data[1], 'b-.', label='Walk P')
plt.xlim(45,48.5)
plt.ylim(0, 1800)
plt.xlabel("time [s]")
plt.ylabel("accel [10$^-3$ g] / ADC Pressure [-]")
plt.legend()
if WITH_TITLE:
    plt.title('Accel data')

plt.show()
