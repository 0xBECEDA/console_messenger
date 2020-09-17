#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

/* фyнкиця очистки буфера */
char* clear_buf (char* buf, int buf_len ) {
    for (int i = 0; i < buf_len; i++) {
        buf[i] = 0;
    }
    return buf;
}

void  main() {
    int fd = 0; // stdin
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    char buf[1024];
    clear_buf((char*)&buf, 1024);

    /* объявление сокета */
    int sock;
    struct sockaddr_in addr;

    /* инициализация сокета */
    /* сетевой, стриминговый, протокол по умолчанию */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /* если возвращаемый дескриптор < 0, печатаем сообщение об ошибке */
    if ( sock < 0 ) {
        perror("socket");
        exit(1);
    }

    fcntl(sock, F_SETFL, O_NONBLOCK);

    /* инициализация struct sockaddr_in */
    /* семейство адресов AF_INET */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425); // присваиваем порт
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1


    /* попытка соединения */
    int r = -1;
    while( r < 0) {

        /* передаем дескриптор сокета, указатель на структуру
           с адресом сервера, длинну структуры */
        r = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
        if (r < 0) {
            perror("connect");
        }
    }

    /* отправка */
    while (1) {

        int len = -1;
        sleep(3);
        len = read(fd, buf, sizeof(buf));

        /* передаем дескриптор сокета, указатель на буфер с данными
           длинну буфера в байтах, флаги */
        if (len > 0) {

            /* send возвращает кол-во отправленных байтов */
            printf("I:   %s\n", buf);
            fflush(stdout);

            int amount = send(sock, buf, sizeof(buf), 0);
            clear_buf((char*)&buf, 1024);
        }
        /* чтение ответа */
        int bytes_read =  recv(sock, buf, sizeof(buf), 0);

        if (bytes_read > 0) {
            printf("Client #2:   %s\n", buf);
            fflush(stdout);
            clear_buf((char*)&buf, 1024);
        }
    }

    /* закрываем сокет */
    close(sock);
}
