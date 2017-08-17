#include "sockset.h"

void rx_mode(m_file *rx_tx_file)
{
  int server_fd, client_fd;
  int size = 0;
  int file_fd = fileno(rx_tx_file->file_in);
  int send_byte = 0;
  int remain_data = 0;
  off_t offset = 0;

  struct stat stats;
  struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(iPort),
    .sin_addr.s_addr = inet_addr(rx_tx_ip)
  };
  struct sockaddr_in client_addr;
  socklen_t length = sizeof(client_addr);
  stat(rx_tx_file->in_name, &stats);

  /* Create an internet domain TCP socket */
  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Unable to create socket: ");
    exit(SOCKET_ERROR);
  }
  /* Bind socket to an network interface */
  if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == - 1) {
    perror("Unable to bind socket: ");
    exit(BIND_ERROR);
  }
  /* mark the socket used for incoming requests */
  if(listen(server_fd, 5) == -1){
    perror("Listen failed: ");
    exit(-1);
  } else
    puts("Listen accpeted, waiting for connection");
  
  client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &length);
  if (client_fd < 0) {
    perror("Accept failed: ");
    exit(-1);
  }
 
  /*send name of file*/
  send_data(client_fd, rx_tx_file, SEND_NAME, 0);
  /*send md5 sum*/
  take_check_sum(rx_tx_file,rx_tx_file->file_in, 0);
  send_data(client_fd, rx_tx_file, SEND_MD5, 0);
  /*send size of file*/
  size = stats.st_size;
  send_data(client_fd, rx_tx_file, SEND_SIZE, size);
    
  remain_data = stats.st_size;
  printf("File [%s] ready to send\ncheck sum [%s]\n", rx_tx_file->in_name,rx_tx_file->md5sum);
  while (((send_byte = sendfile(client_fd, file_fd, &offset, size)) > 0) && (remain_data > 0)) {
      remain_data -= send_byte;
  }
  if (!remain_data)
    printf("Succesfully send\n");
  else
    printf("Send failed\n");

  shutdown(client_fd, SHUT_RDWR);
  shutdown(server_fd, SHUT_RDWR);
  fclose(rx_tx_file->file_in);
  close(server_fd);
  close(client_fd);
  return ;
}
