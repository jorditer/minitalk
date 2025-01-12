/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jordi <jordi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:09:31 by jordi             #+#    #+#             */
/*   Updated: 2025/01/12 13:36:30 by jordi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
// sig_atomic_t guarantees any read or write will complete
// before any other operation is done
static sig_atomic_t	g_ack_received = 0;

void	client_signal_handler(int signum);
int		ft_atoi(char *str);
void	ft_send_signal(int pid, unsigned char byte);

// if sigaction fails or there are too args return error
int	main(int argc, char **argv)
{
	int					pid;
	struct sigaction	sa;
	char				*msg;

	sa.sa_handler = &client_signal_handler;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		write(2, "Signal setup failed\n", 19);
		return (1);
	}
	if (argc != 3)
	{
		write(1, "Args error\n", 11);
		return (1);
	}
	pid = ft_atoi(argv[1]);
	msg = argv[2];
	while (*msg)
	{
		ft_send_signal(pid, (unsigned char)*msg);
		msg++;
	}
	ft_send_signal(pid, '\n');
	return (0);
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
		nb = (nb * 10) + (str[i++] - '0');
	return (nb);
}

// waits for 0.1s max
void	ft_send_signal(int pid, unsigned char byte)
{
	int	i;
	int	wait_count;

	i = 7;
	wait_count = 0;
	while (i >= 0)
	{
		g_ack_received = 0;
		if (byte & (1 << i))
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		while (!g_ack_received && wait_count < 1000)
		{
			usleep(100);
			wait_count++;
		}
		if (!g_ack_received)
		{
			write(2, "Error: Server not responding\n", 28);
			exit(1);
		}
		i--;
	}
}

// When SIGUSR1 is received from server g_ack = 1 (conditional security)
void	client_signal_handler(int signum)
{
	if (signum == SIGUSR1)
		g_ack_received = 1;
}
