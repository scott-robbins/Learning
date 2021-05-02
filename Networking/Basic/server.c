/** gcc -pthread -o server server.c **/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#define BUFFER_SZ 2048
#define CLIENT_LIMIT 10

/* Client structure */
typedef struct {
    struct sockaddr_in addr; // remote address
    int connfd;              // connection file descriptor
    int uid;                 // client identifier
    char name[32];           // client name
} client_t;

client_t *clients[CLIENT_LIMIT];
static _Atomic unsigned int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Add client to queue */
void queue_add(client_t *cl){
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < CLIENT_LIMIT; ++i) {
        if (!clients[i]) {
            clients[i] = cl;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

/* Delete client from queue */
void queue_delete(int uid){
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < CLIENT_LIMIT; ++i) {
        if (clients[i]) {
            if (clients[i]->uid == uid) {
                clients[i] = NULL;
                break;
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

/* Print ip address */
void print_client_addr(struct sockaddr_in addr){
    printf("%d.%d.%d.%d",
        addr.sin_addr.s_addr & 0xff,
        (addr.sin_addr.s_addr & 0xff00) >> 8,
        (addr.sin_addr.s_addr & 0xff0000) >> 16,
        (addr.sin_addr.s_addr & 0xff000000) >> 24);
}

/* Handle all communication with the client */
void *handle_client(void *arg){
    char buff_out[BUFFER_SZ];
    char buff_in[BUFFER_SZ / 2];
    int rlen;

    client_count++;
    client_t *cli = (client_t *)arg;

    printf("<< accepted: ");
    print_client_addr(cli->addr);

    /* Close connection */
    close(cli->connfd);

    /* Delete client from queue and yield thread */
    queue_delete(cli->uid);
    print_client_addr(cli->addr);
    printf(" referenced by %d\n", cli->uid);
    free(cli);
    client_count--;
    pthread_detach(pthread_self());

    return NULL;
}

void usage()
{
	printf("[!!] Incorrect Usage\n");
	printf("$ server <port>\n");
}
int main(int argc, char const *argv[])
{
	// check args
	if(argc < 2){
		usage();
		exit(0);
	}

	int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    pthread_t tid;

    /* Socket settings */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    /* Ignore pipe signals */
    signal(SIGPIPE, SIG_IGN);

    /* Bind */
    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Socket binding failed");
        return EXIT_FAILURE;
    }

    /* Listen */
    if (listen(listenfd, 10) < 0) {
        perror("Socket listening failed");
        return EXIT_FAILURE;
    }

    printf("[*] Server Started\n");
    /* Accept clients */
    while (1) {
    	/* Handle Client */
        socklen_t clilen = sizeof(cli_addr);
        connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

        /* Client settings */
        client_t *cli = (client_t *)malloc(sizeof(client_t));
        cli->addr = cli_addr;
        cli->connfd = connfd;

        /* Add client to the queue and fork thread */
        queue_add(cli);
        pthread_create(&tid, NULL, &handle_client, (void*)cli);


        /* Reduce CPU usage */
        sleep(1);
    }

    printf("[*] Shutting Down Server\n");
	return EXIT_SUCCESS;
}