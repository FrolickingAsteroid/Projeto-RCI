#!/bin/bash

IP=127.0.0.1
start_port=55000
end_port=55099
net=064

# Compile the program
if ! make --silent; then
	echo "Failed to compile the program."
	exit 1
fi

tmux new-session -d -s TestingSesh

for ((port = start_port, id = 0; port <= end_port; port++, id++)); do
	tmux new-window -t TestingSesh:$(($id + 1)) -n "NODE$id"
	tmux send-keys -t TestingSesh:$(($id + 1)) "valgrind --leak-check=full --show-leak-kinds=all -s ./cot $IP $port" Enter
	# Input (testar também com "djoin $net $(printf "%02d" $id) 00 $IP $start_port")
	tmux send-keys -t TestingSesh:$(($id + 1)) "join $net $(printf "%02d" $id)" Enter

	sleep 0.5
done

for ((port = start_port, id = 0; port <= end_port; port++, id++)); do
	tmux send-keys -t TestingSesh:$(($id + 1)) "st" Enter
done

tmux attach-session -t TestingSesh