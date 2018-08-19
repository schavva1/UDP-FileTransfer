Steps to run the code:



Server Side:
	
1.Compile the program -> gcc -o server server.c
	
2.Run the program by giving port number in command line -> ./server <portnum>	
Example output on Server side:

schavva1@remote06:~/CS528/test2$ hostname -i
128.226.180.168
schavva1@remote06:~/CS528/test2$ cd serv
schavva1@remote06:~/CS528/test2/serv$ ls
s  server.c
schavva1@remote06:~/CS528/test2/serv$ gcc -o s server.c
schavva1@remote06:~/CS528/test2/serv$ ./s 8080

Successfully binded!

Waiting for file name...
client ip: 128.226.180.162
client port: 50681
name: jelly.mkv
length: 95050627
mm: 95050627
num of bytes received = 1500
num of bytes received = 1500
num of bytes received = 1500
num of bytes received = 1500
num of bytes received = 1500
num of bytes received = 1500
num of bytes received = 1500
num of bytes received = 1500
num of bytes received = 1500
num of bytes received = 1500
num of bytes received = 1500
num of bytes received = 1500
num of bytes received = 1500
num of bytes received = 1500





Client Side:

1.Compile the program -> gcc -o client client.c
2.Run t
he program by giving command as follows:
	./client <IPaddress> <portnum> <filename>
3.UDP protocol transfers packets of 1500B length

Example output on Client side:

schavva1@remote00:~/CS528/test2$ ls
audio.mp3  c  client.c  jelly.mkv  nature.jpg  serv
schavva1@remote00:~/CS528/test2$ gcc -o c client.c
schavva1@remote00:~/CS528/test2$ ./c 128.226.180.168 8080 jelly.mkv

file descriptor 3 received
Total size: 95050627
value of send1 8
ack: 1
number of bytes sent:  = 1500
ack: 1
number of bytes sent:  = 1500
ack: 1
number of bytes sent:  = 1500
ack: 1
number of bytes sent:  = 1500
ack: 1
number of bytes sent:  = 1500
ack: 1
number of bytes sent:  = 1500
ack: 1
number of bytes sent:  = 1500



