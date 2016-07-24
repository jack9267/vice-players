#include "main.h"

void logprintf(char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	char buffer[512];
	vsprintf(buffer, format, ap);
	va_end(ap);
	puts(buffer);
}

CRconClient* pRconClient = NULL;
bool bQuitApp = false;

int main(int argc, char* argv[])
{
	// Print welcome message.
	puts("\n VC:P Command Line Remote Console Client");
	puts(" ----------------------------------------");
	puts(" v1.1, 1st Sep 2010\n");

	// If there's not enough command line args, print usage.
	if (argc < 3)
	{
		puts(" Usage:");
		puts("   rcon <ip/host> <port> <password>");
		getc(stdin);
		exit(0);
	}

	// Setup the Remote Console client.
	pRconClient = new CRconClient(argv[1], atoi(argv[2]), argv[3]);
	char buf[2048];
	// While not wanting to quit...
	while (!bQuitApp)
	{
		// Process Remote Console client network messages.
		pRconClient->Process();
		if(kbhit())
		{
			gets(buf);
			pRconClient->Command(buf);
			printf("%s\n", buf);
		}
		// Sleep for 100ms
		Sleep(100);
	}

	// Delete the Remote Console client.
	delete pRconClient;

	return 0;
}
