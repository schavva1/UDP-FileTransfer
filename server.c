#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
 
#define IP_PROTOCOL 0
#define NET_BUF_SIZE 1488
#define sendrecvflag 0
 
struct ack {
  int ack;
};

struct packet {
    int seq_no;
    char data[1488];
    int source_port;
    int dest_port;
};

// driver code
int main(int argc, char * argv[])
{
    int sockfd, nBytes, receive_length;
    struct sockaddr_in addr_con, client_con;
    int addrlen = sizeof(addr_con);
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(atoi(argv[1]));
    addr_con.sin_addr.s_addr = INADDR_ANY;
    char net_buf[NET_BUF_SIZE];
    char fname[32];
    char ack[1] = {'1'};
    struct packet p;
    struct ack a;
    FILE* fp;

    memset(&p, 0, sizeof(struct packet));
    memset(&a, 0, sizeof(struct ack));
    
    // socket()
    sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);
    p.source_port = htons(atoi(argv[1]));
    p.dest_port = htons(atoi(argv[1]));
 
    if (sockfd < 0)
        printf("\nfile descriptor not received!!\n");
    else
        printf("\nfile descriptor %d received\n", sockfd);
 
    // bind()
    if (bind(sockfd, (struct sockaddr*)&addr_con, sizeof(addr_con)) == 0)
        printf("\nSuccessfully binded!\n");
    else
        printf("\nBinding Failed!\n");
 
    printf("\nWaiting for file name...\n");
	
    nBytes = recvfrom(sockfd, fname,
                      32, sendrecvflag,
                      (struct sockaddr*)&addr_con, &addrlen);
                      
	char* ipString = inet_ntoa(addr_con.sin_addr);
	int port = ntohs(addr_con.sin_port); 
	printf("client ip: %s \n", ipString);
	printf("client port: %d \n", port);
	printf("name: %s \n", fname);
	
	int cli_addrlen = sizeof(client_con);
    client_con.sin_family = AF_INET;
    client_con.sin_port = htons(port);
    client_con.sin_addr.s_addr = inet_addr(ipString);
	
	memset(net_buf, '\0', sizeof(net_buf));
	
    nBytes = recvfrom(sockfd, net_buf,NET_BUF_SIZE, sendrecvflag,(struct sockaddr*)&addr_con, &addrlen);
    
    printf("length: %s \n", net_buf);
    unsigned long mm = atoi(net_buf);
	printf("mm: %ld", mm);
	fp = fopen(fname,"wb");
	int loop = 1;
	memset(net_buf,0,1488);
  
	while(loop*1500 < mm)
	{
		receive_length = recvfrom(sockfd,(void *)&p, sizeof(struct packet),0,(struct sockaddr*)&addr_con, &addrlen);
		if(receive_length < 0)
		{
			printf("error receiving \n");
		}
	printf("num of bytes received = %d \n",	receive_length);
    fwrite(p.data,1488,1,fp);
		//memset(net_buf,0,1500);
		memset(&p, 0, sizeof(struct packet));
    a.ack = 1;
    nBytes = sendto(sockfd, (const void *)&a, sizeof(struct ack), sendrecvflag,(struct sockaddr*)&client_con, cli_addrlen);
    memset(&a, 0, sizeof(struct ack));
		loop++;
	}
	//printf("remaining %d \n",(mm % 1500));
	receive_length = recvfrom(sockfd,(void *)&p, sizeof(struct packet), 0, (struct sockaddr*)&addr_con, &addrlen);
	printf("num of bytes received : %d \n",receive_length);
	fwrite(p.data, (mm % 1488), 1, fp);
	memset(net_buf, 0, 1488);
	fclose(fp);
	close(sockfd);
  return 0;
}
