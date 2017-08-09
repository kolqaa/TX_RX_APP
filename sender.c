#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define DEFAULT_PORT	5150
#define DEFAULT_BUFFER_LENGTH	64
#define TC 1
#define RC 0
#define FILE_SIZE 128
#define IP_SIZE 128
int iPort = DEFAULT_PORT;
char rx_tx_ip[IP_SIZE];
int mode; 

typedef struct file {
  int file_out, file_in;
  char fo_name[FILE_SIZE];
  char fi_name[FILE_SIZE];
} m_file;

void usage()
{
    printf("usage: [-m:tc,rc] [-p:int] [-i:IP][-n:x]\n\n");
    printf("       -m:rx,tx Mode Transmitter, Reciever\n");
    printf("       -p:int   Local port\n");
    printf("       -i:IP    Local IP address\n");
    printf("       -f:x     Send File\n");
    exit(1);
}

void check_param(int argc, char **argv, m_file *rx_tx_file)
{
  int i;
  for(i = 1; i < argc; i++)
    {
      if ((argv[i][0] == '-'))
	{
	  switch (tolower(argv[i][1]))
	    {
	    case 'm': {
	      if (!strcmp("rc", &argv[i][3])) {
		mode = RC;
		printf("RX mode set");
	      }
	      else if (!strcmp("tc", &argv[i][3])) {
		mode = TC;
		printf("TX mode set\n");
	      }
	      break ;
	    }
	    case 'p': {
	      iPort = atoi(&argv[i][3]);
	      break;
	    }
	    case 'i': {
	      strcpy(rx_tx_ip, &argv[i][3]);
	      break;
	    }
	    case 'f': {
	      if ((rx_tx_file->file_in = open(&argv[i][3], O_RDONLY)) == -1) {
		printf("Cannot open file.\n");
		exit (1);
	      }
	      else {
		strcpy(rx_tx_file->fi_name, &argv[i][3]);
		printf("open file succsss\n");
	      }
	      break ;
	    }
	    default:
	      usage();
	      break;
	    }
	}
    }
}

int main(int argc, char **argv)
{
  m_file *rx_tx_file;
  struct stat fileStat;
  rx_tx_file = (m_file*)malloc(sizeof(*rx_tx_file));
  check_param(argc, argv, rx_tx_file);
  fstat(rx_tx_file->file_in, &fileStat);
  printf("ip = {%s}\nport {%d}\nmode {%d}\nfile name {%s}\nfile size {%ld}\n", rx_tx_ip, iPort, mode, rx_tx_file->fi_name, fileStat.st_size);
  return 0;
}
      
