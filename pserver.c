//Zaira E Cruz
//Nov. 23,2016
//Program mimics the TCP opening 3 way handshaking. 
//In the third packet to establish connection the first 128 bytes of a total 
//1KB file is transmitted to the sender. The sender then sends an ACK packet acknowleding that they got that packet by 
//requesting the next set of data ( in this case the sequence # of the next packet).And also mimics the 4 way closing TCP protocol.
//Client first contacts the sender by sending a packet with an initial sequence number the SYN bit to 1. 
//Then sender responds with an ACK packet with SYN and ACK bit set to 1. The client ends the 3 way 
//handshaking by sending one last packet with ACK bit set to 1. 
//The 4 way closing protocol is started by the client again and the FIN bit is set to 1. The sender
//then responds with another packet that ACK the previous packet (ACK to 1). The sender again responds
//with last packet setting their FIN bit to 1. The client again finalizes the connection by sending last
//packet with ACK bit set to 1.

//Source: Checksum code and tcp segment structure - Dr. Robin J Pottathuparambil

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define SIZE 4096
//Error handling function.
void error(const char *msg) {
	perror(msg); 
	exit(0);
}

struct tcp_hdr{
                unsigned short int src;
                unsigned short int des;
                unsigned int seq;
                unsigned int ack;
                unsigned short int hdr_flags;
                unsigned short int rec;
                unsigned short int cksum;
                unsigned short int ptr;
                unsigned int opt;
		char data[128];
              };


int main(int argc, char **argv)
{
	FILE *sender_file,*transfer_data;

    char str[80];
    int cache_size=0;
    int listen_fd,conn_fd,x=0,portnum,n,seqnum=1;

    char buffer[BUFFER_SIZE],recvline[BUFFER_SIZE];

     struct sockaddr_in servaddr,serv_addr;
	portnum=atoi(argv[1]);

	struct tcp_hdr tcp_seg;

	struct tcp_hdr tcp_seg2;
	unsigned short int cksum_arr[12],some_arr[12];
	unsigned int i,sum=0, cksum;

    /* AF_INET - IPv4 IP , Type of socket, protocol*/
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);


    bzero(&serv_addr,sizeof(serv_addr));
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(portnum);


    /* Binds the above details to the socket */
	bind(listen_fd,  (struct sockaddr *) &servaddr, sizeof(servaddr));
    /* Start listening to incoming connections */
	listen(listen_fd, 20);


   	 /* Accepts an incoming connection */
	conn_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL);
	bzero(recvline,sizeof(recvline));

	tcp_seg.src = 2200;
  	tcp_seg.des = 21334;
  	tcp_seg.seq = 50;
  	tcp_seg.ack = 2;
  	tcp_seg.hdr_flags = 0x6012;
  	tcp_seg.rec = 0;
  	tcp_seg.cksum = 0;
  	tcp_seg.ptr = 0;
  	tcp_seg.opt = 0;

	 memcpy(cksum_arr, &tcp_seg, 24);

	for (i=0;i<12;i++)               // Compute sum
   	sum = sum + cksum_arr[i];

  	cksum = sum >> 16;              // Fold once
  	sum = sum & 0x0000FFFF;
  	sum = cksum + sum;

  	cksum = sum >> 16;             // Fold once more
  	sum = sum & 0x0000FFFF;
  	cksum = cksum + sum;

	tcp_seg.cksum=(0xFFFF^cksum);

	//keep accepting connections from the client.
	while((n=read(conn_fd,&tcp_seg,sizeof(tcp_seg)))>0)
	{
		sender_file=fopen("sender.txt","a+");

		//Check if the sequence number is equal to 1 and
		//SYN bit is set to 1 as well. This means that the hdr_flags should
		//be equal to 0x6002.

		if(tcp_seg.seq==1){
			//Writing to a file
			if(sender_file!=NULL){
				fprintf(sender_file,"%s","First packet sent from client\n");
				fprintf(sender_file,"%s","SYN bit set to 1. Connection request.\n");
				fprintf(sender_file,"0x%04X\n",tcp_seg.src);
				fprintf(sender_file,"0x%04X\n",tcp_seg.des);
				fprintf(sender_file,"0x%08X\n",tcp_seg.seq);
				fprintf(sender_file,"0x%08X\n",tcp_seg.ack);
				fprintf(sender_file,"0x%04X\n",tcp_seg.hdr_flags);
				fprintf(sender_file,"0x%04X\n",tcp_seg.rec);
				fprintf(sender_file,"0x%04X\n",tcp_seg.cksum);
				fprintf(sender_file,"0x%04X\n",tcp_seg.ptr);
				fprintf(sender_file,"0x%04X\n",tcp_seg.opt);
				//fprintf(sender_file,"%s\n",tcp_seg.data);
	        		fclose(sender_file);
			}
			printf("\nFirst packet receive from client.\n SYN bit set to 1. Connection request.\n");
			printf("\n0x%04X\n", tcp_seg.src); // Printing all values
       			printf("0x%04X\n", tcp_seg.des);
        		printf("0x%08X\n", tcp_seg.seq);
        		printf("0x%08X\n", tcp_seg.ack);
        		printf("0x%04X\n", tcp_seg.hdr_flags);
        		printf("0x%04X\n", tcp_seg.rec);
        		printf("0x%04X\n", tcp_seg.cksum);
        		printf("0x%04X\n", tcp_seg.ptr);
        		printf("0x%08X\n", tcp_seg.opt);
			//printf("%s\n",tcp_seg.data);

			tcp_seg.src=2200;
			tcp_seg.des=21334;
			tcp_seg.seq=50;
			tcp_seg.ack=2;
			tcp_seg.hdr_flags=0x6012;
			tcp_seg.rec=0;
			tcp_seg.cksum=0;
			tcp_seg.ptr=0;
			tcp_seg.opt=0;

			memcpy(cksum_arr, &tcp_seg,24);

			for (i=0;i<12;i++)               // Compute sum
  			sum = sum + cksum_arr[i];
  			cksum = sum >> 16;              // Fold once
  			sum = sum & 0x0000FFFF;
  			sum = cksum + sum;
  			cksum = sum >> 16;             // Fold once more
  			sum = sum & 0x0000FFFF;
  			cksum = cksum + sum;

			tcp_seg.cksum=(0xFFFF^cksum);

			write(conn_fd,&tcp_seg,sizeof(tcp_seg));
		}
		/*else if(tcp_seg.ack==2){
			//Writing to a file
			if(sender_file!=NULL){
				fprintf(sender_file,"%s","Second packet received from client\n");
				fprintf(sender_file,"%s","ACK bit set to 1. Connection Established.\n");
				fprintf(sender_file,"%s","End of handshaking.\n");
				fprintf(sender_file,"0x%04X\n",tcp_seg.src);
				fprintf(sender_file,"0x%04X\n",tcp_seg.des);
				fprintf(sender_file,"0x%08X\n",tcp_seg.seq);
				fprintf(sender_file,"0x%08X\n",tcp_seg.ack);
				fprintf(sender_file,"0x%04X\n",tcp_seg.hdr_flags);
				fprintf(sender_file,"0x%04X\n",tcp_seg.rec);
				fprintf(sender_file,"0x%04X\n",tcp_seg.cksum);
				fprintf(sender_file,"0x%04X\n",tcp_seg.ptr);
				fprintf(sender_file,"0x%04X\n",tcp_seg.opt);
				fclose(sender_file);
			}
		printf("\nSecond packet receive from client.\n");
		printf("SYN bit set to 1. Connection established.\n");
		printf("End of handshaking.\n");
		printf("\n0x%04X\n", tcp_seg.src); // Printing all values
       		printf("0x%04X\n", tcp_seg.des);
        	printf("0x%08X\n", tcp_seg.seq);
        	printf("0x%08X\n", tcp_seg.ack);
        	printf("0x%04X\n", tcp_seg.hdr_flags);
        	printf("0x%04X\n", tcp_seg.rec);
        	printf("0x%04X\n", tcp_seg.cksum);
        	printf("0x%04X\n", tcp_seg.ptr);
        	printf("0x%08X\n", tcp_seg.opt);

		}*/
		else if(tcp_seg.seq==2){
			transfer_data=fopen("transfer_data.txt","a+");
			fprintf(transfer_data,"%s\n",tcp_seg.data);
			fclose(transfer_data);

			//Writing to a file
			if(sender_file!=NULL){
				fprintf(sender_file,"%s","Second packet received from client\n");
				fprintf(sender_file,"%s","ACK bit set to 1. \n");
				fprintf(sender_file,"%s","End of handshaking. First 128 bytes received.\n");
				fprintf(sender_file,"0x%04X\n",tcp_seg.src);
				fprintf(sender_file,"0x%04X\n",tcp_seg.des);
				fprintf(sender_file,"0x%08X\n",tcp_seg.seq);
				fprintf(sender_file,"0x%08X\n",tcp_seg.ack);
				fprintf(sender_file,"0x%04X\n",tcp_seg.hdr_flags);
				fprintf(sender_file,"0x%04X\n",tcp_seg.rec);
				fprintf(sender_file,"0x%04X\n",tcp_seg.cksum);
				fprintf(sender_file,"0x%04X\n",tcp_seg.ptr);
				fprintf(sender_file,"0x%04X\n",tcp_seg.opt);
	        		fprintf(sender_file,"%s\n",tcp_seg.data);
				fclose(sender_file);
			}
			printf("\nSecond packet receive from client.\n");
			printf("ACK bit set to 1.\n");
			printf("\n0x%04X\n", tcp_seg.src); // Printing all values
       		printf("0x%04X\n", tcp_seg.des);
        	printf("0x%08X\n", tcp_seg.seq);
        	printf("0x%08X\n", tcp_seg.ack);
        	printf("0x%04X\n", tcp_seg.hdr_flags);
        	printf("0x%04X\n", tcp_seg.rec);
        	printf("0x%04X\n", tcp_seg.cksum);
        	printf("0x%04X\n", tcp_seg.ptr);
        	printf("0x%08X\n", tcp_seg.opt);
		printf("%s\n",tcp_seg.data);

			//Third packet received and ACK will be sent-- Packet #5
			tcp_seg.src=2200;
			tcp_seg.des=21334;
			tcp_seg.seq=51;
			tcp_seg.ack=3;
			tcp_seg.hdr_flags=0x6010;
			tcp_seg.rec=0;
			tcp_seg.cksum=0;
			tcp_seg.ptr=0;
			tcp_seg.opt=0;

			memcpy(cksum_arr, &tcp_seg,24);

			for (i=0;i<12;i++)               // Compute sum
  			sum = sum + cksum_arr[i];
  			cksum = sum >> 16;              // Fold once
  			sum = sum & 0x0000FFFF;
  			sum = cksum + sum;
  			cksum = sum >> 16;             // Fold once more
  			sum = sum & 0x0000FFFF;
  			cksum = cksum + sum;

			tcp_seg.cksum=(0xFFFF^cksum);

			write(conn_fd,&tcp_seg,sizeof(tcp_seg));
		}
		else if(tcp_seg.seq==3){
			transfer_data=fopen("transfer_data.txt","a+");
			fprintf(transfer_data,"%s\n",tcp_seg.data);
			fclose(transfer_data);
			//Writing to a file
			if(sender_file!=NULL){
				fprintf(sender_file,"%s","Third packet received from client\n");
				fprintf(sender_file,"%s","ACK bit set to 1. \n");
				fprintf(sender_file,"0x%04X\n",tcp_seg.src);
				fprintf(sender_file,"0x%04X\n",tcp_seg.des);
				fprintf(sender_file,"0x%08X\n",tcp_seg.seq);
				fprintf(sender_file,"0x%08X\n",tcp_seg.ack);
				fprintf(sender_file,"0x%04X\n",tcp_seg.hdr_flags);
				fprintf(sender_file,"0x%04X\n",tcp_seg.rec);
				fprintf(sender_file,"0x%04X\n",tcp_seg.cksum);
				fprintf(sender_file,"0x%04X\n",tcp_seg.ptr);
				fprintf(sender_file,"0x%04X\n",tcp_seg.opt);
	        		fprintf(sender_file,"%s\n",tcp_seg.data);
				fclose(sender_file);
			}
			printf("\nThird packet receive from client.\n");
			printf("ACK bit set to 1.\n");
			printf("\n0x%04X\n", tcp_seg.src); // Printing all values
       		printf("0x%04X\n", tcp_seg.des);
        	printf("0x%08X\n", tcp_seg.seq);
        	printf("0x%08X\n", tcp_seg.ack);
        	printf("0x%04X\n", tcp_seg.hdr_flags);
        	printf("0x%04X\n", tcp_seg.rec);
        	printf("0x%04X\n", tcp_seg.cksum);
        	printf("0x%04X\n", tcp_seg.ptr);
        	printf("0x%08X\n", tcp_seg.opt);
		printf("%s\n",tcp_seg.data);

			//Fourth packet received and ACK will be sent-- Sending Packet #7
			tcp_seg.src=2200;
			tcp_seg.des=21334;
			tcp_seg.seq=52;
			tcp_seg.ack=4;
			tcp_seg.hdr_flags=0x6010;
			tcp_seg.rec=0;
			tcp_seg.cksum=0;
			tcp_seg.ptr=0;
			tcp_seg.opt=0;

			memcpy(cksum_arr, &tcp_seg,24);

			for (i=0;i<12;i++)               // Compute sum
  			sum = sum + cksum_arr[i];
  			cksum = sum >> 16;              // Fold once
  			sum = sum & 0x0000FFFF;
  			sum = cksum + sum;
  			cksum = sum >> 16;             // Fold once more
  			sum = sum & 0x0000FFFF;
  			cksum = cksum + sum;

			tcp_seg.cksum=(0xFFFF^cksum);

			write(conn_fd,&tcp_seg,sizeof(tcp_seg));
		}
		else if(tcp_seg.seq==4){
			transfer_data=fopen("transfer_data.txt","a+");
			fprintf(transfer_data,"%s\n",tcp_seg.data);
			fclose(transfer_data);

			//Writing to a file
			if(sender_file!=NULL){
				fprintf(sender_file,"%s","Fourth packet received from client\n");
				fprintf(sender_file,"%s","ACK bit set to 1. \n");
				fprintf(sender_file,"0x%04X\n",tcp_seg.src);
				fprintf(sender_file,"0x%04X\n",tcp_seg.des);
				fprintf(sender_file,"0x%08X\n",tcp_seg.seq);
				fprintf(sender_file,"0x%08X\n",tcp_seg.ack);
				fprintf(sender_file,"0x%04X\n",tcp_seg.hdr_flags);
				fprintf(sender_file,"0x%04X\n",tcp_seg.rec);
				fprintf(sender_file,"0x%04X\n",tcp_seg.cksum);
				fprintf(sender_file,"0x%04X\n",tcp_seg.ptr);
				fprintf(sender_file,"0x%04X\n",tcp_seg.opt);
				fprintf(sender_file,"%s\n",tcp_seg.data);
	        		fclose(sender_file);
			}
			printf("\nFourth packet receive from client.\n");
			printf("ACK bit set to 1.\n");
			printf("\n0x%04X\n", tcp_seg.src); // Printing all values
       		printf("0x%04X\n", tcp_seg.des);
        	printf("0x%08X\n", tcp_seg.seq);
        	printf("0x%08X\n", tcp_seg.ack);
        	printf("0x%04X\n", tcp_seg.hdr_flags);
        	printf("0x%04X\n", tcp_seg.rec);
        	printf("0x%04X\n", tcp_seg.cksum);
        	printf("0x%04X\n", tcp_seg.ptr);
        	printf("0x%08X\n", tcp_seg.opt);
		printf("%s\n",tcp_seg.data);

			//Fifth packet received and ACK will be sent-- Sending Packet #9
			tcp_seg.src=2200;
			tcp_seg.des=21334;
			tcp_seg.seq=53;
			tcp_seg.ack=5;
			tcp_seg.hdr_flags=0x6010;
			tcp_seg.rec=0;
			tcp_seg.cksum=0;
			tcp_seg.ptr=0;
			tcp_seg.opt=0;

			memcpy(cksum_arr, &tcp_seg,24);

			for (i=0;i<12;i++)               // Compute sum
  			sum = sum + cksum_arr[i];
  			cksum = sum >> 16;              // Fold once
  			sum = sum & 0x0000FFFF;
  			sum = cksum + sum;
  			cksum = sum >> 16;             // Fold once more
  			sum = sum & 0x0000FFFF;
  			cksum = cksum + sum;

			tcp_seg.cksum=(0xFFFF^cksum);

			write(conn_fd,&tcp_seg,sizeof(tcp_seg));
		}
		else if(tcp_seg.seq==5){
			transfer_data=fopen("transfer_data.txt","a+");
			fprintf(transfer_data,"%s\n",tcp_seg.data);
			fclose(transfer_data);
			
			//Writing to a file
			if(sender_file!=NULL){
				fprintf(sender_file,"%s","Fifth packet received from client\n");
				fprintf(sender_file,"%s","ACK bit set to 1. \n");
				fprintf(sender_file,"0x%04X\n",tcp_seg.src);
				fprintf(sender_file,"0x%04X\n",tcp_seg.des);
				fprintf(sender_file,"0x%08X\n",tcp_seg.seq);
				fprintf(sender_file,"0x%08X\n",tcp_seg.ack);
				fprintf(sender_file,"0x%04X\n",tcp_seg.hdr_flags);
				fprintf(sender_file,"0x%04X\n",tcp_seg.rec);
				fprintf(sender_file,"0x%04X\n",tcp_seg.cksum);
				fprintf(sender_file,"0x%04X\n",tcp_seg.ptr);
				fprintf(sender_file,"0x%04X\n",tcp_seg.opt);
				fprintf(sender_file,"%s\n",tcp_seg.data);
	        		fclose(sender_file);
			}
			printf("\nFifth packet receive from client.\n");
			printf("ACK bit set to 1.\n");
			printf("\n0x%04X\n", tcp_seg.src); // Printing all values
       		printf("0x%04X\n", tcp_seg.des);
        	printf("0x%08X\n", tcp_seg.seq);
        	printf("0x%08X\n", tcp_seg.ack);
        	printf("0x%04X\n", tcp_seg.hdr_flags);
        	printf("0x%04X\n", tcp_seg.rec);
        	printf("0x%04X\n", tcp_seg.cksum);
        	printf("0x%04X\n", tcp_seg.ptr);
        	printf("0x%08X\n", tcp_seg.opt);
		printf("%s\n",tcp_seg.data);
			
			//Sixth packet received and ACK will be sent-- Sending Packet #11
			tcp_seg.src=2200;
			tcp_seg.des=21334;
			tcp_seg.seq=54;
			tcp_seg.ack=6;
			tcp_seg.hdr_flags=0x6010;
			tcp_seg.rec=0;
			tcp_seg.cksum=0;
			tcp_seg.ptr=0;
			tcp_seg.opt=0;

			memcpy(cksum_arr, &tcp_seg,24);

			for (i=0;i<12;i++)               // Compute sum
  			sum = sum + cksum_arr[i];
  			cksum = sum >> 16;              // Fold once
  			sum = sum & 0x0000FFFF;
  			sum = cksum + sum;
  			cksum = sum >> 16;             // Fold once more
  			sum = sum & 0x0000FFFF;
  			cksum = cksum + sum;

			tcp_seg.cksum=(0xFFFF^cksum);

			write(conn_fd,&tcp_seg,sizeof(tcp_seg));
		}
		else if(tcp_seg.ack==55){
			
			transfer_data=fopen("transfer_data.txt","a+");
			fprintf(transfer_data,"%s\n",tcp_seg.data);
			fclose(transfer_data);

			//Writing to a file
			if(sender_file!=NULL){
				fprintf(sender_file,"%s","Sixth packet received from client\n");
				fprintf(sender_file,"%s","ACK bit set to 1. \n");
				fprintf(sender_file,"0x%04X\n",tcp_seg.src);
				fprintf(sender_file,"0x%04X\n",tcp_seg.des);
				fprintf(sender_file,"0x%08X\n",tcp_seg.seq);
				fprintf(sender_file,"0x%08X\n",tcp_seg.ack);
				fprintf(sender_file,"0x%04X\n",tcp_seg.hdr_flags);
				fprintf(sender_file,"0x%04X\n",tcp_seg.rec);
				fprintf(sender_file,"0x%04X\n",tcp_seg.cksum);
				fprintf(sender_file,"0x%04X\n",tcp_seg.ptr);
				fprintf(sender_file,"0x%04X\n",tcp_seg.opt);
	        		fprintf(sender_file,"%s\n",tcp_seg.data);

				fclose(sender_file);
			}
			printf("\nSixth packet receive from client.\n");
			printf("ACK bit set to 1.\n");
			printf("\n0x%04X\n", tcp_seg.src); // Printing all values
       		printf("0x%04X\n", tcp_seg.des);
        	printf("0x%08X\n", tcp_seg.seq);
        	printf("0x%08X\n", tcp_seg.ack);
        	printf("0x%04X\n", tcp_seg.hdr_flags);
        	printf("0x%04X\n", tcp_seg.rec);
        	printf("0x%04X\n", tcp_seg.cksum);
        	printf("0x%04X\n", tcp_seg.ptr);
        	printf("0x%08X\n", tcp_seg.opt);
		printf("%s\n", tcp_seg.data);

			//Seventh packet received and ACK will be sent-- Sending Packet #13
			tcp_seg.src=2200;
			tcp_seg.des=21334;
			tcp_seg.seq=55;
			tcp_seg.ack=7;
			tcp_seg.hdr_flags=0x6010;
			tcp_seg.rec=0;
			tcp_seg.cksum=0;
			tcp_seg.ptr=0;
			tcp_seg.opt=0;

			memcpy(cksum_arr, &tcp_seg,24);

			for (i=0;i<12;i++)               // Compute sum
  			sum = sum + cksum_arr[i];
  			cksum = sum >> 16;              // Fold once
  			sum = sum & 0x0000FFFF;
  			sum = cksum + sum;
  			cksum = sum >> 16;             // Fold once more
  			sum = sum & 0x0000FFFF;
  			cksum = cksum + sum;

			tcp_seg.cksum=(0xFFFF^cksum);

			write(conn_fd,&tcp_seg,sizeof(tcp_seg));
		}
		else if(tcp_seg.seq==7){
			transfer_data=fopen("transfer_data.txt","a+");
			fprintf(transfer_data,"%s\n",tcp_seg.data);
			fclose(transfer_data);
			
			//Writing to a file
			if(sender_file!=NULL){
				fprintf(sender_file,"%s","Seventh packet received from client\n");
				fprintf(sender_file,"%s","ACK bit set to 1. \n");
				fprintf(sender_file,"0x%04X\n",tcp_seg.src);
				fprintf(sender_file,"0x%04X\n",tcp_seg.des);
				fprintf(sender_file,"0x%08X\n",tcp_seg.seq);
				fprintf(sender_file,"0x%08X\n",tcp_seg.ack);
				fprintf(sender_file,"0x%04X\n",tcp_seg.hdr_flags);
				fprintf(sender_file,"0x%04X\n",tcp_seg.rec);
				fprintf(sender_file,"0x%04X\n",tcp_seg.cksum);
				fprintf(sender_file,"0x%04X\n",tcp_seg.ptr);
				fprintf(sender_file,"0x%04X\n",tcp_seg.opt);
				fprintf(sender_file,"%s\n",tcp_seg.data);

	        		fclose(sender_file);
			}
			printf("\nSeventh packet receive from client.\n");
			printf("ACK bit set to 1.\n");
			printf("\n0x%04X\n", tcp_seg.src); // Printing all values
       		printf("0x%04X\n", tcp_seg.des);
        	printf("0x%08X\n", tcp_seg.seq);
        	printf("0x%08X\n", tcp_seg.ack);
        	printf("0x%04X\n", tcp_seg.hdr_flags);
        	printf("0x%04X\n", tcp_seg.rec);
        	printf("0x%04X\n", tcp_seg.cksum);
        	printf("0x%04X\n", tcp_seg.ptr);
        	printf("0x%08X\n", tcp_seg.opt);
		printf("%s\n", tcp_seg.data);

			//Eighth packet received and ACK will be sent-- Sending Packet #15
			tcp_seg.src=2200;
			tcp_seg.des=21334;
			tcp_seg.seq=56;
			tcp_seg.ack=8;
			tcp_seg.hdr_flags=0x6010;
			tcp_seg.rec=0;
			tcp_seg.cksum=0;
			tcp_seg.ptr=0;
			tcp_seg.opt=0;

			memcpy(cksum_arr, &tcp_seg,24);

			for (i=0;i<12;i++)               // Compute sum
  			sum = sum + cksum_arr[i];
  			cksum = sum >> 16;              // Fold once
  			sum = sum & 0x0000FFFF;
  			sum = cksum + sum;
  			cksum = sum >> 16;             // Fold once more
  			sum = sum & 0x0000FFFF;
  			cksum = cksum + sum;

			tcp_seg.cksum=(0xFFFF^cksum);

			write(conn_fd,&tcp_seg,sizeof(tcp_seg));
		}
		else if(tcp_seg.seq==8){
			transfer_data=fopen("transfer_data.txt","a+");
			fprintf(transfer_data,"%s\n",tcp_seg.data);
			fclose(transfer_data);

			//Writing to a file
			if(sender_file!=NULL){
				fprintf(sender_file,"%s","Eighth packet received from client\n");
				fprintf(sender_file,"%s","ACK bit set to 1. \n");
				fprintf(sender_file,"0x%04X\n",tcp_seg.src);
				fprintf(sender_file,"0x%04X\n",tcp_seg.des);
				fprintf(sender_file,"0x%08X\n",tcp_seg.seq);
				fprintf(sender_file,"0x%08X\n",tcp_seg.ack);
				fprintf(sender_file,"0x%04X\n",tcp_seg.hdr_flags);
				fprintf(sender_file,"0x%04X\n",tcp_seg.rec);
				fprintf(sender_file,"0x%04X\n",tcp_seg.cksum);
				fprintf(sender_file,"0x%04X\n",tcp_seg.ptr);
				fprintf(sender_file,"0x%04X\n",tcp_seg.opt);
				fprintf(sender_file,"%s\n",tcp_seg.data);

	        		fclose(sender_file);
			}
			printf("\nEighth packet receive from client.\n");
			printf("ACK bit set to 1.\n");
			printf("\n0x%04X\n", tcp_seg.src); // Printing all values
       		printf("0x%04X\n", tcp_seg.des);
        	printf("0x%08X\n", tcp_seg.seq);
        	printf("0x%08X\n", tcp_seg.ack);
        	printf("0x%04X\n", tcp_seg.hdr_flags);
        	printf("0x%04X\n", tcp_seg.rec);
        	printf("0x%04X\n", tcp_seg.cksum);
        	printf("0x%04X\n", tcp_seg.ptr);
        	printf("0x%08X\n", tcp_seg.opt);
		printf("%s\n", tcp_seg.data);


			//Nineth packet received and ACK will be sent-- Sending Packet #17
			tcp_seg.src=2200;
			tcp_seg.des=21334;
			tcp_seg.seq=57;
			tcp_seg.ack=9;
			tcp_seg.hdr_flags=0x6010;
			tcp_seg.rec=0;
			tcp_seg.cksum=0;
			tcp_seg.ptr=0;
			tcp_seg.opt=0;

			memcpy(cksum_arr, &tcp_seg,24);

			for (i=0;i<12;i++)               // Compute sum
  			sum = sum + cksum_arr[i];
  			cksum = sum >> 16;              // Fold once
  			sum = sum & 0x0000FFFF;
  			sum = cksum + sum;
  			cksum = sum >> 16;             // Fold once more
  			sum = sum & 0x0000FFFF;
  			cksum = cksum + sum;

			tcp_seg.cksum=(0xFFFF^cksum);

			write(conn_fd,&tcp_seg,sizeof(tcp_seg));
		}
		else if(tcp_seg.seq==9){
			transfer_data=fopen("transfer_data.txt","a+");
			fprintf(transfer_data,"%s\n",tcp_seg.data);
			fclose(transfer_data);

			//Writing to a file
			if(sender_file!=NULL){
				fprintf(sender_file,"%s","Nineth packet received from client\n");
				fprintf(sender_file,"%s","ACK bit set to 1. \n");
				fprintf(sender_file,"0x%04X\n",tcp_seg.src);
				fprintf(sender_file,"0x%04X\n",tcp_seg.des);
				fprintf(sender_file,"0x%08X\n",tcp_seg.seq);
				fprintf(sender_file,"0x%08X\n",tcp_seg.ack);
				fprintf(sender_file,"0x%04X\n",tcp_seg.hdr_flags);
				fprintf(sender_file,"0x%04X\n",tcp_seg.rec);
				fprintf(sender_file,"0x%04X\n",tcp_seg.cksum);
				fprintf(sender_file,"0x%04X\n",tcp_seg.ptr);
				fprintf(sender_file,"0x%04X\n",tcp_seg.opt);
				fprintf(sender_file,"%s\n",tcp_seg.data);
	        		fclose(sender_file);
			}
			printf("\nNineth packet receive from client.\n");
			printf("ACK bit set to 1.\n");
			printf("\n0x%04X\n", tcp_seg.src); // Printing all values
       		printf("0x%04X\n", tcp_seg.des);
        	printf("0x%08X\n", tcp_seg.seq);
        	printf("0x%08X\n", tcp_seg.ack);
        	printf("0x%04X\n", tcp_seg.hdr_flags);
        	printf("0x%04X\n", tcp_seg.rec);
        	printf("0x%04X\n", tcp_seg.cksum);
        	printf("0x%04X\n", tcp_seg.ptr);
        	printf("0x%08X\n", tcp_seg.opt);
		printf("%s\n", tcp_seg.data);

			bzero(tcp_seg.data,sizeof(tcp_seg.data));
			//Tenth packet received and ACK will be sent-- Sending Packet #19
			tcp_seg.src=2200;
			tcp_seg.des=21334;
			tcp_seg.seq=58;
			tcp_seg.ack=10;
			tcp_seg.hdr_flags=0x6010;
			tcp_seg.rec=0;
			tcp_seg.cksum=0;
			tcp_seg.ptr=0;
			tcp_seg.opt=0;

			memcpy(cksum_arr, &tcp_seg,24);

			for (i=0;i<12;i++)               // Compute sum
  			sum = sum + cksum_arr[i];
  			cksum = sum >> 16;              // Fold once
  			sum = sum & 0x0000FFFF;
  			sum = cksum + sum;
  			cksum = sum >> 16;             // Fold once more
  			sum = sum & 0x0000FFFF;
  			cksum = cksum + sum;

			tcp_seg.cksum=(0xFFFF^cksum);

			write(conn_fd,&tcp_seg,sizeof(tcp_seg));
		}
		else if(tcp_seg.seq==20)
		{

			printf("sequence num: 20\n");
			bzero(tcp_seg.data,sizeof(tcp_seg.data));
			 //Writing to a file
                        if(sender_file!=NULL){
                                fprintf(sender_file,"%s","Twentieth packet received from client.\n");
				fprintf(sender_file,"%s","Initialization of closing TCP connection. FIN bit set to 1.\n");
                                fprintf(sender_file,"0x%04X\n",tcp_seg.src);
                                fprintf(sender_file,"0x%04X\n",tcp_seg.des);
                                fprintf(sender_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(sender_file,"0x%08X\n",tcp_seg.ack);
                                fprintf(sender_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(sender_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(sender_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(sender_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(sender_file,"0x%04X\n",tcp_seg.opt);
  //                            fclose(sender_file);
                        }

			//Writing to the console.
			printf("Twentieth packet received from client.\n");
			printf("Initialization of closing TCP connection. FIN bit set to 1.\n");
			printf("\n0x%04X\n", tcp_seg.src); // Printing all values
                	printf("0x%04X\n", tcp_seg.des);
                	printf("0x%08X\n", tcp_seg.seq);
                	printf("0x%08X\n", tcp_seg.ack);
                	printf("0x%04X\n", tcp_seg.hdr_flags);
                	printf("0x%04X\n", tcp_seg.rec);
                	printf("0x%04X\n", tcp_seg.cksum);
                	printf("0x%04X\n", tcp_seg.ptr);
                	printf("0x%08X\n", tcp_seg.opt);

		//Sending packet #21
			tcp_seg.src=2200;
			tcp_seg.des=21334;
			tcp_seg.seq=70;	//random sequence number again.
			tcp_seg.ack=21; //seqnum sent plus one.
			tcp_seg.hdr_flags=0x6010;
			tcp_seg.rec=0;
			tcp_seg.cksum=0;
			tcp_seg.ptr=0;
			tcp_seg.opt=0;

			memcpy(cksum_arr, &tcp_seg, 24);
			for (i=0;i<12;i++)               // Compute sum
   			sum = sum + cksum_arr[i];

  			cksum = sum >> 16;              // Fold once
  			sum = sum & 0x0000FFFF;
  			sum = cksum + sum;

  			cksum = sum >> 16;             // Fold once more
  			sum = sum & 0x0000FFFF;
  			cksum = cksum + sum;

        		tcp_seg.cksum=(0xFFFF^cksum);

			write(conn_fd,&tcp_seg,sizeof(tcp_seg));

	//Sending packet 22.
			tcp_seg.src=2200;
                        tcp_seg.des=21334;
                        tcp_seg.seq=80; //random sequence number again.
                        tcp_seg.ack=21; //seqnum sent plus one.
                        tcp_seg.hdr_flags=0x6001;
                        tcp_seg.rec=0;
                        tcp_seg.cksum=0;
                        tcp_seg.ptr=0;
                        tcp_seg.opt=0;

			memcpy(cksum_arr, &tcp_seg, 24);

			for (i=0;i<12;i++)               // Compute sum
   			sum = sum + cksum_arr[i];

  			cksum = sum >> 16;              // Fold once
  			sum = sum & 0x0000FFFF;
  			sum = cksum + sum;

  			cksum = sum >> 16;             // Fold once more
  			sum = sum & 0x0000FFFF;
  			cksum = cksum + sum;

			tcp_seg.cksum=(0xFFFF^cksum);

			write(conn_fd,&tcp_seg,sizeof(tcp_seg));

			//Send last packet from sender with FINbit.
			//Reading last packet from client.
			read(conn_fd,&tcp_seg,sizeof(tcp_seg));


			 if(sender_file!=NULL){
                                fprintf(sender_file,"%s","Twentythird packet received from client.\n");
                        	fprintf(sender_file,"%s","This closes connection. ACK bit set to 1.\n");
                                fprintf(sender_file,"0x%04X\n",tcp_seg.src);
                                fprintf(sender_file,"0x%04X\n",tcp_seg.des);
                                fprintf(sender_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(sender_file,"0x%08X\n",tcp_seg.ack);
                                fprintf(sender_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(sender_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(sender_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(sender_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(sender_file,"0x%04X\n",tcp_seg.opt);
                                fclose(sender_file);
                        }


			printf("\nTwentythird packet received from client.\n");
			printf("This closes connection. ACK bit set to 1.\n");
			printf("\n0x%04X\n", tcp_seg.src); // Printing all values
        		printf("0x%04X\n", tcp_seg.des);
        		printf("0x%08X\n", tcp_seg.seq);
        		printf("0x%08X\n", tcp_seg.ack);
        		printf("0x%04X\n", tcp_seg.hdr_flags);
        		printf("0x%04X\n", tcp_seg.rec);
        		printf("0x%04X\n", tcp_seg.cksum);
        		printf("0x%04X\n", tcp_seg.ptr);
        		printf("0x%08X\n", tcp_seg.opt);

		}
	else{
		printf("ERROR\n");
	}
	}

        close (conn_fd); //close the connection

	return 0;
}
