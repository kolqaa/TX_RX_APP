#include "header.h"

void tx_mode(m_file *rx_tx_file)
{
  int fd, option;
  struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_addr.s_addr = inet_addr(rx_tx_ip),
    .sin_port = htons(iPort)
  };

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
      fprintf(stderr, "Error creating socket\n");
      exit(-1);
  }

  //memset(&server_addr, 0, sizeof(server_addr));
  if (connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
      fprintf(stderr, "Error connecting to server\n");
      exit(-1);
  }
  recv(fd, rx_tx_file->ou_name, sizeof(fname),0);
  int size = 0;
  // read file size first
  read(fd, &size, sizeof(int));
  //  printf("{%s}\n", rx_tx_file->ou_name);
  int bytes_read = 0;
  int to_read = size;
  rx_tx_file->file_out = fopen("books23.pdf", "wb");
  // FILE* out = fopen(rx_tx_file->file_name, "wb");
  
  char *buffer = malloc(size);
  do
    {
      bytes_read = read(fd, buffer, to_read);
      printf("To read: %d\n", to_read);
      printf("Data read: %d\n", bytes_read);
      to_read = to_read - bytes_read;
      printf("name %s\n", rx_tx_file->ou_name);
      // save content to disk
      fwrite(buffer, 1, bytes_read, rx_tx_file->file_out);
    } while (to_read != 0);
  return ;
}
