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
  struct stat st = {0};

  if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Error creating socket: ");
    exit(-1);
  }
  
  if (connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    perror("Error connecting to server:");
    exit(-1);
  }
  /* Received file name */
  if (recv(fd, rx_tx_file->out_name, sizeof(rx_tx_file->out_name),0) <= 0) {
    perror("Failed to receive file name: ");
    exit(-1);
  }
  trimm_path_name(rx_tx_file);
  
  /* Received md5sum */
  if (recv(fd, rx_tx_file->md5sum, sizeof(rx_tx_file->md5sum), 0) <= 0) {
    perror("Failed to receive check sum: ");
    exit(-1);
  }
  /* Read file size */
  if (read(fd, &size, sizeof(int)) < 0) {
    perror("Faild to read file size: ");
    exit(-1);
  }
  to_read = size;
  
  if (stat(dir, &st) == -1){
    mkdir(dir, 0777);
  }
  sprintf(rx_tx_file->final_path, "%s/%s", dir,rx_tx_file->out_name);
  rx_tx_file->file_out = fopen(rx_tx_file->final_path, "wb");
 
  buffer = malloc(size);
  printf("\nReady to receive file [%s]\n\n", rx_tx_file->out_name);
  do
    {
      bytes_read = read(fd, buffer, to_read);
      to_read = to_read - bytes_read;
      /* save content to disk */
      if (fwrite(buffer, 1, bytes_read, rx_tx_file->file_out) != bytes_read) {
	perror("Save content error: ");
	exit(SAVE_CONTENT_ERROR);
      }
    } while (to_read != 0);
  check_control_sum(rx_tx_file);
  free(buffer);
  return ;
}
