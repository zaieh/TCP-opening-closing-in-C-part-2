#include <stdio.h>
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
//		char data[256];
              };

int main(void)
{


  struct tcp_hdr tcp_seg;
  unsigned short int cksum_arr[72];
  unsigned int i,sum=0, cksum;
  char buff[]="abcdefghijklmnopqrstvwxyz";
char buff2[256]="";
  tcp_seg.src = 2200;
  tcp_seg.des = 21334;
  tcp_seg.seq = 50;
  tcp_seg.ack = 2;
  tcp_seg.hdr_flags = 0x6012;
  tcp_seg.rec = 0;
  tcp_seg.cksum = 0;
  tcp_seg.ptr = 0;
  tcp_seg.opt = 0;
//strcpy(tcp_seg.data,buff2);
//strcpy(tcp_seg.data,buff);
//tcp_seg.data="hello";

  memcpy(cksum_arr, &tcp_seg, 152); //Copying 24 bytes

/*  printf("tcp_seg.src: 0x%04X\n", tcp_seg.src); // Printing all values
  printf("tcp_seg.des: 0x%04X\n", tcp_seg.des);
  printf("tcp_seg.seq: 0x%08X\n", tcp_seg.seq);
  printf("tcp_seg.ack: 0x%08X\n", tcp_seg.ack);
  printf("tcp_seg.hdr_flags: 0x%04X\n", tcp_seg.hdr_flags);
  printf("tcp_seg.rec: 0x%04X\n", tcp_seg.rec);
  printf("tcp_seg.cksum: 0x%04X\n", tcp_seg.cksum);
  printf("tcp_seg.ptr: 0x%04X\n", tcp_seg.ptr);
  printf("tcp_seg.opt: 0x%08X\n", tcp_seg.opt);
*/
  for (i=0;i<72;i++)               // Compute sum
   sum = sum + cksum_arr[i];

  cksum = sum >> 16;              // Fold once
  sum = sum & 0x0000FFFF;
  sum = cksum + sum;

  cksum = sum >> 16;             // Fold once more
  sum = sum & 0x0000FFFF;
  cksum = cksum + sum;

  /* XOR the sum for checksum */
  printf("Checksum Value: 0x%04X\n", (0xFFFF^cksum));


	tcp_seg.cksum=(0xFFFF^cksum);

printf("Checksum Value: 0x%04X\n", (0xFFFF^cksum));
        printf("0x%04X\n", tcp_seg.src); // Printing all values
        printf("0x%04X\n", tcp_seg.des);
        printf("0x%08X\n", tcp_seg.seq);
        printf("0x%08X\n", tcp_seg.ack);
        printf("0x%04X\n", tcp_seg.hdr_flags);
        printf("0x%04X\n", tcp_seg.rec);
        printf("0x%04X\n", tcp_seg.cksum);
        printf("0x%04X\n", tcp_seg.ptr);
        printf("0x%08X\n", tcp_seg.opt);
//	printf("%s\n", tcp_seg.data);


}
