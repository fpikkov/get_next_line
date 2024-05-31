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
	list->content = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!list->content)
	{
		free(list);
		return (NULL);
	}
	list->next = NULL;
	return (list);
}

void	buff_to_list(char *content, char *buffer)
{
	int	i;

	i = 0;
	if (!buffer || !content)
		return ;
	while (buffer[i] != '\0')
	{
		content[i] = buffer[i];
		i++;
	}
	content[i] = '\0';
}

// Returns the last node in the linked list
t_list	*last_list(t_list **head)
{
	t_list	*temp;

	temp = *head;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

int	find_nl(char *content)
{
	int	i;

	i = 0;
	while (content[i] != '\0')
	{
		if (content[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

// TODO check for NULL returns from new_list calls
int	store_buff(t_list **head, int fd)
{
	char	*buffer;
	t_list	*temp;

	temp = last_list(head);
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (-1);
	buffer[BUFFER_SIZE] = '\0';
	while (read(fd, buffer, BUFFER_SIZE) > 0)
	{
		buff_to_list(temp->content, buffer);
		if (find_nl(temp->content) == 1)
			break ;
		temp->next = new_list();
		if (temp->next == NULL)
			return (-1);
		temp = temp->next;
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
	temp = *head;
	while (temp != NULL)
	{
		i = 0;
		while (temp->content[i] != '\0')
		{
			len++;
			if (temp->content[i] == '\n')
			{
				len++;
				break ;
			}
			i++;
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
		temp = temp->next;
	}
	line[i] = '\0';
	return (line);
}

void	clear_list(t_list **head)
{
	t_list	*first;
	t_list	*last;
	t_list	*temp;
	char	*content;

	last = last_list(head);
	content = last->content;
	while (*content != '\0' && *content != '\n')
		content++;
	temp = new_list();
	buff_to_list(temp->content, content);
	first = *head;
	last = first;
	while (last != NULL)
	{
		last = first->next;
		free(first->content);
		free(first);
		first = last;
	}
	*head = temp;
}

// TODO clear linked list in case of failure or when finished
char	*get_next_line(int fd)
{
	static t_list	*head;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) == -1)
		return (NULL);
	head = new_list();
	if (!head)
		return (NULL);
	// TODO Clear the list
	if (store_buff(&head, fd) == -1)
		return (NULL);
	line = next_line(&head);
	// TODO Clear the list
	if (!line)
		return (NULL);
	clear_list(&head);
	return (line);
}
