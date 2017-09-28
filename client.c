//Zaira E Cruz
//Nov. 23, 2016
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

//Source: Code for the checksum and tcp segment structure by Dr. Robin J Pottathuparambil

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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


int main(int argc,char **argv)
{

	FILE *client_file,*filept;

	struct tcp_hdr tcp_seg;
	unsigned short int cksum_arr[72];
  	unsigned int i,sum=0, cksum;
	int index=0;


    int sockfd, n,portnum,m,rc,x=1,p,seqnum=1;
    int len = sizeof(struct sockaddr),nbytes=200;
    char recvline[40960],* pointer=NULL;
    char buffer[256],buff[256];
    struct sockaddr_in servaddr;

    /* AF_INET - IPv4 IP , Type of socket, protocol*/
    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr,sizeof(servaddr));

    //port number is taken from main arguments.
    portnum=atoi(argv[1]);

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(portnum); // Server port number

    /* Convert IPv4 and IPv6 addresses from text to binary form */
	inet_pton(AF_INET,"129.120.151.94",&(servaddr.sin_addr));

    /* Connect to the server */
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

	tcp_seg.src = 21334;
	tcp_seg.des = 2200;
	tcp_seg.seq = 1;
	tcp_seg.ack = 0;
	tcp_seg.hdr_flags = 0x6002;
	tcp_seg.rec = 0;
	tcp_seg.cksum = 0;
	tcp_seg.ptr = 0;
	tcp_seg.opt = 0;
	//tcp_seg.data

	 memcpy(cksum_arr, &tcp_seg, 152); //Copying 24 bytes

  for (i=0;i<72;i++)               // Compute sum
   sum = sum + cksum_arr[i];

  cksum = sum >> 16;              // Fold once
  sum = sum & 0x0000FFFF;
  sum = cksum + sum;

  cksum = sum >> 16;             // Fold once more
  sum = sum & 0x0000FFFF;
  cksum = cksum + sum;

  /* XOR the sum for checksum */

	tcp_seg.cksum=(0xFFFF^cksum);

	while(x){
		client_file=fopen("client.txt","a+");

			//socket writes to connecting port.
	    		n = write(sockfd, &tcp_seg, sizeof(tcp_seg));
	    			if(n<0)
				error("ERROR writing to socket");

		//bzero(recvline,sizeof(recvline));
		read(sockfd,&tcp_seg,sizeof(tcp_seg));

				//Writing to a file.
			 	if(client_file!=NULL){
                                fprintf(client_file,"%s","First ACK received from sender.\n");
                                fprintf(client_file,"%s","SYN and ACK bit set to 1\n");
                                fprintf(client_file,"0x%04X\n",tcp_seg.src);
                                fprintf(client_file,"0x%04X\n",tcp_seg.des);
                                fprintf(client_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(client_file,"0x%08X\n",tcp_seg.ack);
				fprintf(client_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(client_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(client_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(client_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(client_file,"0x%04X\n",tcp_seg.opt);
                        }

                        //Client then reads from server the message received from HTTP request.
                        printf("First ACK received from sender.\n");
                        printf("SYN and ACK bit set to 1.\n");
                        printf("0x%04X\n", tcp_seg.src); // Printing all values
                        printf("0x%04X\n", tcp_seg.des);
                        printf("0x%08X\n", tcp_seg.seq);
                        printf("0x%08X\n", tcp_seg.ack);
                        printf("0x%04X\n", tcp_seg.hdr_flags);
                        printf("0x%04X\n", tcp_seg.rec);
                        printf("0x%04X\n", tcp_seg.cksum);
                        printf("0x%04X\n", tcp_seg.ptr);
                        printf("0x%08X\n", tcp_seg.opt);
/*
		//Second packet sent
		printf("\nStarting to send packet %d: \n",seqnum+1);

		tcp_seg.src=21334;
		tcp_seg.des=2200;
		tcp_seg.seq=2;
		tcp_seg.ack=51;
		tcp_seg.hdr_flags=0x6010;
		tcp_seg.rec=0;
		tcp_seg.cksum=0;
		tcp_seg.ptr=0;
		tcp_seg.opt=0;

		memcpy(cksum_arr, &tcp_seg, 152);

	        for (i=0;i<72;i++)               // Compute sum
	        sum = sum + cksum_arr[i];

	        cksum = sum >> 16;              // Fold once
	        sum = sum & 0x0000FFFF;
	        sum = cksum + sum;

        	cksum = sum >> 16;             // Fold once more
        	sum = sum & 0x0000FFFF;
        	cksum = cksum + sum;

        	tcp_seg.cksum=(0xFFFF^cksum);

		n=write(sockfd,&tcp_seg,sizeof(tcp_seg));
*/
		//Send packet - #4 Data from 0-128 bytes
		printf("\nStarting to send packet #3\n");
		//File handling-opening and fseeking first 128 bytes.
		filept=fopen(argv[2],"r");
		if(filept==0){
			error("\n File could not be opened.");
		}

		fseek(filept,128*index,SEEK_CUR);
		fread(tcp_seg.data,sizeof(char)*128,1,filept);
		fclose(filept);
printf("File name is : %s\n",argv[2]);
printf("First 128 bytes: %s\n",tcp_seg.data);

		tcp_seg.src=21334;
		tcp_seg.des=2200;
		tcp_seg.seq=2;
		tcp_seg.ack=51;
		tcp_seg.hdr_flags=0x6010;
		tcp_seg.rec=0;
		tcp_seg.cksum=0;
		tcp_seg.ptr=0;
		tcp_seg.opt=0;
		//DATA

		memcpy(cksum_arr, &tcp_seg, 152);

	        for (i=0;i<72;i++)               // Compute sum
	        sum = sum + cksum_arr[i];

	        cksum = sum >> 16;              // Fold once
	        sum = sum & 0x0000FFFF;
	        sum = cksum + sum;

        	cksum = sum >> 16;             // Fold once more
        	sum = sum & 0x0000FFFF;
        	cksum = cksum + sum;

        	tcp_seg.cksum=(0xFFFF^cksum);

		n=write(sockfd,&tcp_seg,sizeof(tcp_seg));

		//Read ack - packet from sender. Packet #5
		read(sockfd,&tcp_seg,sizeof(tcp_seg));
			//Writing to a file.
			 	if(client_file!=NULL){
                                fprintf(client_file,"%s","Second ACK received from sender.\nPacket #4.\n");
                                //fprintf(client_file,"%s","ACK bit set to 1. In closing connection.\n");
                                fprintf(client_file,"0x%04X\n",tcp_seg.src);
                                fprintf(client_file,"0x%04X\n",tcp_seg.des);
                                fprintf(client_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(client_file,"0x%08X\n",tcp_seg.ack);
				fprintf(client_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(client_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(client_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(client_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(client_file,"0x%04X\n",tcp_seg.opt);
//                                fclose(sender_file);
                        }

                        printf("\nReceiving packet #4 ACK\n");
                       // printf("ACK bit set to 1.\n");
                        printf("0x%04X\n", tcp_seg.src); // Printing all values
                        printf("0x%04X\n", tcp_seg.des);
                        printf("0x%08X\n", tcp_seg.seq);
                        printf("0x%08X\n", tcp_seg.ack);
                        printf("0x%04X\n", tcp_seg.hdr_flags);
                        printf("0x%04X\n", tcp_seg.rec);
                        printf("0x%04X\n", tcp_seg.cksum);
                        printf("0x%04X\n", tcp_seg.ptr);
                        printf("0x%08X\n", tcp_seg.opt);

		//Send packet #6 -Data from 128-256
		printf("\nStarting to send packet #5.\n");
		index=index+1;
		//File handling-opening and fseeking first 128 bytes.
		filept=fopen(argv[2],"r");
		if(filept==0){
			error("\n File could not be opened.");
		}

		fseek(filept,128*index,SEEK_CUR);
		fread(tcp_seg.data,sizeof(char)*128,1,filept);
		fclose(filept);
printf("File name is : %s\n",argv[2]);
printf("Second 128 bytes: %s\n",tcp_seg.data);


		tcp_seg.src=21334;
		tcp_seg.des=2200;
		tcp_seg.seq=3;
		tcp_seg.ack=52;
		tcp_seg.hdr_flags=0x6010;
		tcp_seg.rec=0;
		tcp_seg.cksum=0;
		tcp_seg.ptr=0;
		tcp_seg.opt=0;
		///DATA

		memcpy(cksum_arr, &tcp_seg, 152);

	        for (i=0;i<72;i++)               // Compute sum
	        sum = sum + cksum_arr[i];

	        cksum = sum >> 16;              // Fold once
	        sum = sum & 0x0000FFFF;
	        sum = cksum + sum;

        	cksum = sum >> 16;             // Fold once more
        	sum = sum & 0x0000FFFF;
        	cksum = cksum + sum;

        	tcp_seg.cksum=(0xFFFF^cksum);

		n=write(sockfd,&tcp_seg,sizeof(tcp_seg));

		//Read ack - packet from sender. Packet #7
		read(sockfd,&tcp_seg,sizeof(tcp_seg));
			//Writing to a file.
			 	if(client_file!=NULL){
                                fprintf(client_file,"%s","Third ACK received from sender.\nPacket #6\n");
                                //fprintf(client_file,"%s","ACK bit set to 1. In closing connection.\n");
                                fprintf(client_file,"0x%04X\n",tcp_seg.src);
                                fprintf(client_file,"0x%04X\n",tcp_seg.des);
                                fprintf(client_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(client_file,"0x%08X\n",tcp_seg.ack);
				fprintf(client_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(client_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(client_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(client_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(client_file,"0x%04X\n",tcp_seg.opt);
//                                fclose(sender_file);
                        }

                        printf("\nReceiving packet #6 -ACK\n");
                       // printf("ACK bit set to 1.\n");
                        printf("0x%04X\n", tcp_seg.src); // Printing all values
                        printf("0x%04X\n", tcp_seg.des);
                        printf("0x%08X\n", tcp_seg.seq);
                        printf("0x%08X\n", tcp_seg.ack);
                        printf("0x%04X\n", tcp_seg.hdr_flags);
                        printf("0x%04X\n", tcp_seg.rec);
                        printf("0x%04X\n", tcp_seg.cksum);
                        printf("0x%04X\n", tcp_seg.ptr);
                        printf("0x%08X\n", tcp_seg.opt);

		//Send packet #8 -Data from 256-384
		printf("\nStarting to send packet #7.\n");

		index=index+1;
		//File handling-opening and fseeking first 128 bytes.
		filept=fopen(argv[2],"r");
		if(filept==0){
			error("\n File could not be opened.");
		}

		fseek(filept,128*index,SEEK_CUR);
		fread(tcp_seg.data,sizeof(char)*128,1,filept);
		fclose(filept);
printf("File name is : %s\n",argv[2]);
printf("Third 128 bytes: %s\n",tcp_seg.data);

		tcp_seg.src=21334;
		tcp_seg.des=2200;
		tcp_seg.seq=4;
		tcp_seg.ack=53;
		tcp_seg.hdr_flags=0x6010;
		tcp_seg.rec=0;
		tcp_seg.cksum=0;
		tcp_seg.ptr=0;
		tcp_seg.opt=0;
		///DATA

		memcpy(cksum_arr, &tcp_seg, 152);

	        for (i=0;i<72;i++)               // Compute sum
	        sum = sum + cksum_arr[i];

	        cksum = sum >> 16;              // Fold once
	        sum = sum & 0x0000FFFF;
	        sum = cksum + sum;

        	cksum = sum >> 16;             // Fold once more
        	sum = sum & 0x0000FFFF;
        	cksum = cksum + sum;

        	tcp_seg.cksum=(0xFFFF^cksum);

		n=write(sockfd,&tcp_seg,sizeof(tcp_seg));

		//Read ack - packet from sender. Packet #9
		read(sockfd,&tcp_seg,sizeof(tcp_seg));
			//Writing to a file.
			 	if(client_file!=NULL){
                                fprintf(client_file,"%s","Fourth ACK received from sender.\nPacket #8\n");
                                //fprintf(client_file,"%s","ACK bit set to 1. In closing connection.\n");
                                fprintf(client_file,"0x%04X\n",tcp_seg.src);
                                fprintf(client_file,"0x%04X\n",tcp_seg.des);
                                fprintf(client_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(client_file,"0x%08X\n",tcp_seg.ack);
				fprintf(client_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(client_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(client_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(client_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(client_file,"0x%04X\n",tcp_seg.opt);
//                                fclose(sender_file);
                        }

                        printf("\nReceiving packet #8 -ACK\n");
                       // printf("ACK bit set to 1.\n");
                        printf("0x%04X\n", tcp_seg.src); // Printing all values
                        printf("0x%04X\n", tcp_seg.des);
                        printf("0x%08X\n", tcp_seg.seq);
                        printf("0x%08X\n", tcp_seg.ack);
                        printf("0x%04X\n", tcp_seg.hdr_flags);
                        printf("0x%04X\n", tcp_seg.rec);
                        printf("0x%04X\n", tcp_seg.cksum);
                        printf("0x%04X\n", tcp_seg.ptr);
                        printf("0x%08X\n", tcp_seg.opt);

		//Send packet #10 -Data from 384-512
		printf("\nStarting to send packet #9.\n");

		index=index+1;
		//File handling-opening and fseeking first 128 bytes.
		filept=fopen(argv[2],"r");
		if(filept==0){
			error("\n File could not be opened.");
		}

		fseek(filept,128*index,SEEK_CUR);
		fread(tcp_seg.data,sizeof(char)*128,1,filept);
		fclose(filept);
printf("File name is : %s\n",argv[2]);
printf("Fourth 128 bytes: %s\n",tcp_seg.data);

		tcp_seg.src=21334;
		tcp_seg.des=2200;
		tcp_seg.seq=5;
		tcp_seg.ack=54;
		tcp_seg.hdr_flags=0x6010;
		tcp_seg.rec=0;
		tcp_seg.cksum=0;
		tcp_seg.ptr=0;
		tcp_seg.opt=0;
		///DATA

		memcpy(cksum_arr, &tcp_seg, 152);

	        for (i=0;i<72;i++)               // Compute sum
	        sum = sum + cksum_arr[i];

	        cksum = sum >> 16;              // Fold once
	        sum = sum & 0x0000FFFF;
	        sum = cksum + sum;

        	cksum = sum >> 16;             // Fold once more
        	sum = sum & 0x0000FFFF;
        	cksum = cksum + sum;

        	tcp_seg.cksum=(0xFFFF^cksum);

		n=write(sockfd,&tcp_seg,sizeof(tcp_seg));

		//Read ack - packet from sender. Packet #11
		read(sockfd,&tcp_seg,sizeof(tcp_seg));
			//Writing to a file.
			 	if(client_file!=NULL){
                                fprintf(client_file,"%s","Fifth ACK received from sender.Packet #10.\n");
                                //fprintf(client_file,"%s","ACK bit set to 1. In closing connection.\n");
                                fprintf(client_file,"0x%04X\n",tcp_seg.src);
                                fprintf(client_file,"0x%04X\n",tcp_seg.des);
                                fprintf(client_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(client_file,"0x%08X\n",tcp_seg.ack);
				fprintf(client_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(client_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(client_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(client_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(client_file,"0x%04X\n",tcp_seg.opt);
//                                fclose(sender_file);
                        }

                        printf("\nReceiving packet #10 -ACK\n");
                       // printf("ACK bit set to 1.\n");
                        printf("0x%04X\n", tcp_seg.src); // Printing all values
                        printf("0x%04X\n", tcp_seg.des);
                        printf("0x%08X\n", tcp_seg.seq);
                        printf("0x%08X\n", tcp_seg.ack);
                        printf("0x%04X\n", tcp_seg.hdr_flags);
                        printf("0x%04X\n", tcp_seg.rec);
                        printf("0x%04X\n", tcp_seg.cksum);
                        printf("0x%04X\n", tcp_seg.ptr);
                        printf("0x%08X\n", tcp_seg.opt);


		//Send packet #12 -Data from 512-640
		printf("\nStarting to send packet #11.\n");

		index=index+1;
		//File handling-opening and fseeking first 128 bytes.
		filept=fopen(argv[2],"r");
		if(filept==0){
			error("\n File could not be opened.");
		}

		fseek(filept,128*index,SEEK_CUR);
		fread(tcp_seg.data,sizeof(char)*128,1,filept);
		fclose(filept);
printf("File name is : %s\n",argv[2]);
printf("Fifth 128 bytes: %s\n",tcp_seg.data);

		tcp_seg.src=21334;
		tcp_seg.des=2200;
		tcp_seg.seq=6;
		tcp_seg.ack=55;
		tcp_seg.hdr_flags=0x6010;
		tcp_seg.rec=0;
		tcp_seg.cksum=0;
		tcp_seg.ptr=0;
		tcp_seg.opt=0;
		///DATA

		memcpy(cksum_arr, &tcp_seg, 152);

	        for (i=0;i<72;i++)               // Compute sum
	        sum = sum + cksum_arr[i];

	        cksum = sum >> 16;              // Fold once
	        sum = sum & 0x0000FFFF;
	        sum = cksum + sum;

        	cksum = sum >> 16;             // Fold once more
        	sum = sum & 0x0000FFFF;
        	cksum = cksum + sum;

        	tcp_seg.cksum=(0xFFFF^cksum);

		n=write(sockfd,&tcp_seg,sizeof(tcp_seg));

		//Read ack - packet from sender. Packet #13
		read(sockfd,&tcp_seg,sizeof(tcp_seg));
			//Writing to a file.
			 	if(client_file!=NULL){
                                fprintf(client_file,"%s","Sixth ACK received from sender.\nPacket #12\n");
                                //fprintf(client_file,"%s","ACK bit set to 1. In closing connection.\n");
                                fprintf(client_file,"0x%04X\n",tcp_seg.src);
                                fprintf(client_file,"0x%04X\n",tcp_seg.des);
                                fprintf(client_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(client_file,"0x%08X\n",tcp_seg.ack);
				fprintf(client_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(client_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(client_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(client_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(client_file,"0x%04X\n",tcp_seg.opt);
//                                fclose(sender_file);
                        }

                        printf("\nReceiving packet #12 -ACK\n");
                       // printf("ACK bit set to 1.\n");
                        printf("0x%04X\n", tcp_seg.src); // Printing all values
                        printf("0x%04X\n", tcp_seg.des);
                        printf("0x%08X\n", tcp_seg.seq);
                        printf("0x%08X\n", tcp_seg.ack);
                        printf("0x%04X\n", tcp_seg.hdr_flags);
                        printf("0x%04X\n", tcp_seg.rec);
                        printf("0x%04X\n", tcp_seg.cksum);
                        printf("0x%04X\n", tcp_seg.ptr);
                        printf("0x%08X\n", tcp_seg.opt);

		//Send packet #14 -Data from 640-768
		printf("\nStarting to send packet #13.\n");

		index=index+1;
		//File handling-opening and fseeking first 128 bytes.
		filept=fopen(argv[2],"r");
		if(filept==0){
			error("\n File could not be opened.");
		}

		fseek(filept,128*index,SEEK_CUR);
		fread(tcp_seg.data,sizeof(char)*128,1,filept);
		fclose(filept);
printf("File name is : %s\n",argv[2]);
printf("Sixth 128 bytes: %s\n",tcp_seg.data);

		tcp_seg.src=21334;
		tcp_seg.des=2200;
		tcp_seg.seq=7;
		tcp_seg.ack=56;
		tcp_seg.hdr_flags=0x6010;
		tcp_seg.rec=0;
		tcp_seg.cksum=0;
		tcp_seg.ptr=0;
		tcp_seg.opt=0;
		///DATA

		memcpy(cksum_arr, &tcp_seg, 152);

	        for (i=0;i<72;i++)               // Compute sum
	        sum = sum + cksum_arr[i];

	        cksum = sum >> 16;              // Fold once
	        sum = sum & 0x0000FFFF;
	        sum = cksum + sum;

        	cksum = sum >> 16;             // Fold once more
        	sum = sum & 0x0000FFFF;
        	cksum = cksum + sum;

        	tcp_seg.cksum=(0xFFFF^cksum);

		n=write(sockfd,&tcp_seg,sizeof(tcp_seg));

		//Read ack - packet from sender. Packet #15
		read(sockfd,&tcp_seg,sizeof(tcp_seg));
			//Writing to a file.
			 	if(client_file!=NULL){
                                fprintf(client_file,"%s","Seventh ACK received from sender.\nPacket #14\n");
                                //fprintf(client_file,"%s","ACK bit set to 1. In closing connection.\n");
                                fprintf(client_file,"0x%04X\n",tcp_seg.src);
                                fprintf(client_file,"0x%04X\n",tcp_seg.des);
                                fprintf(client_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(client_file,"0x%08X\n",tcp_seg.ack);
				fprintf(client_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(client_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(client_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(client_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(client_file,"0x%04X\n",tcp_seg.opt);
//                                fclose(sender_file);
                        }

                        printf("\nReceiving packet #14 -ACK\n");
                       // printf("ACK bit set to 1.\n");
                        printf("0x%04X\n", tcp_seg.src); // Printing all values
                        printf("0x%04X\n", tcp_seg.des);
                        printf("0x%08X\n", tcp_seg.seq);
                        printf("0x%08X\n", tcp_seg.ack);
                        printf("0x%04X\n", tcp_seg.hdr_flags);
                        printf("0x%04X\n", tcp_seg.rec);
                        printf("0x%04X\n", tcp_seg.cksum);
                        printf("0x%04X\n", tcp_seg.ptr);
                        printf("0x%08X\n", tcp_seg.opt);

		//Send packet #16 -Data from 768-896
		printf("\nStarting to send packet #15.\n");

		index=index+1;
		//File handling-opening and fseeking first 128 bytes.
		filept=fopen(argv[2],"r");
		if(filept==0){
			error("\n File could not be opened.");
		}

		fseek(filept,128*index,SEEK_CUR);
		fread(tcp_seg.data,sizeof(char)*128,1,filept);
		fclose(filept);
printf("File name is : %s\n",argv[2]);
printf("Seventh 128 bytes: %s\n",tcp_seg.data);

		tcp_seg.src=21334;
		tcp_seg.des=2200;
		tcp_seg.seq=8;
		tcp_seg.ack=57;
		tcp_seg.hdr_flags=0x6010;
		tcp_seg.rec=0;
		tcp_seg.cksum=0;
		tcp_seg.ptr=0;
		tcp_seg.opt=0;
		///DATA

		memcpy(cksum_arr, &tcp_seg, 152);

	        for (i=0;i<72;i++)               // Compute sum
	        sum = sum + cksum_arr[i];

	        cksum = sum >> 16;              // Fold once
	        sum = sum & 0x0000FFFF;
	        sum = cksum + sum;

        	cksum = sum >> 16;             // Fold once more
        	sum = sum & 0x0000FFFF;
        	cksum = cksum + sum;

        	tcp_seg.cksum=(0xFFFF^cksum);

		n=write(sockfd,&tcp_seg,sizeof(tcp_seg));

		//Read ack - packet from sender. Packet 17
		read(sockfd,&tcp_seg,sizeof(tcp_seg));
			//Writing to a file.
			 	if(client_file!=NULL){
                                fprintf(client_file,"%s","Eighth ACK received from sender.\nPacket #16\n");
                                //fprintf(client_file,"%s","ACK bit set to 1. In closing connection.\n");
                                fprintf(client_file,"0x%04X\n",tcp_seg.src);
                                fprintf(client_file,"0x%04X\n",tcp_seg.des);
                                fprintf(client_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(client_file,"0x%08X\n",tcp_seg.ack);
				fprintf(client_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(client_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(client_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(client_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(client_file,"0x%04X\n",tcp_seg.opt);
//                                fclose(sender_file);
                        }

                        printf("\nReceiving packet #16 -ACK\n");
                       // printf("ACK bit set to 1.\n");
                        printf("0x%04X\n", tcp_seg.src); // Printing all values
                        printf("0x%04X\n", tcp_seg.des);
                        printf("0x%08X\n", tcp_seg.seq);
                        printf("0x%08X\n", tcp_seg.ack);
                        printf("0x%04X\n", tcp_seg.hdr_flags);
                        printf("0x%04X\n", tcp_seg.rec);
                        printf("0x%04X\n", tcp_seg.cksum);
                        printf("0x%04X\n", tcp_seg.ptr);
                        printf("0x%08X\n", tcp_seg.opt);

		//Send packet #18 -Data from 896-1024
		printf("\nStarting to send packet #17.\n");

		index=index+1;
		//File handling-opening and fseeking last 128 bytes.
		filept=fopen(argv[2],"r");
		if(filept==0){
			error("\n File could not be opened.");
		}

		fseek(filept,128*index,SEEK_CUR);
		fread(tcp_seg.data,sizeof(char)*128,1,filept);
		fclose(filept);
printf("File name is : %s\n",argv[2]);
printf("Eighth 128 bytes: %s\n",tcp_seg.data);

		tcp_seg.src=21334;
		tcp_seg.des=2200;
		tcp_seg.seq=9;
		tcp_seg.ack=58;
		tcp_seg.hdr_flags=0x6010;
		tcp_seg.rec=0;
		tcp_seg.cksum=0;
		tcp_seg.ptr=0;
		tcp_seg.opt=0;
		///DATA

		memcpy(cksum_arr, &tcp_seg, 152);

	        for (i=0;i<72;i++)               // Compute sum
	        sum = sum + cksum_arr[i];

	        cksum = sum >> 16;              // Fold once
	        sum = sum & 0x0000FFFF;
	        sum = cksum + sum;

        	cksum = sum >> 16;             // Fold once more
        	sum = sum & 0x0000FFFF;
        	cksum = cksum + sum;

        	tcp_seg.cksum=(0xFFFF^cksum);

		n=write(sockfd,&tcp_seg,sizeof(tcp_seg));

		bzero(tcp_seg.data,sizeof(tcp_seg.data));
		bzero(cksum_arr,sizeof(cksum_arr));

		//Read ack - packet from sender. Packet #19
		read(sockfd,&tcp_seg,sizeof(tcp_seg));
			//Writing to a file.
			 	if(client_file!=NULL){
                                fprintf(client_file,"%s","Nineth ACK received from sender.\nPacket #18\n");
                                //fprintf(client_file,"%s","ACK bit set to 1. In closing connection.\n");
                                fprintf(client_file,"0x%04X\n",tcp_seg.src);
                                fprintf(client_file,"0x%04X\n",tcp_seg.des);
                                fprintf(client_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(client_file,"0x%08X\n",tcp_seg.ack);
				fprintf(client_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(client_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(client_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(client_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(client_file,"0x%04X\n",tcp_seg.opt);
//				fprintf(client_file,"%s",tcp_seg.data);
//                                fclose(sender_file);
                        }

                        printf("\nReceiving packet #18 -ACK\n");
                       // printf("ACK bit set to 1.\n");
                        printf("0x%04X\n", tcp_seg.src); // Printing all values
                        printf("0x%04X\n", tcp_seg.des);
                        printf("0x%08X\n", tcp_seg.seq);
                        printf("0x%08X\n", tcp_seg.ack);
                        printf("0x%04X\n", tcp_seg.hdr_flags);
                        printf("0x%04X\n", tcp_seg.rec);
                        printf("0x%04X\n", tcp_seg.cksum);
                        printf("0x%04X\n", tcp_seg.ptr);
                        printf("0x%08X\n", tcp_seg.opt);
//			printf("%s\n",tcp_seg.data);

		//Initializing closing connection. Packet #3 FIN bit.
		printf("Starting to send packet #20.\n");

		tcp_seg.src=21334;
		tcp_seg.des=2200;
		tcp_seg.seq=20;
		tcp_seg.ack=0;
		tcp_seg.hdr_flags=0x6001;
		tcp_seg.rec=0;
		tcp_seg.cksum=0;
		tcp_seg.ptr=0;
		tcp_seg.opt=0;

		memcpy(cksum_arr, &tcp_seg,152);
		for (i=0;i<72;i++)               // Compute sum
                sum = sum + cksum_arr[i];

                cksum = sum >> 16;              // Fold once
                sum = sum & 0x0000FFFF;
                sum = cksum + sum;

                cksum = sum >> 16;             // Fold once more
                sum = sum & 0x0000FFFF;
                cksum = cksum + sum;

                tcp_seg.cksum=(0xFFFF^cksum);

		n=write(sockfd,&tcp_seg,sizeof(tcp_seg));

		bzero(cksum_arr,sizeof(cksum_arr));

		//Read ack - packet from sender. Packet #21
		read(sockfd,&tcp_seg,sizeof(tcp_seg));
			//Writing to a file.
			 	if(client_file!=NULL){
                                fprintf(client_file,"%s","Twenty-first ACK received from sender.\n");
                                fprintf(client_file,"%s","ACK bit set to 1. In closing connection.\n");
                                fprintf(client_file,"0x%04X\n",tcp_seg.src);
                                fprintf(client_file,"0x%04X\n",tcp_seg.des);
                                fprintf(client_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(client_file,"0x%08X\n",tcp_seg.ack);
				fprintf(client_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(client_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(client_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(client_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(client_file,"0x%04X\n",tcp_seg.opt);
				//fprintf(client_file,"%s\n",tcp_seg.data);
//                                fclose(sender_file);
                        }

                        printf("\nReceiving packet #21 after FIN bit by client sent.\n");
                        printf("ACK bit set to 1.\n");
                        printf("0x%04X\n", tcp_seg.src); // Printing all values
                        printf("0x%04X\n", tcp_seg.des);
                        printf("0x%08X\n", tcp_seg.seq);
                        printf("0x%08X\n", tcp_seg.ack);
                        printf("0x%04X\n", tcp_seg.hdr_flags);
                        printf("0x%04X\n", tcp_seg.rec);
                        printf("0x%04X\n", tcp_seg.cksum);
                        printf("0x%04X\n", tcp_seg.ptr);
                        printf("0x%08X\n", tcp_seg.opt);
			//printf("%s\n",tcp_seg.data);


        //Read ack from sender- Packet #22
		read(sockfd,&tcp_seg,sizeof(tcp_seg));

				//Writing to a file.
			 	if(client_file!=NULL){
                                fprintf(client_file,"%s"," ACK received from sender.\n");
                                fprintf(client_file,"%s","FIN bit set to 1. Last ACK from sender.\n");
                                fprintf(client_file,"0x%04X\n",tcp_seg.src);
                                fprintf(client_file,"0x%04X\n",tcp_seg.des);
                                fprintf(client_file,"0x%08X\n",tcp_seg.seq);
                                fprintf(client_file,"0x%08X\n",tcp_seg.ack);
				fprintf(client_file,"0x%04X\n",tcp_seg.hdr_flags);
                                fprintf(client_file,"0x%04X\n",tcp_seg.rec);
                                fprintf(client_file,"0x%04X\n",tcp_seg.cksum);
                                fprintf(client_file,"0x%04X\n",tcp_seg.ptr);
                                fprintf(client_file,"0x%04X\n",tcp_seg.opt);
			//	fprintf(client_file,"%s\n",tcp_seg.data);
                               fclose(client_file);
                        }

                        printf("\nReceiving last ACK from sender.\n");
                        printf("FIN bit set to 1.\n");
                        printf("0x%04X\n", tcp_seg.src); // Printing all values
                        printf("0x%04X\n", tcp_seg.des);
                        printf("0x%08X\n", tcp_seg.seq);
                        printf("0x%08X\n", tcp_seg.ack);
                        printf("0x%04X\n", tcp_seg.hdr_flags);
                        printf("0x%04X\n", tcp_seg.rec);
                        printf("0x%04X\n", tcp_seg.cksum);
                        printf("0x%04X\n", tcp_seg.ptr);
                        printf("0x%08X\n", tcp_seg.opt);
		//	printf("%s\n",tcp_seg.data);


		//Send last packet to sender from client.
		tcp_seg.src = 21334;
        	tcp_seg.des = 2200;
        	tcp_seg.seq = 6;
        	tcp_seg.ack = 81;
        	tcp_seg.hdr_flags = 0x6010;;
        	tcp_seg.rec = 0;
        	tcp_seg.cksum = 0;
        	tcp_seg.ptr = 0;
        	tcp_seg.opt = 0;

		 memcpy(cksum_arr, &tcp_seg, 152);

        	for (i=0;i<72;i++)               // Compute sum
        	sum = sum + cksum_arr[i];

        	cksum = sum >> 16;              // Fold once
        	sum = sum & 0x0000FFFF;
        	sum = cksum + sum;

        	cksum = sum >> 16;             // Fold once more
        	sum = sum & 0x0000FFFF;
        	cksum = cksum + sum;

        	tcp_seg.cksum=(0xFFFF^cksum);

		printf("Sending last packet to sender\n");
                write(sockfd,&tcp_seg,sizeof(tcp_seg));
     		x=0;
	}
	close(sockfd);
	printf("Done receiving\n");

    return 0;
}

