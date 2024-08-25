/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpikkov <fpikkov@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:26:57 by fpikkov           #+#    #+#             */
/*   Updated: 2024/06/10 14:22:30 by fpikkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Initiaalizes a new list node and allocates space for content
t_list	*new_list(void)
{
	t_list	*list;

	list = (t_list *)malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	list->next = NULL;
	list->content = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!list->content)
	{
		free(list);
		return (NULL);
	}
	return (list);
}

// Appends content of buffer to the end of linked list
void	append_list(t_list **head, char *content)
{
	t_list	*list;
	t_list	*last;
	int		i;

	if (!content || content[0] == '\0')
		return ;
	list = new_list();
	if (!list)
	{
		clear_list(head, 1);
		return ;
	}
	i = 0;
	while (content[i] != '\0')
	{
		list->content[i] = content[i];
		i++;
	}
	list->content[i] = '\0';
	last = last_list(head);
	if (!last)
		*head = list;
	else
		last->next = list;
}

// Function reads from file descriptor and writes contents into a linked list
int	store_buff(t_list **head, int fd)
{
	char	*buffer;
	int		bytes;

	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (-1);
	bytes = 1;
	while (bytes > 0 && find_nl(head) == 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1 || (!(*head) && bytes == 0))
		{
			if (bytes == -1 && (*head))
				clear_list(head, 1);
			free(buffer);
			return (-1);
		}
		buffer[bytes] = '\0';
		append_list(head, buffer);
	}
	free(buffer);
	return (0);
}

// Combines node contents into a string up until the newline character
char	*next_line(t_list **head)
{
	t_list	*temp;
	int		len;
	char	*line;
	int		i;
	int		j;

	if (!(*head))
		return (NULL);
	len = line_len(head);
	line = (char *)malloc((len + 1) * sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	temp = *head;
	while (i < len && temp != NULL)
	{
		j = 0;
		while (temp->content[j] != '\0' && temp->content[j] != '\n')
			line[i++] = temp->content[j++];
		if (temp->content[j] == '\n')
			line[i++] = '\n';
		temp = temp->next;
	}
	line[i] = '\0';
	return (line);
}

// Gets next line from file descriptor
char	*get_next_line(int fd)
{
	static t_list	*head = NULL;
	char			*line;
	long long		max_buff;

	max_buff = (((1 << 30) - 1) * 2);
	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > max_buff)
		return (clear_list(&head, 1));
	if (store_buff(&head, fd) == -1)
		return (clear_list(&head, 1));
	line = next_line(&head);
	if (!line)
		return (clear_list(&head, 1));
	if (line[0] == '\0')
	{
		free(line);
		return (clear_list(&head, 1));
	}
	clear_list(&head, 0);
	return (line);
}
