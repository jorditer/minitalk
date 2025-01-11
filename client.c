/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jordi <jordi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:09:31 by jordi             #+#    #+#             */
/*   Updated: 2025/01/11 12:48:15 by jordi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static volatile sig_atomic_t g_ack_received = 0;

void client_signal_handler(int signum)
{
	if (signum == SIGUSR1)
		g_ack_received = 1;
}

int	ft_atoi(char *str)
{
	long	nb;
	int		len;
	int		i;

	len = 0;
	while (str && str[len])
		len++;
	i = 0;
	nb = 0;
	while (i < len)
		nb = (nb * 10) + (str[i++] - 48);
	return (nb);
}

void ft_send_signal(int pid, char c)
{
	int i;

	i = 7;
	while (i >= 0)
	{
		g_ack_received = 0;
		if (c & (1 << i))
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		// Wait for acknowledgment with timeout
		int wait_count = 0;
		while (!g_ack_received && wait_count < 1000)
		{
			usleep(100);
			wait_count++;
		}
		// Check if we timed out
		if (!g_ack_received)
		{
			write(2, "Error: Server not responding\n", 28);
			exit(1);
		}
		i--;
	}
}

int main(int argc, char **argv)
{
	int pid;
	struct sigaction sa;

	sa.sa_handler = &client_signal_handler;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		write(2, "Signal setup failed\n", 19);
		return (1);
	}
	if (argc != 3)
		write(1, "Args error", 10);
	else
	{
		pid = ft_atoi(argv[1]);
		while (argv[2] && *argv[2])
			ft_send_signal(pid, *argv[2]++);
		ft_send_signal(pid, '\n');
	}
	return (0);
}
