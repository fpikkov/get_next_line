#include <stdio.h>
#include <fcntl.h>
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
	fildes = open(file, O_RDONLY);
	if (fildes == -1)
	{
		printf("Failed to open file. \n");
		return (1);
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
