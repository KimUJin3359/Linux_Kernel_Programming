#include <fcntl.h>
#include <unistd.h>

/*
int ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}
*/

int main()
{
	int fd = open("/dev/LED", O_RDWR);
	if (fd < 0)
	{
		write(1, "ERROR\n", 6);
		return (1);
	}
	write(fd, "APP ON\n", 7);

	// read string
	//char buf[100];
	//read(fd, buf, 100);
	//write(0, buf, ft_strlen(buf));

	// ioctl
	//ioctl(fd, 3, 0);
	//ioctl(fd, 4, 0);
	//ioctl(fd, 5, 0);

	// LED on/off
	while (1)
	{
		ioctl(fd, 6, 0);
		usleep(1000 * 1000);
		ioctl(fd, 7, 0);
		usleep(1000 * 1000);
	}
	close(fd);
	return (0);
}
