#include "Client.h"

int main()
{
	sim::Client client;
	client.connect_to_server("127.0.0.1", 50000);

	client.run();

	client.disconnect();
	
	return 0;
}