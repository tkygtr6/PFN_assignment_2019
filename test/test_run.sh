#!/bin/bash

cd ../server
DATA_DIR=../test/data_test1 FLASK_APP=server.py flask run 1>/dev/null 2>/dev/null &
cd ../worker
#make clean
make
sleep 2

echo "test1"
diff <(MAXTIME=20 CAPACITY=-1 PRIORITY_RANGE=1 ./worker | awk '{print $2}') <(cat ../test/ans/test1 | awk '{print $2}') 

echo "test2"
diff <(MAXTIME=20 CAPACITY=10 PRIORITY_RANGE=1 ./worker | awk '{print $2}') <(cat ../test/ans/test2 | awk '{print $2}')

echo "test3"
diff <(MAXTIME=20 CAPACITY=10 PRIORITY_RANGE=2 NAIVE=1 ./worker | awk '{print $2}') <(cat ../test/ans/test3 | awk '{print $2}') 

echo "test4"
diff <(MAXTIME=20 CAPACITY=10 PRIORITY_RANGE=2 ./worker | awk '{print $2}') <(cat ../test/ans/test3 | awk '{print $2}')

kill $(ps -a | grep "flask run" | grep -v grep | awk '{print $1}')
