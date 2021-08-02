#!/bin/bash
sleep(15)
chmod +777 /dev/ttyS0
sleep(15)
cd /home/lbs/xuechong/ntrip_share
./ntrip_client_main /dev/ttyS0 115200
