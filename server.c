#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <linux/unistd.h>

/* структура и массив клиентов */
struct connection {
    int thread;
    int connection;
    char buf[1024];

} clients[2];

/* фнукиця очистки буфера */
char* clear_buf (char* buf, int buf_len ) {
    for (int i = 0; i < buf_len; i++) {
        buf[i] = 0;
    }
    return buf;
}

void test ( int cnt ) {
    printf("\n| %10s | %10s | %10s |\n", "thread", "connection", "buf");
    for ( int i = 0; i <= cnt; i++ ) {
        printf("| %10X | %10d | %10s |\n",
               clients[i].thread, clients[i].connection, clients[i].buf);
    }
    fflush(stdout);
}

/* функция потока */
/* принимает идентификатор потока */
void* thread_func( void* param ) {
    int idx = *(int*)param;

    /* дентификатор почему-то всегда на 1 больше, чем был передан в фнукицю */
    /* данный костыль исправляет ситуацию */
    idx = idx - 1;

    test(2);
    while (1) {
        sleep(3);
        /* читаем из соединения */
        int bytes_read = recv(clients[idx].connection,
                              clients[idx].buf, 1024, 0);

        if (bytes_read > 0) {
            int descriptor = clients[idx].connection;
            char* buf_pointer = clients[idx].buf;

            /* ищем все структуры, чьи дескрипторы соединений */
            /* не совпадают с текущим */
            for (int j = 0; j <= 1; j++) {

                /* Условие: дескриптор отличный от нашего, */
                /* дескриптор не нулевой, поток существует. */
                if (clients[j].connection != descriptor &&
                    clients[j].connection != 0 && clients[j].thread != 0) {
                    send(clients[j].connection, buf_pointer, bytes_read, 0);
                }
            }
        }
    }
}


void main() {

    printf("SERVER: begin work\n");
    fflush(stdout);
    struct sockaddr_in serv_in, clnt_in;
    int conn_idx = 0;
    void* p = (void*)clients;

    /* объявление и инициализация слушающего серверного сокета */
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if ( listener < 0 ) {
        perror("invalid socket()");
        exit(1);
    }

    /* инициализация struct sockaddr_in */
    serv_in.sin_family = AF_INET;
    serv_in.sin_port = htons(3425);
    serv_in.sin_addr.s_addr = htonl(INADDR_ANY); /* inet_addr("127.0.0.1") */

    /* привязываем дескриптор сокета к адресу */
    /* параметры: дескриптор слушающего сокета,
       указатель на структуру сервера, размер структуры */
    if ( bind( listener, (struct sockaddr *)&serv_in, sizeof(serv_in) ) < 0 ) {
        perror("bind() failed");
        exit(2);
    }
    /* слушаем запросы,
       передаем дескриптор сокета и размер очереди ожидания */
    listen( listener, 1 );

    while(1) {
        /* возвращаем дескриптор соединения с конкретным сокетом */
        int c = sizeof( struct sockaddr_in );
        int sock = accept( listener, (struct sockaddr *)&clnt_in, (socklen_t *)&c );

        /* если соединение установлено */
        if( sock > 0 ) {

            /* сообщение о успешном подключении */
            printf("%d sock to connection[%d]\n", sock, conn_idx);
            fflush(stdout);

            /* записываем дескриптор соединения в структуру */
            clients[conn_idx].connection = sock;

            /* создаем переменную для идентификатора потока */
            pthread_t thread;

            /* создаем поток с помощью pthread_create, которая получает: */
            /* - указатель на переменную потока, чтобы вернуть дескриптор потока */
            /* - атрибуты потока (по умолчанию: NULL) */
            /* - функция потока */
            /* - аргумент, передаваемый в функцию потока */
            pthread_create(&thread, NULL, thread_func,
                           (void *)&conn_idx);

            /* записываем идентификатор потока в структуру */
            clients[conn_idx].thread = thread;
            conn_idx++;

        } else {
            perror("accept");
            exit(3);
            close(sock);
        }
    }
}
