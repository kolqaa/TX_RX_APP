#ifndef SOCKSET_H
# define SOCKSET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sendfile.h>
#include <openssl/md5.h>

#define DEFAULT_PORT 0000
#define TX 1
#define RX 0
#define DEFAULT_MODE 255
#define FILE_SIZE 128
#define IP_SIZE 128
#define BUFSIZE (1025*16)
#define EMPTY_SIZE 0
#define NAME_LEN 128
#define PATH_LEN 512
#define MD5_LEN 64

extern int iPort;
extern char rx_tx_ip[IP_SIZE];
extern int mode;

typedef struct file {
  char out_name[NAME_LEN];
  char rc_name[NAME_LEN];
  char in_name[NAME_LEN];
  char final_path[PATH_LEN];
  char md5sum[MD5_LEN];
  char recv_md5[MD5_LEN];
  FILE *file_out, *file_in;
} m_file;

enum send {
  SEND_NAME = 1,
  SEND_MD5,
  SEND_SIZE
};

enum receive {
  RECV_NAME = 1,
  RECV_MD5,
  RECV_SIZE
};

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
void take_check_sum(m_file *rx_tx_file, FILE *file, int RECEIVED);
void check_control_sum(m_file *rx_tx_file);
void trimm_path_name(m_file *rx_tx_file);
void recv_data(int fd, m_file *rx_tx_file, int option, int *size);
void send_data(int client_fd, m_file *rx_tx_file, int option, int size);
void create_file_and_dir(m_file *rx_tx_file, char *dir);

#endif
