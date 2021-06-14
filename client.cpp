// Please note this is a C program
// It compiles without warnings with gcc
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;
void error(char *msg)
{
    perror(msg);
    exit(0);
}


struct symbol{
    char **argv;
    char s; 
    int frequency = 0; 
    string fib; 
    int use_to_sort; 
    int assign;
    
    string line; 
    
};
void sort(vector<symbol> &s){
  int i, j; 
  for(i = 0; i < s.size()-1; i++){
    for(j = 0; j < s.size()-1-i; j++){
      if(s[j].use_to_sort < s[j+1].use_to_sort){
        symbol temp = s[j];
        s[j] = s[j+1];
        s[j+1] = temp;
      }
    }
  }
 
}


void* fib(void*pas){
    int sockfd, portno, n;
    
      symbol *x = (symbol *)pas;


    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    /*if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }*/
    portno = atoi(x->argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(x->argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    //printf("Please enter the message: ");
    bzero(buffer,256);
    //buffer = "1";
    //fgets(buffer,255,stdin);
    //fgets(buffer,255,x->test_number);
    buffer[0] = x->assign;
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    //printf("%s\n",buffer);
    x->line = buffer;
    return nullptr;
    
}

int main(int argc, char *argv[])
{   
    int number_symbol; 
    cin >> number_symbol; 
    
    string str;
    string line;// for "111011001111010110110110001110011 "
    string name_file; 

  
  //get input and set up vector 
    
    cin.ignore();
    static vector<symbol> v(number_symbol);
    pthread_t tid[number_symbol];
    
    
   
    
    for(int i = 0; i < number_symbol; i++){
        getline(cin, str);
        //cout << str << " " << i+1 << endl;
        //cout << str << endl;
        if(str.size() == 1){
            v[i].s = ' ';
            v[i].frequency = str[0]-48;
        }
        else{
            v[i].s = str[0];
            v[i].frequency = str[2]-48;
        
        }
        v[i].use_to_sort = v[i].frequency*100+int(v[i].s);
        v[i].argv = argv;
    //cout << v[i].frequency << endl;
  }
  
  getline(cin, name_file);
  //cout << name_file; 
  
  ifstream dep_file(name_file);
    if(dep_file.is_open()){
        getline(dep_file, line);
        
    }
    //cout << "line" << line << endl;
  
  
  
    sort(v);//sort the whole vector
  
  
  //cout << "v.size " << v.size() << endl;
  
  
  

    for(int i = 0; i < number_symbol; i++){
        v[i].assign = i+1;
    }

    
    
    
    
    for(int i = 0; i < number_symbol; i++){
    if(pthread_create(&tid[i], NULL, fib, &v[i])){
      //cout << "error";
      return 1; 
    }
  }
  
  for(int i = 0; i < number_symbol; i++){
      pthread_join(tid[i], NULL);
    
  }
  
  
  
   vector<string> compare_list; //a list after seprate by '11' 
    string x = "";

    for(int i =0; i < line.size(); i++){
        x += line[i];
        int n = x.size();
        if(x[n-1] == '1' && x[n-2] =='1'){
        compare_list.push_back(x);
        x = "";
        }

    }
    
   
  //compare the list and fib code to decode 
    string result = "";
    cout << "Decompressed message = ";
    for(int i = 0; i < compare_list.size(); i++){
        
      //cout << compare_list[i] << endl;
        for(int j = 0; j< v.size(); j++){
            if(compare_list[i] == v[j].line){
                result += v[j].s;
            }
          
        }
    }
    cout << result << endl;
  
  
  
  
  
  for(int i = 0; i < number_symbol; i++){
      //cout <<  v[i].line << endl;
  }
  return 0;
}
