#include "sockset.h"

void check_control_sum(m_file *rx_tx_file)
{
	FILE *receieved;

	receieved = fopen(rx_tx_file->final_path, "rb");
	take_check_sum(rx_tx_file, receieved, 1);

	if (!strcmp(rx_tx_file->md5sum, rx_tx_file->recv_md5)) {
		printf("Sending file [%s] Check Sum [%s] OK\n",
				rx_tx_file->out_name,rx_tx_file->md5sum);
		printf("Receieved file [%s] Check Sum [%s] OK\n",
				rx_tx_file->out_name,rx_tx_file->recv_md5);
		printf("File receieved succesfully, without lost data.\n\n");
	}
	else {
		printf("Sending file ->[%s]<- Check Sum ->[%s]<- NOT OK\n",
				rx_tx_file->out_name,rx_tx_file->md5sum);
		printf("Receieved file ->[%s]<- Check Sum ->[%s]<- NOT OK\n",
				rx_tx_file->out_name,rx_tx_file->recv_md5);
		printf("Receiving file was failed, some data was corrupted.\n");
	}
}

void take_check_sum(m_file *rx_tx_file, FILE *file, int RECEIVED)
{
	MD5_CTX c;
	unsigned char buf[BUFSIZE];
	unsigned char md_buf[MD5_DIGEST_LENGTH];
	int inf = fileno(file);
	int i = 0;
	MD5_Init(&c);

	i = read(inf, buf, BUFSIZE);
	MD5_Update(&c, buf, (unsigned long)i);

	MD5_Final(md_buf, &c);

	i = 0;

	if(!RECEIVED)
		for(i = 0; i < MD5_DIGEST_LENGTH; i++)
			sprintf(rx_tx_file->md5sum+i*2, "%02x", md_buf[i]);
	else if (RECEIVED) 
		for(i = 0; i < MD5_DIGEST_LENGTH; i++)
			sprintf(rx_tx_file->recv_md5+i*2, "%02x", md_buf[i]);
}
