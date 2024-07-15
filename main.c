#include <bits/types/struct_timeval.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// change lib depending on device
#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/select.h>
#	include <unistd.h>
#endif

volatile sig_atomic_t keep_running = 1;

void sleep_ms(int millisec)
{
#ifdef __linux__
	struct timeval tv;
	tv.tv_sec = millisec / 1000;
	tv.tv_usec = (millisec / 1000) * 1000;
	select(0, NULL, NULL, NULL, &tv);

#elif _WIN32
	Sleep(millisec)
#endif
}
// Signal handler function
void sigint_handler(int sig)
{
	keep_running = 0;
}

int main(int argc, char* argv[])
{
	char fig_font[] = "Banner3-D";
	time_t current_time;
	FILE* fp;
	char cmd_buffer[1024];
	if(signal(SIGINT, sigint_handler) == SIG_ERR)
	{
		fprintf(stderr, "Can't set signal handler\n");
		return 1;
	}

	while(keep_running)
	{
		sleep(1);
		system("clear");

		time(&current_time);
		struct tm* local_time = localtime(&current_time);
		sprintf(cmd_buffer,
				"figlet -f %s \"%02d:%02d:%02d\"",
				fig_font,
				local_time->tm_hour,
				local_time->tm_min,
				local_time->tm_sec);

		fp = popen(cmd_buffer, "r");
		if(fp == NULL)
		{
			printf("Failed to run command\n");
			return 1;
		}
		// Read the output a line at a time
		while(fgets(cmd_buffer, sizeof(cmd_buffer), fp) != NULL)
		{
			printf("%s", cmd_buffer); // Print each line
		}
	}
	return 0;
}
