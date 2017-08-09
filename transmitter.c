#include "header.h"

void tx_mode(m_file *rx_tx_file) {
  struct stat fileStat;
  fstat(rx_tx_file->file_in, &fileStat);
  printf("ip = {%s}\nport {%d}\nmode {%d}\nfile name {%s}\nfile size {%ld}\n", rx_tx_ip, iPort, mode, rx_tx_file->fi_name, fileStat.st_size);
}
