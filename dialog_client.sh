#!/bin/bash

SERVER=server
CLIENT=client
DELAY=3
if test -x "$SERVER" && test -x "$CLIENT"; then
    screen -S "server" -d -m -L
    screen -S server -p 0 -X stuff './server\n'
    sleep $DELAY

    screen -S "client_1" -d -m
    sleep $DELAY

    screen -S client_1 -p 0 -X stuff './client  > client_1.txt \n'
    sleep $DELAY

    screen -S "client_2" -d -m
    sleep $DELAY

    screen -S client_2 -p 0 -X stuff './client  > client_2.txt\n'
    sleep 3

    # общение
    screen -S client_2 -p 0 -X stuff '"Hi!"\n'
    sleep $DELAY
    screen -S client_1 -p 0 -X stuff '"Hello!"\n'
    sleep $DELAY
    screen -S client_1 -p 0 -X stuff '"How are you?"\n'
    sleep $DELAY
    screen -S client_2 -p 0 -X stuff '" I am fine"\n'
    sleep $DELAY
    screen -S client_1 -p 0 -X stuff '"Buy :)"\n'
    sleep $DELAY

    screen -S client_1 -p 0 -X stuff 'exit\n'
    screen -S client_2 -p 0 -X stuff 'exit\n'
    screen -S server -p 0 -X stuff 'exit\n'

    screen -X -S server kill
    screen -X -S client_1 kill
    screen -X -S client_2 kill

else
    echo "$SERVER или $CLIENT не существует\n"
fi
