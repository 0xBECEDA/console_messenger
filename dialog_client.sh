#!/bin/bash

echo "Begin\n"
screen -S "try" -d -m -L

sleep 1

# screen -S try -X stuff 'script log.txt'$(echo -ne '\015')

screen -S try -X stuff 'script log.txt\n'

sleep 1

# screen -S try -X stuff 'echo Hello!'$(echo -ne '\015')
screen -S try -X stuff 'echo Hello!\n'
sleep 1

# screen -S try -X stuff 'exit'$(echo -ne '\015')
screen -S try -X stuff 'exit\n'
sleep 1

screen -X -S try kill

echo "end\n"
