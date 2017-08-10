#include "sockset.h"

void rx_mode(m_file *rx_tx_file)
{
  int server_fd, client_fd;
  int result;
  unsigned int size = 0;
  char *buffer;
  
  struct stat stats;
  struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(iPort),
    .sin_addr.s_addr = inet_addr(rx_tx_ip)
  };
  struct sockaddr_in client_addr;
  socklen_t length = sizeof(client_addr);

  /* Create an internet domain TCP socket */
  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Unable to create socket: ");
    exit(SOCKET_ERROR);
  }
  /* Bind socket to an network interface */
  result = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (result == -1) {
    perror("Unable to bind socket: ");
    exit(BIND_ERROR);
  }
  /* mark the socket used for incoming requests */
  listen(server_fd, 5);
  printf("\nWait to connect transmitter\n\n");
  
  client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &length);
  if (client_fd == -1) {
    perror("Unable to accept peer connection: ");
    exit(ACCEPT_ERROR);
  }
  /* send file name */
  if(send(client_fd, rx_tx_file->in_name, strlen(rx_tx_file->in_name)+1,0) == -1) {
    perror("Failed to send file name to client: ");
    exit(SEND_TO_CLIENT_ERROR);
  }
  
  take_check_sum(rx_tx_file,rx_tx_file->file_in, 0);
  printf("File [%s] ready to send\ncheck sum [%s]\n\n", rx_tx_file->in_name,rx_tx_file->md5sum);

  if (send(client_fd, rx_tx_file->md5sum, strlen(rx_tx_file->md5sum)+1, 0) == -1) {
    perror("Failed to send file md5sum to client: ");
    exit(-1);
  }
  
  stat(rx_tx_file->in_name, &stats);
  size = stats.st_size;

  /* send size of file to the client */
  write(client_fd, &size, sizeof(int));

  /* read and send one full file */
  buffer = malloc(size);
  int inf = fileno(rx_tx_file->file_in);
  lseek(inf, 0L, 0);
  
  if (fread(buffer, 1, size, rx_tx_file->file_in) != size) {
    if(feof(rx_tx_file->file_in)) {
      perror("Unexpected end of file: ");
      exit(EOF_ERROR);
    }
    else {
      perror("Failed to read file: ");
      exit(ERROR_TO_READ);
    }
  }
  
  if (write(client_fd, buffer, size) != size) {
    perror("Receiver write to clinet_fd failed: ");
    exit(WRITE_TO_CLIENT_ERROR);
  }
  else
    printf("File send succesfully\n");
  
  fclose(rx_tx_file->file_in);
  close(server_fd);
  close(client_fd);
  free(buffer);
  return ;
}
