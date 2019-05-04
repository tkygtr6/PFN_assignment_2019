#!/bin/bash

cd ../server
DATA_DIR=../test/data_test1 FLASK_APP=server.py flask run 1>/dev/null 2>/dev/null &
cd ../worker
#make clean
#make
sleep 2

diff <(MAXTIME=20 CAPACITY=-1 PRIORITY_RANGE=1 ./worker | awk '{print $2}') <(cat ../test/ans/test1 | awk '{print $2}') | wc -l

diff <(MAXTIME=20 CAPACITY=10 PRIORITY_RANGE=1 ./worker | awk '{print $2}') <(cat ../test/ans/test2 | awk '{print $2}') | wc -l

diff <(MAXTIME=20 CAPACITY=10 PRIORITY_RANGE=2 ./worker | awk '{print $2}') <(cat ../test/ans/test3 | awk '{print $2}') | wc -l

kill $(ps -a | grep "flask run" | grep -v grep | awk '{print $1}')
