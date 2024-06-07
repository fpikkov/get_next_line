/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpikkov <fpikkov@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:26:57 by fpikkov           #+#    #+#             */
/*   Updated: 2024/05/31 21:03:42 by fpikkov          ###   ########.fr       */
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
		return ;
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

// Returns the last node in the linked list
t_list	*last_list(t_list **head)
{
	t_list	*temp;

	if (!(*head))
		return (NULL);
	temp = *head;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

int	find_nl(t_list **head)
{
	t_list *last;
	int		i;

	i = 0;
	last = last_list(head);
	if (!last)
		return (0);
	while (last->content[i] != '\0')
	{
		if (last->content[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

// TODO check for NULL returns from new_list calls
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
			free(buffer);
			return (-1);
		}
		buffer[bytes] = '\0';
		append_list(head, buffer);
	}
	free(buffer);
	return (0);
}

// Finds the amount of characters to allocate until the newline character
int	line_len(t_list **head)
{
	t_list	*temp;
	int		len;
	int		i;

	len = 0;
	if (*head == NULL)
		return (0);
	temp = *head;
	while (temp != NULL)
	{
		i = 0;
		while (temp->content[i] != '\0')
		{
			if (temp->content[i] == '\n')
			{
				len++;
				break ;
			}
			i++;
			len++;
		}
		temp = temp->next;
	}
	return (len);
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

void	free_list(t_list **head)
{
	t_list	*current;
	t_list	*next;

	current = *head;
	next = current;
	while (next != NULL)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	*head = NULL;
}

char	*fetch_remainder(t_list **head)
{	
	t_list	*last;
	char	*buffer;
	int		i;
	int		j;

	last = last_list(head);
	if (!last)
		return (NULL);
	i = 0;
	while (last->content[i] != '\0' && last->content[i] != '\n')
		i++;
	if (last->content[i] == '\n')
		i++;
	if (last->content[i] == '\0')
		return (NULL);
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	j = 0;
	while (last->content[i] != '\0')
		buffer[j++] = last->content[i++];
	buffer[j] = '\0';
	return (buffer);
}

void	clear_list(t_list **head)
{
	char	*buffer;

	if (!(*head))
		return ;
	buffer = fetch_remainder(head);
	free_list(head);
	if (buffer)
	{
		append_list(head, buffer);
		free(buffer);
	}
}

// TODO clear linked list in case of failure or when finished
char	*get_next_line(int fd)
{
	static t_list	*head = NULL;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) == -1)
		return (NULL);
	if (store_buff(&head, fd) == -1)
		return (NULL);
	line = next_line(&head);
	// TODO Clear the list
	if (!line)
		return (NULL);
	if (line[0] == '\0')
	{
		free(line);
		clear_list(&head);
		return (NULL);
	}
	clear_list(&head);
	return (line);
}
