#include "sockset.h"

void tx_mode(m_file *rx_tx_file)
{
  int fd;
  int size = 0;
  size_t bytes_read = 0;
  int to_read = 0;
  char *buffer;
  char *dir = "Received_files";
  
  struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_addr.s_addr = inet_addr(rx_tx_ip),
    .sin_port = htons(iPort)
  };
  
  if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Error creating socket: ");
    exit(-1);
  }
  
  connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr));// == -1) {
  //    perror("Error connecting to server:");
  //exit(-1);
  //}

  /*receive name of the file*/
  recv_data(fd, rx_tx_file, RECV_NAME, EMPTY_SIZE);
  /*receive md5 check sum for file*/
  recv_data(fd, rx_tx_file, RECV_MD5, EMPTY_SIZE);
  /*receive size of a file*/
  recv_data(fd, rx_tx_file, RECV_SIZE, &size);
  
  to_read = size;
  /*Create dir if neccesery where all received file will be store
    and open file for wirte data to it with the same name as we recv*/
  create_file_and_dir(rx_tx_file,dir);
  
  buffer = malloc(size);
  printf("\nReady to receive file [%s]\n", rx_tx_file->out_name);
  /*receiving file*/
  do
    {
      bytes_read = read(fd, buffer, to_read);
      to_read = to_read - bytes_read;
      /* save content to disk*/
      if (fwrite(buffer, 1, bytes_read, rx_tx_file->file_out) != bytes_read) {
	perror("Save content error: ");
	exit(SAVE_CONTENT_ERROR);
      }
    } while (to_read != 0);
  /*check control sum of receievd file*/
  check_control_sum(rx_tx_file);
  free(buffer);
  return ;
}
