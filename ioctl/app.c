#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main()
{
	int fd = open("/dev/ioctl_test", O_RDWR);
	if (fd < 0)
	{
		write(1, "ERROR\n", 6);
		return (1);
	}
	write(fd, "APP ON\n", 7);

	// ioctl
	ioctl(fd, _IO(777, 3), 0);
	ioctl(fd, _IO(777, 4), 0);
	ioctl(fd, _IO(777, 5), 0);

	close(fd);
	return (0);
}
