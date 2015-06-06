#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <unordered_map>

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
    //std::cout << "Operation: " << buffer << std::endl;
}

void writetosocket(int newsockfd, const char * s)
{
    int n = write(newsockfd,s,strlen(s));
    if (n < 0) error("ERROR writing to socket");
}

int myadd(int i, int j)
{
    return i+j;
}

int mysub(int i, int j)
{
    return i-j;
}

int mymul(int i, int j)
{
    return i*j;
}

int mydiv(int i, int j)
{
    if(j==0) return -1;
    return i/j;
}


int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

     char ending[] = "OK CLOSE";

     typedef int(*Func)(int,int);
     std::unordered_map<std::string, Func> hs;
     hs["PLUS"] = myadd;
     hs["MINUS"] = mysub;
     hs["TIMES"] = mymul;
     hs["DIVIDE"] = mydiv;

     while(1)
     {

         readfromsocket(newsockfd, buffer); 
         
         if(strncmp(buffer,ending,8)==0) 
         {
             writetosocket(newsockfd, "CLOSED");
             break;
         }
         
         std::string op;
         int i,j;
         std::istringstream iss;
         iss.str(buffer);
         if(!(iss >> op) || hs.find(op) == hs.end())
         {
             writetosocket(newsockfd, "Invalid operator! Please try again.");
             continue;
         }

         if(!(iss >> i))
         {
             writetosocket(newsockfd, "Missing 1st Operand! Please try again.");
             continue;
         } 
         else if(i<0 || i>65535)
         {
             writetosocket(newsockfd, "1st Operand out of range([0,65535])! Please try again.");
             continue;
         }

         if(!(iss >> j))
         {
             writetosocket(newsockfd, "Missing 2nd Operand! Please try again.");
             continue;
         }
         else if(j<0 || j>65535)
         {
             writetosocket(newsockfd, "2nd Operand out of range([0,65535])! Please try again.");
             continue;
         }
         //std::cout << i << " " << op << " " << j << std::endl;
         iss.clear();

         int res = hs[op](i,j);
         if(res<0 || res>65535)
            writetosocket(newsockfd, "RESULT ERROR");
         else
            writetosocket(newsockfd, ("RESULT " + std::to_string(res)).c_str());

     }

     close(newsockfd);
     close(sockfd);
     return 0; 
}
