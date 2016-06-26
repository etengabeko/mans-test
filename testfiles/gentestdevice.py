#!/usr/bin/env python3

import datetime
import os
import random

epoch = datetime.datetime.utcfromtimestamp(0)

def unix_time_msec(dt):
    return (dt - epoch).total_seconds() * 1000.0

def get_device_name():
    counter = 1
    default_name = "device"
    device_name = default_name + str(counter)
    while os.path.isdir(device_name) and os.path.exists(device_name):
        device_name = default_name + str(counter)
        counter += 1
    return device_name

def get_random_ports_count(max_ports_count = 8):
    return random.randint(1, max_ports_count)

def get_port_number(max_port_number = 32767):
    return random.randint(1, max_port_number)

def get_random_entry_count(max_entry_count = 20):
    return random.randint(2, max_entry_count)

def get_random_bytes(max_bytes = 1000000):
    return random.randint(0, max_bytes)

def get_random_date(days_before = 7):
    return datetime.datetime.now() - datetime.timedelta(days = random.randint(1, days_before))

def get_random_period_msec(max_period_msec = 60*1000):
    return random.randint(0, max_period_msec)

def get_random_microsec_str():
    mcs = str(random.randint(0, 999))
    while len(mcs) < 3:
        mcs = "0" + mcs
    return mcs

def create_port(device_name, port_number):
    create_port_file(device_name, port_number, ".tx")
    create_port_file(device_name, port_number, ".rx")

def create_port_file(device_name, port_number, ext):
    fname = device_name + "/" + str(port_number) + ext
    file = open(fname, 'w')
    dt = get_random_date()
    for i in range(0, get_random_entry_count()):
        dt = dt + datetime.timedelta(milliseconds = get_random_period_msec())
        bts = get_random_bytes()
        file.write(dt.strftime("%s") + get_random_microsec_str() + " " + str(bts) + "\n")

if __name__ == "__main__":
    devname = get_device_name()
    dirname = "./" + devname
    os.mkdir(dirname)
    for i in range(0, get_random_ports_count()):
        create_port(dirname, get_port_number())
    print("Created " + devname)
