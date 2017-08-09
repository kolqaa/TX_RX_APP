#include "header.h"
#include <openssl/md5.h>

#define BUFSIZE (1025*16)
#define SIZER 32

void take_check_sum(m_file *rx_tx_file)
{
  MD5_CTX c; /* контекст хэша */
  unsigned char buf[BUFSIZE];
  unsigned char md_buf[MD5_DIGEST_LENGTH];

  /* В командной строке передается имя файла, для которого вычисляется хэш */
  int inf = fileno(rx_tx_file->file_in);

  /* Инициализируем контекст */
  MD5_Init(&c);

  /* Вычисляем хэш */
  int i = read(inf, buf, BUFSIZE);
  MD5_Update(&c, buf, (unsigned long)i);

  /* Помещаем вычисленный хэш в буфер md_buf */
  MD5_Final(md_buf, &c);

  i = 0;
  /* Отображаем результат */
  for(i = 0; i < MD5_DIGEST_LENGTH; i++){
    sprintf(rx_tx_file->md5sum+i*2, "%02x", md_buf[i]);
  }
  //  printf("MY HEXXX%s\n", rx_tx_file->md5sum);
  //  printf("check sum %s", rd_buf);
}

void rx_mode(m_file *rx_tx_file)
{
  int server_fd, client_fd, result;
  int size = 0;
  char *buffer;
  struct sockaddr_in client_addr;
  struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(iPort),
    .sin_addr.s_addr = inet_addr(rx_tx_ip)
  };
  socklen_t length = sizeof(client_addr);
  struct stat stats;

  // Create an internet domain TCP socket
  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      fprintf(stderr, "Unable to create socket\n");
      exit(SOCKET_ERROR);
  }
  // bind socket to an network interface
  result = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (result == -1) {
      fprintf(stderr, "Unable to bind socket\n");
      exit(BIND_ERROR);
  }
  // mark the socket used for incoming requests
  listen(server_fd, 5);
  printf("Wait to connect transmitter.\n");
  
  client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &length);
  if (client_fd == -1) {
      fprintf(stderr, "Unable to accept peer connection\n");
      exit(ACCEPT_ERROR);
  }

  take_check_sum(rx_tx_file);
  //send name of the file to the client 
  if(send(client_fd, rx_tx_file->in_name, strlen(rx_tx_file->in_name)+1,0) == -1) {
    fprintf(stderr, "Failed to send file name to client.\n");
    exit(SEND_TO_CLIENT_ERROR);
  }

  //  printf("MD5 SUM = %s\n", rx_tx_file->md5sum);
  if (send(client_fd, rx_tx_file->md5sum, strlen(rx_tx_file->md5sum)+1, 0) == -1) {
    fprintf(stderr, "Failed to send file md5sum to client.\n");
    exit(-1);
  }
  
  stat(rx_tx_file->in_name, &stats);
  size = stats.st_size;

  // send size of file to the client
  write(client_fd, &size, sizeof(int));

  // read and send one full file
  buffer = malloc(size);
  int inf = fileno(rx_tx_file->file_in);
  lseek(inf, 0L, 0);
  if (fread(buffer, 1, size, rx_tx_file->file_in) != size) {
    if(feof(rx_tx_file->file_in)) {
      fprintf(stderr, "Unexpected end of file\n");
      exit(EOF_ERROR);
    }
    else {
      fprintf(stderr, "Failed to read file.\n");
      exit(ERROR_TO_READ);
    }
  }
  if (write(client_fd, buffer, size) != size) {
    fprintf(stderr, "Receiver write to clinet_fd failed.\n");
    exit(WRITE_TO_CLIENT_ERROR);
  }
  else
    printf("File send succesfully\n");
  
  fclose(rx_tx_file->file_in);
  close(server_fd);
  close(client_fd);
  free(buffer);
}
