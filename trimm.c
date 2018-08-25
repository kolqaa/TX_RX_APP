#include "sockset.h"

void trimm_path_name(m_file *rx_tx_file)
{
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

	while (tmpbuf[len])
		rx_tx_file->out_name[i++] = tmpbuf[len++];
}
