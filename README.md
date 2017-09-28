# TCP-opening-closing-in-C-part-2
Transfer of 1K of file - packets will contain and represent message being sent. 


Compilation:
		gcc pserver.c -o pserver
		gcc client.c -o client

Execution:
		./pserver <port>
		./client <port> <1KB_filename>

Program mimics the TCP opening 3 way handshaking. 
In the third packet to establish connection the first 128 bytes of a total 
1KB file is transmitted to the sender. The sender then sends an ACK packet acknowleding that they got that packet by 
requesting the next set of data ( in this case the sequence # of the next packet). This connection goes on until the Eighth
set of data is sent totaling the 1024 bytes of the 1KB file. Each time, a new checksum is computed now including the payload buffer.
Program also mimics the 4 way closing TCP protocol.
Client first contacts the sender by sending a packet with an initial sequence number the SYN bit to 1. 
Then sender responds with an ACK packet with SYN and ACK bit set to 1. The client ends the 3 way 
handshaking by sending one last packet with ACK bit set to 1. 
The 4 way closing protocol is started by the client again and the FIN bit is set to 1. The sender
then responds with another packet that ACK the previous packet (ACK to 1). The sender again responds
with last packet setting their FIN bit to 1. The client again finalizes the connection by sending last
packet with ACK bit set to 1.

Source for checksum and tcp segment structure Dr. Robin J Pottathuparambil

In total:
		client: 
			sends 11 packets
			receives 11 packets

		sender: 
			sends 11 packets
			receives 11 packets 
3 Files are created:

		client.txt:
			All the responds from the sender(ACK's)
		sender.txt:
			All packets sent from client including handshake, closing, and packets with payload.
		transfer_data.txt:
			Concatenation of all the data sent from the client to the sender.
