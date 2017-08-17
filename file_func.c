#include "sockset.h"

void trimm_path_name(m_file *rx_tx_file) {
  int i = 0;
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
    len--;
  }

  while (tmpbuf[len]) {
    rx_tx_file->out_name[i++] = tmpbuf[len++];
  }
}

void create_file_and_dir(m_file *rx_tx_file, char *dir) {
  struct stat st;
  
  if (stat(dir, &st) == -1){
    mkdir(dir, 0777);
  }

  sprintf(rx_tx_file->final_path, "%s/%s", dir,rx_tx_file->out_name);
  rx_tx_file->file_out = fopen(rx_tx_file->final_path, "wb");
}

