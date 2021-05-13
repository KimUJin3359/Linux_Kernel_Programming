#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("/dev/my_drive", O_RDWR);

	if (fd < 0)
	{
		write(2,"ERROR\n", 6);
		return (1);
	}
	write(1, "RUN\n", 4);
	close(fd);
	return (0);
}
