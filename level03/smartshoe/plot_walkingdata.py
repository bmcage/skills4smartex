#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Wed Sep 11 16:15:17 2019

@author: benny

process the serial data of the smartshoe
"""

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
plt.title('Pressure ADC data left shoe during walking')
#plt.xlim(20, 26)
plt.xlim(45, 52)
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
            distance += [row[3].replace(',','.')]
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
    distance = np.array(np.array(timesec, float), float)

    # offset time with start time
    timesec = timesec - skipval

    return timesec, ADC_pressure, np.array(timesec, float)

rtoewalk_data = do_read_right(RFILENAME1, skipval=toewalk_data[2])
rwalk_data = do_read_right(RFILENAME2, skipval=walk_data[2])
rirregwalk_data = do_read_right(RFILENAME3, skipval=irregwalk_data[2])


plt.figure()
plt.plot(rwalk_data[0], rwalk_data[1], 'b-', label='walk')
plt.plot(rtoewalk_data[0], rtoewalk_data[1], 'r-', label='toewalk')
plt.plot(rirregwalk_data[0], rirregwalk_data[1], 'g-', label='irregular walk')
plt.title('Pressure ADC data right shoe during walking')
#plt.xlim(20, 26)
plt.xlim(45, 52)
plt.legend()


plt.show()