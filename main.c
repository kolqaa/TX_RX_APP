#include "sockset.h"

int iPort = DEFAULT_PORT;
char rx_tx_ip[IP_SIZE];
int mode = DEFAULT_MODE;

void usage()
{
	printf("usage: [-m:tx,rx] [-p:port] [-i:IP][-f:file to send]\n\n");
	printf("       -m:rx,tx  Mode Transmitter, Reciever\n");
	printf("       -p:int    Local port\n");
	printf("       -i:IP     Local IP address\n");
	printf("       -f:file   File to send(work in rx mode)\n");
	exit(1);
}

void check_param(int argc, char **argv, m_file *rx_tx_file)
{
	int i;

	for(i = 1; i < argc; i++) {
		if ((argv[i][0] == '-')) {
			switch (tolower(argv[i][1])) {
			case 'm':
				if (!strcmp("rx", &argv[i][3])) {
					mode = RX;
					printf("\n/***MODE: [RECEIVER]***/\n");
				} else if (!strcmp("tx", &argv[i][3])) {
					mode = TX;
					 printf("\n/***MODE: [TRANSMITTER]***/\n");
				}
				break ;
			case 'p':
				iPort = atoi(&argv[i][3]);
				break;
			case 'i':
				strcpy(rx_tx_ip, &argv[i][3]);
				break;
			case 'f':
				if ((rx_tx_file->file_in = fopen(&argv[i][3], "rb")) == NULL) {
					printf("Cannot open file.\n");
					exit (1);
				} else {
					strcpy(rx_tx_file->in_name, &argv[i][3]);
				}
				break ;
			default:
				usage();
				break;
			}
		}
	}
}

int main(int argc, char **argv)
{
	m_file *rx_tx_file;

	rx_tx_file = (m_file*)malloc(sizeof(*rx_tx_file));
	check_param(argc, argv, rx_tx_file);

	if (mode == TX)
		tx_mode(rx_tx_file);
	else if (mode == RX)
		rx_mode(rx_tx_file);
	else
		exit(printf("ERROR : [-m:mode] is Required parameter, choose mode rx,tx\n"));

	return 0;
}
