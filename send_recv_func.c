#include "sockset.h"

void recv_data(int fd, m_file *rx_tx_file, int option, int *size)
{
	int rc_byte = 0;

	if (option == RECV_NAME) {
		rc_byte = recv(fd, rx_tx_file->out_name, sizeof(rx_tx_file->out_name),0);
		if (rc_byte < 0){
			perror("Failed to receive file name: ");
			exit(-1);
		} else {
			printf("\nReceived file name succesfully: file[%s] \n", rx_tx_file->out_name);
		}
		trimm_path_name(rx_tx_file);
	} else if (option == RECV_MD5) {
		/* Received md5sum */
		rc_byte = recv(fd, rx_tx_file->md5sum, sizeof(rx_tx_file->md5sum), 0);
		if (rc_byte < 0) {
			perror("Failed to receive check sum: ");
			exit(-1);
		} else {
			printf("Received MD5 check sum succesfully: MD5 [%s]\n", rx_tx_file->md5sum);
		}
	} else if (option == RECV_SIZE) {
		rc_byte = recv(fd, size, sizeof(size), 0);
		if (rc_byte < 0) {
			perror("Recevid size of file: ");
			exit(-1);
		} else {
			printf("Received file size succesfully: File size [%d] byte\n", *size);
		}
	}
}

void send_data(int client_fd, m_file *rx_tx_file, int option, int size)
{
	int send_byte = 0;
	int total_send = 0;

	if (option == SEND_NAME) {
		while (total_send < strlen(rx_tx_file->in_name)) {
			send_byte = send(client_fd, rx_tx_file->in_name,
					sizeof(rx_tx_file->in_name),0);
			if (send_byte == -1) {
				perror("Failed to send file name to client: ");
				exit(SEND_TO_CLIENT_ERROR);
			}
			total_send += send_byte;
		}
	} else if (option == SEND_MD5) {
		total_send = 0;
		send_byte = 0;

		while (total_send < strlen(rx_tx_file->md5sum)) {
			send_byte = send(client_fd, rx_tx_file->md5sum,
					sizeof(rx_tx_file->md5sum),0);
			if (send_byte == -1){
				perror("Failed to send file md5sum to client: ");
				exit(-1);
			}
			total_send += send_byte;
		}
	} else if (option == SEND_SIZE) {
		send_byte = send(client_fd, &size, sizeof(size),0);
		if (send_byte == -1)
			perror("Failed to send size: ");
	}
}
