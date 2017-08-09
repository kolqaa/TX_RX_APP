#include "header.h"

void trimm_path_name(m_file *rx_tx_file) {
  int i = 0;
  int flag = 0;
  char tmpbuf[128];
  int len;
  
  strcpy(tmpbuf, rx_tx_file->out_name);
  len = strlen(tmpbuf);
  memset(&rx_tx_file->out_name, 0, sizeof(rx_tx_file->out_name));
  
  while (len > 0) {
    if (tmpbuf[len] == '/'){
      len++;
      break;
    }
    tmpbuf[len--];
  }
  
  while (tmpbuf[len]) {
    rx_tx_file->out_name[i++] = tmpbuf[len++];
  }
}

void tx_mode(m_file *rx_tx_file)
{
  int fd;
  int size = 0;
  int bytes_read = 0;
  int to_read = 0;
  char *buffer;
  char *dir = "Received_files";
  
  struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_addr.s_addr = inet_addr(rx_tx_ip),
    .sin_port = htons(iPort)
  };
  struct stat st = {0};

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
  
  recv(fd, rx_tx_file->out_name, sizeof(rx_tx_file->out_name),0);
  
  recv(fd, rx_tx_file->md5sum, sizeof(rx_tx_file->md5sum), 0);
  trimm_path_name(rx_tx_file);
  // read file size first
  read(fd, &size, sizeof(int));
  //  printf("{%s}\n", rx_tx_file->ou_name);
  to_read = size;
  if (stat(dir, &st) == -1){
    mkdir(dir, 0777);
  }
  sprintf(rx_tx_file->final_path, "%s/%s", dir,rx_tx_file->out_name);
  //printf("FINAL NAME IS %s\n", rx_tx_file->final_path);
  rx_tx_file->file_out = fopen(rx_tx_file->final_path, "wb");
 
  buffer = malloc(size);
  printf("Ready to receive file [%s]\n", rx_tx_file->out_name);
  do
    {
      bytes_read = read(fd, buffer, to_read);
      //      printf("To read: %d\n", to_read);
      //printf("Data read: %d\n", bytes_read);
      //      printf("File name: %s\n", name);
      to_read = to_read - bytes_read;
      // save content to disk
      if (fwrite(buffer, 1, bytes_read, rx_tx_file->file_out) != bytes_read) {
	fprintf(stderr, "Save content error.\n");
	exit(SAVE_CONTENT_ERROR);
      }
    } while (to_read != 0);
  printf("File [%s] receive succesfully\n", rx_tx_file->out_name);
  free(buffer);
  return ;
}
