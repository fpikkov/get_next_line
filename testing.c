#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "get_next_line.h"

int	main(int argc, char **argv)
{
	int		fildes;
	char	*line;
	int		count;
	char	*file;

	if (argc > 1 && *argv[1])
		file = argv[1];
	else
		file = "fresh.txt";
	count = 0;
	if (strcmp(file, "0") == 0)
		fildes = 0;
	else if (strcmp(file, "1") == 0)
		fildes = 1;
	else if (strcmp(file, "2") == 0)
		fildes = 2;
	else
		fildes = open(file, O_RDONLY);
	if (fildes == -1)
	{
		printf("Failed to open file. (fd: %i) \n", fildes);
		return (1);
	}
	line = get_next_line(fildes);
	printf("[Read: %i] %s\n", count, line);
	while (line != NULL)
	{
		free(line);
		count++;
		line = get_next_line(fildes);
		printf("[Read: %i] %s\n", count, line);
	}
	free(line);
	close(fildes);
	return (0);
}
