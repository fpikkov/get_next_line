#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"

int	main(void)
{
	int		fildes;
	char	*line;
	int		count;

	count = 0;
	fildes = open("test.txt", O_RDONLY);
	if (fildes == -1)
	{
		printf("Failed to open filee \n");
		return (0);
	}
	line = get_next_line(fildes);
	printf("[Read: %i] [%s]\n", count, line);
	while (line != NULL)
	{
		free(line);
		count++;
		line = get_next_line(fildes);
		printf("[Read: %i] [%s]\n", count, line);

	}
	free(line);
	close(fildes);
	return (0);
}
