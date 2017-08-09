#ifndef HEADER_H
# define HEADER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sendfile.h>

#define DEFAULT_PORT    5150
#define TX 1
#define RX 0
#define DEFAULT_MODE 255;
#define FILE_SIZE 512
#define IP_SIZE 128


extern int iPort;
extern char rx_tx_ip[IP_SIZE];
extern int mode;

typedef struct file {
  FILE *file_out, *file_in;
  char out_name[FILE_SIZE];
  char in_name[FILE_SIZE];
  char final_path[FILE_SIZE];
  char md5sum[FILE_SIZE];
} m_file;

enum error {
  SOCKET_ERROR,
  BIND_ERROR,
  ACCEPT_ERROR,
  SEND_TO_CLIENT_ERROR,
  EOF_ERROR,
  ERROR_TO_READ,
  WRITE_TO_CLIENT_ERROR,
  CONNECT_TO_SERV_ERROR,
  SAVE_CONTENT_ERROR,
  RECEIVE_ERROR
};

void tx_mode(m_file *rx_tx_file);
void rx_mode(m_file *rx_tx_file);

#endif
