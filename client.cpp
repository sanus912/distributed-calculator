#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void readfromsocket(int newsockfd, char * buffer)
{
    bzero(buffer,256);
    int n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    std::cout << buffer << std::endl;
    std::cout << std::endl;
}

void writetosocket(int newsockfd, const char * s)
{
    int n = write(newsockfd,s,strlen(s));
    if (n < 0) error("ERROR writing to socket");
}

int main(int argc, char *argv[])
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(1);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    /*--for test--*/
    //freopen("test_in.txt","r",stdin); 
    //freopen("test_out.txt","w",stdout);
    /*--for test end--*/
   
    char ending[] = "CLOSED";
    while(1)
    {

        std::cout << "Please enter an operator and two operands: (Enter 'OK CLOSE' to exit.)" << std::endl;
        std::cout << "Input format: [PLUS|MINUS|TIMES|DIVIDE] operand1 operand2 (e.g. PLUS 1 3)" << std::endl;
        std::cin.getline(buffer,256);
        if(!*buffer) 
        {
            std::cout << "Empty input! Please try again." << std::endl;
            std::cout << std::endl;
            continue;  
        }

        writetosocket(sockfd, buffer);

        readfromsocket(sockfd, buffer);

        if(strncmp(buffer,ending,6)==0) 
        {
            break;
        }

    }

    close(sockfd);

    /*--for test--*/
    //fclose(stdin);
    //fclose(stdout);
    /*--for test end--*/
    
    return 0;
}
