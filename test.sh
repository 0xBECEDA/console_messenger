#!/bin/sh

SERVER=server
CLIENT=client
if test -x "$SERVER" && test -x "$CLIENT"; then
    echo "Запускаю сервер $SERVER и клиенты $CLIENT\n"

    ./"$SERVER" &
    export PID1=$!
    sleep 3
    ./"$CLIENT"    &
    export PID2=$!
    sleep 3
    ./"$CLIENT"    &
    export PID3=$!
    sleep 3
    echo "\n файловые дескрипкторы $PID1 $PID2 $PID3\n"

    fg %1
    bg %1

    kill "$PID1"
    kill "$PID2"
    kill "$PID3"

else
    echo "$SERVER или $CLIENT не существует\n"
fi
