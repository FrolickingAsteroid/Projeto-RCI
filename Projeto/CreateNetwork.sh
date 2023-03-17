#!/bin/bash

IP=127.0.0.1
start_port=55000
end_port=55099
net=777

# Compile the program
if ! make --silent; then
	echo "Failed to compile the program."
	exit 1
fi

tmux new-session -d -s test_session

for ((port = start_port, id = 0; port <= end_port; port++, id++)); do
	tmux new-window -t test_session:$(($id + 1)) -n "NODE$id"
	tmux send-keys -t test_session:$(($id + 1)) "./cot $IP $port" Enter
	# Input (testar também com "djoin $net $(printf "%02d" $id) 00 $IP $start_port")
	tmux send-keys -t test_session:$(($id + 1)) "djoin $net $(printf "%02d" $id) 00 $IP $start_port" Enter

	sleep 0.5
done

for ((port = start_port, id = 0; port <= end_port; port++, id++)); do
	tmux send-keys -t test_session:$(($id + 1)) "st" Enter
done

tmux attach-session -t test_session
