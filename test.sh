#!/bin/bash

#!/bin/bash

SERVER=server
CLIENT=client
if test -x "$SERVER" && test -x "$CLIENT"; then
    echo "Запускаю сервер $SERVER и клиенты $CLIENT\n"

    screen -S "server" -d -m
    sleep 3
    screen -S server -p 0 -X stuff 'script server.txt'$(echo -ne '\015')
    sleep 3
    # screen -S server -p 0 -X stuff 'echo Hello!'$(echo -ne '\015')
    sleep 3
    screen -S server -p 0 -X stuff './server'$(echo -ne '\015')

    sleep 3

    # screen -S "client_1" -d -m
    # screen -S client_1 -p 0 -X stuff 'script client_1.txt'$(echo -ne '\015')
    # screen -S client_1 -p 0 -X stuff './client'$(echo -ne '\015')

    # sleep 2

    # screen -S "client_2" -d -m
    # sleep 0.5
    # screen -S client_2 -p 0 -X stuff 'script client_2.txt'$(echo -ne '\015')
    # screen -S client_2 -p 0 -X stuff './client'$(echo -ne '\015')

    # sleep 2

    # screen -S client_1 -p 0 -X stuff 'echo Hello!'$(echo -ne '\015')

    # screen -S client_1 -p 0 -X stuff 'exit'$(echo -ne '\015')
    # screen -S client_2 -p 0 -X stuff 'exit'$(echo -ne '\015')
    screen -S server -p 0 -X stuff 'exit'$(echo -ne '\015')
    sleep 1
    # echo "here\n"
    # screen -ls
    screen -X -S server kill
    # screen -X -S client_1 kill
    # screen -X -S client_2 kill

else
    echo "$SERVER или $CLIENT не существует\n"
fi
