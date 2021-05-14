#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

static int fd;

void *led_check()
{
	while (1)
	{
		ioctl(fd, 5, 0);
		usleep(1000 * 50);
	}
}

int main()
{
	char buf[128];
	pthread_t tid;

	fd = open("/dev/SWITCH", O_RDWR);
	if (fd < 0)
	{
		write(1, "ERROR\n", 6);
		return (1);
	}
	write(fd, "APP ON\n", 7);
	// thread
	pthread_create(&tid, NULL, led_check, NULL);
	while (1)
	{
		write(0, "S-UJIN >> ", 10);
		fgets(buf, sizeof(buf), stdin);
		buf[strlen(buf) - 1] = '\0';	
		if (strcmp(buf, "1") == 0)
		{
			ioctl(fd, 3, 0);
			write(fd, "1", 1);
		}
		else if (strcmp(buf, "2") == 0)
		{
			ioctl(fd, 4, 0);
			write(fd, "2", 1);
		}
		else if (strcmp(buf, "0") == 0)
			break;
	}
	pthread_cancel(tid);
	pthread_join(tid, NULL);
	close(fd);
	return (0);
}
