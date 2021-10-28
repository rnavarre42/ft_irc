#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFFERSIZE  4092

void irc_send(int sock, char *data)
{
    printf("client> %s\n", data);
    send(sock, data, strlen(data), 0);
    send(sock, "\r\n", 2, 0);
}

void send_ident(int server_fd, char *nick, char *ident)
{
    char *data;

    asprintf(&data, "USER %s %s * * *", ident, ident);
    irc_send(server_fd, data);
    free(data);
    asprintf(&data, "NICK %s", nick);
    irc_send(server_fd, data);
    free(data);
}

void *console(void *arg)
{
	// 464 + 21 + 25 + 2
    int sock;
    char *line;
	char *fck = "\
JOIN #micanaloculto\r\n\
MODE #micanaloculto +t\r\n\
MODE #micanaloculto +nt\r\n\
PRIVMSG non :wero iwepori weporiwe poriweop riqeropiq iwopieruqw eroiuqweopir uwerpoi uwqeropi uwerpoi uwqerpoiq wuerpoiwqeur poiwqeiur qwpoeiru weopiru qwepoir uqwepori uqwqeproi uqwerpoi uwerpoi uwerpo iwuer poiiqwuer poiqwieur poiqwieur poiweur poiweur poiweqru opiwequr poiqweur opiqweur poiewru poqiweru powqieru wqeopiru qwpoieru qwpoeiri uwepoiri 23949234 9234923 892348 9234892 34894 823948929384 92384 9283498 34 234998 2398923 892384 98";
	char *fck2 = "\
weroiweoriweoriwe 123456 234567 345678 456789 567890 678901 789012 890123 901234 012345 a1234 b1234 c1234 d1234 e1234 f1234 g1234 h1235\r\nPRIVMSG non :Hola\r\nPRIVMSG non :wero iwepori weporiwe poriweop riqeropiq iwopieruqw eroiuqweopir uwerpoi uwqeropi uwerpoi uwqerpoiq wuerpoiwqeur poiwqeiur qwpoeiru weopiru qwepoir uqwepori uqwqeproi uqwerpoi uwerpoi uwerpo iwuer poiiqwuer poiqwieur poiqwieur poiweur poiweur poiweqru opiwequr poiqweur opiqweur poiewru poqiweru powqieru wqeopiru qwpoieru qwpoeiri uwepoiri 23949234 9234923 892348 9234892 34894 823948929384 92384 9283498 34 234998 2398923 892384 98 | a1234567890 b1234567890 c1234567890\r\n";
    sock = *(int *)arg;
    while (1)
    {
        line = readline("");
        add_history(line);
		if (!strcmp(line, "FCK"))
		{
			send(sock, fck, strlen(fck), 0);
			send(sock, fck2, strlen(fck2), 0);
		}
		else
		{
        	send(sock, line, strlen(line), 0);
	        send(sock, "\r\n", 2, 0);
		}
	    free(line);
    }
}

void	loop_client(int server_fd)
{
	char	buffer[BUFFERSIZE + 1];
	char	*line;
	char	*response;
	int		ret;

    while (1)
    {
		ret = read(server_fd , buffer, BUFFERSIZE);
		buffer[ret] = 0;
		line = strtok(buffer, "\n");
		while (line)
		{
			printf("server> %s\n", line);
			if (!strncmp(line, "PING", 4))
			{
				asprintf(&response, "PONG %s", line + 5);
				irc_send(server_fd, response);
				free(response);
			}
			else if (!strncmp(line, "ERROR", 5))
			{
				close(server_fd);
				return ;
			}
			line = strtok(NULL, "\n");
		}
    }
}

int main(int argc, char **argv)
{
    int server_fd = 0;
    struct sockaddr_in serv_addr;
    pthread_t   console_thread;

    char *nick, *username, *server_host;
    int server_port;

    if (argc != 5)
    {
        printf("Use: %s nick username host port\n", *argv);
        return -1;
    }

    nick = argv[1];
    username = argv[2];
    server_host = argv[3];
    server_port = atoi(argv[4]);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);
    
    if(inet_pton(AF_INET, server_host, &serv_addr.sin_addr)<=0)
    {
        printf("Invalid address/ Address not supported \n");
        return -1;
    }
    if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send_ident(server_fd, nick, username);
    pthread_create(&console_thread, NULL, console, &server_fd);
	loop_client(server_fd);
	return (0);
}
