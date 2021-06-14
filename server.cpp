// A simple server in the internet domain using TCP
// The port nu1mber is passed as an argument

// Please note this is a C program
// It compiles without warnings with gcc

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <vector> 
using namespace std;
void error(char *msg)
{
    perror(msg);
    exit(1);
}

void fireman(int)
{
   while (waitpid(-1, NULL, WNOHANG) > 0)
      //std::cout << "A child process ended" << std::endl;

    return;  
}
string code(int s){
  string word; 

  //Symbol *x = (Symbol *)pas;
  //int n = x->assign;
  int n = s;
  int fib[2000];

  fib[0] = 1; 
  fib[1] = 2; 

  int i = 0; 
  
  // if fib[i-1] is smaller or equal to n, generating the next one 
  for(i = 2; fib[i-1] <= n; i++){
    
    fib[i] = fib[i-1] + fib[i-2];

  }
  //cout << i << endl;
  int r = i-2; //the idex of max 
  
  int id = r; //n = 1 id = 0; 
  vector<char> v(r+3);

  while(n > 0){
    v[id] = '1';
    n = n-fib[id];

    id = id-1; 

   
        while (id>=0 && fib[id]>n)
        {
            v[id] = '0';
            id = id - 1;
        }
    }
 
    //additional '1' bit
    v[r+1] = '1';
    //v[r+2] = '\0';

    

    for(int j = 0; j < v.size()-1; j++){
      word += v[j];
    }
    //x->code = word;
    //return word; 
    return word;
  
  }

int main(int argc, char *argv[])
{    signal(SIGCHLD, fireman); 
     int sockfd, newsockfd, portno, clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
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
     listen(sockfd,10);
     clilen = sizeof(cli_addr);
     while(true){
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
     pid_t pid;
     pid = fork();
    if(pid == 0){
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     
     string str = code(int(buffer[0]));
     cout << "fib " << str << endl;
     if (n < 0) error("ERROR reading from socket");
     //printf("Here is the message: %s\n",buffer);
     //n = write(newsockfd,"I got your message",18);
     bzero(buffer,256);
     for(int i = 0; i < str.length(); i++){
         buffer[i] = str[i];
     }
     n = write(newsockfd,buffer,strlen(buffer));
     
     if (n < 0) error("ERROR writing to socket");
     }
     }
     return 0; 
}
