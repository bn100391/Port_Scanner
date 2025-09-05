#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 


int main(int argc, char *argv[])
{
    if(argc < 3){
        fprintf(stderr, "Ussage: [HOSTNAME] [START_PORT] [END_PORT]");
        exit(1); 
    }
	int sockfd = -1;
	int status;
	ssize_t size;
	int exitcode = EXIT_FAILURE;
	struct addrinfo hints = { .ai_socktype = SOCK_DGRAM };
	struct addrinfo *servinfo = NULL, *p;
	char buf[BUFSIZ];


    int currHostInt_init = atoi(argv[2]); 
    int maxHostValue = atoi(argv[3]); 
    
for(int currHostInt = currHostInt_init; currHostInt < maxHostValue; currHostInt++){ 
	
    /* Lookup host. */
    char host[100];   
    char *currHost = host;      

    int check = sprintf(currHost, "%d", currHostInt);
    if(check < 0){
            fprintf(stderr, "Error sprintf"); 
            exit(1); 
    }

	status = getaddrinfo(argv[1], currHost, &hints, &servinfo);
	if (0 != status) {
		fprintf(stderr, "error looking up towel.blinkenlights.nl: %s\n", gai_strerror(status));
		goto done;
	}

	/* Connect to host; first option that works wins. */
	for (p = servinfo; p != NULL; p = p->ai_next) {
		sockfd = socket(p->ai_family, p->ai_socktype, 0);
		if (-1 == sockfd) {
			continue;
		}

		if (-1 == connect(sockfd, p->ai_addr, p->ai_addrlen)) {
			close(sockfd);
			sockfd = -1;
			continue;
		}

		break;
	}

	if (-1 == sockfd) {
        continue; 
	}
    
    write(sockfd, "init_message\n", 13);
	/* Read all bytes from host, and write them to stdout. */
	for (;;) {
		size = recv(sockfd, buf, BUFSIZ - 1, 0);
        char *endCheck = strchr(buf, '\n'); 
        if (size > 0) {
			write(STDOUT_FILENO, buf, size);
            exit(0); 
		} else if (size < 0) {
            goto ignore; 
		} else if (endCheck != NULL) {
            goto done; 
            break;
		} 
	}
ignore: 
}

	exitcode = EXIT_SUCCESS;


done:
	if (-1 != sockfd) {
		close(sockfd);
	}

	if (NULL != servinfo) {
		freeaddrinfo(servinfo);
	}

	exit(exitcode);
}
