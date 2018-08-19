#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
 
#define IP_PROTOCOL 0
#define IP_ADDRESS "127.0.0.1" // localhost
#define PORT_NO 15050
#define DATA_SIZE 1488
#define cipherKey 'S'
#define sendrecvflag 0

unsigned long fsize(char* file)
{	
	FILE* fp;
	fp = fopen(file,"r");
	fseek(fp,0,SEEK_END);
	unsigned long length = (unsigned long)ftell(fp);
	printf("Total size: %d \n",length);
	fclose(fp);
	return length;
}

struct ack {
    int ack;
};

struct packet {
    int seq_no;
    char data[DATA_SIZE];
    int source_port;
    int dest_port;
};

// driver code
int main(int argc, char * argv[])
{
    int sockfd, nBytes, n;
    struct sockaddr_in addr_con;
    int addrlen = sizeof(addr_con);
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(atoi(argv[2]));
    addr_con.sin_addr.s_addr = inet_addr(argv[1]);
    char message[DATA_SIZE];
    struct packet p;
    struct ack a;
    char ack[1];
    FILE* fp;

    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 100000;

    memset(&p, 0, sizeof(struct packet));
    memset(&a, 0, sizeof(struct ack));
    
    // socket()
    sockfd = socket(AF_INET, SOCK_DGRAM,
                    IP_PROTOCOL);
    p.source_port = htons(atoi(argv[2]));
    p.dest_port = htons(atoi(argv[2]));
 
    if (sockfd < 0)
        printf("\nfile descriptor not received!!\n");
    else
        printf("\nfile descriptor %d received\n", sockfd);
   
  	char fname[32];
  	strcpy(fname, argv[3]);

	  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	
	  sendto(sockfd, fname, 32, sendrecvflag, (struct sockaddr*)&addr_con, addrlen);
  
  	fp = fopen(argv[3],"r");
  	fseek(fp,0,SEEK_END);
  	unsigned long length = (unsigned long)ftell(fp);
  	printf("Total size: %d \n",length);
  	fclose(fp);
  	
   	char str[1000];
   	sprintf(str, "%d", length);
   	int send1 = sendto(sockfd, str,strlen(str), sendrecvflag, (struct sockaddr*)&addr_con, addrlen);
   	printf("value of send1 %d \n",send1);
  
  	fp = fopen(argv[3],"rb");
   	memset(message,0,DATA_SIZE);
   	
    n = fread(p.data, DATA_SIZE, 1, fp);
    p.seq_no = 0;
    
    //strcpy(p.data, message);
    
   	int loop = 1;
    
   	while(loop*1500 < length)
   	{    
        int send2 = sendto(sockfd, (const void *)&p, sizeof(struct packet), sendrecvflag, (struct sockaddr*)&addr_con, addrlen);
    
    		nBytes = recvfrom(sockfd, (void *)&a, sizeof(struct ack), sendrecvflag,(struct sockaddr*)&addr_con, &addrlen);
    		printf("ack: %d\n", a.ack);
        memset(&a, 0, sizeof(struct ack));
        
    		if (nBytes > 0) {
            printf("number of bytes sent:  = %d \n",send2);
        		memset(message, 0, DATA_SIZE);
            memset(&p,0,sizeof(struct packet));
        		fread(p.data, DATA_SIZE, 1, fp);
            p.seq_no = 0;
        }
        
    		loop++;
   	}
    
    memset(message, 0, DATA_SIZE);
    memset(&p,0,sizeof(struct packet));
   	fread(p.data,(length % DATA_SIZE),1,fp);
    p.seq_no = 0;
    
   	int send3 = sendto(sockfd, (const void *)&p, sizeof(struct packet), 0, (struct sockaddr*)&addr_con, addrlen);	
   	if((send3) < 0)
   	{
   		printf("error in sending remaining bytes \n");
   	}
   	
   	printf("number of bytes sent  = %d \n",send3);
   	memset(message,0,DATA_SIZE);
   	fclose(fp);
   	close(sockfd); 
    return 0;
}
