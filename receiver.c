#include "header.h"

void rx_mode(m_file *rx_tx_file)
{
  int server_fd, client_fd, result;
  socklen_t length;
  struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(iPort),
    .sin_addr.s_addr = inet_addr(rx_tx_ip)
  };
  struct sockaddr_in client_addr;

  // Create an internet domain TCP socket
  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      fprintf(stderr, "Unable to create socket\n");
      exit(-1);
  }
  // bind socket to an network interface
  result = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (result == -1) {
      fprintf(stderr, "Unable to bind socket\n");
      exit(-1);
  }
  // mark the socket used for incoming requests
  listen(server_fd, 5);
  printf("Waiting for transmitter connection\n");
  
  length = sizeof(client_addr);
  client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &length);
  if (client_fd == -1) {
      fprintf(stderr, "Unable to accept peer connection\n");
      exit(-1);
  }
  
  //send name of the file to the client 
  send(client_fd, rx_tx_file->fi_name, strlen(rx_tx_file->fi_name)+1,0);
  struct stat stats;
  stat(rx_tx_file->fi_name, &stats);
  int size = stats.st_size;

 
  // send size of file to the client
  write(client_fd, &size, sizeof(int));

  // read and send one full file
  char *buffer = malloc(size);
  fread(buffer, 1, size, rx_tx_file->file_in);
  write(client_fd, buffer, size);
  fclose(rx_tx_file->file_in);
  
}
