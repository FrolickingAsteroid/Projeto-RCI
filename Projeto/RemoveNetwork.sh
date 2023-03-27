#!/bin/bash

start_port=55000
end_port=55099
net=064
server="193.136.138.142"
port=59000

for ((id = 0; id <= (end_port - start_port); id++)); do
	echo -n "UNREG $net $(printf "%02d" $id)" >/dev/udp/$server/$port
done
