/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jordi <jordi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 11:02:54 by anlima            #+#    #+#             */
/*   Updated: 2025/01/05 22:34:12 by jordi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include <signal.h>

void	ft_putnbr(long nbr)
{
	char	temp;

	if (nbr / 10 > 0)
		ft_putnbr(nbr / 10);
	temp = nbr % 10 + '0';
	write(1, &temp, 1);
}

void	sig_handle(int signal)
{
	static int	i;
	static int	n;
	int			nb;

	if (signal == SIGUSR1)
		nb = 0;
	else
		nb = 1;
	n = (n << 1) + nb;
	i++;
	if (i == 8)
	{
		write(1, &n, 1);
		i = 0;
		n = 0;
	}
}

//sigemptyset() Initializes the signal mask
int	main(void)
{
	struct sigaction	sigact;
	pid_t				pid;

	pid = getpid();
	if (pid <= 0)
	{
		write(2, "Error: Invalid process ID\n", 25);
		return (1);
	}
	sigemptyset(&sigact.sa_mask);
	sigact.sa_handler = &sig_handle;
	sigact.sa_flags = SA_RESTART;
	if (sigaction(SIGUSR1, &sigact, NULL) == -1
		|| sigaction(SIGUSR2, &sigact, NULL) == -1)
	{
		write(2, "Signal setup failed\n", 19);
		return (1);
	}
	write(1, "\e[4:1mJelou! Vamos a ir empezando la correccion...\e[4:0m\n", 58);
	// write(1, "\e[4:3mWelcome to anlima's minitalk project!\e[4:0m\n", 51);
	write(1, "Server ready, PID = ", 20);
	ft_putnbr(pid);
	write(1, "\n", 1);
	while (1)
		usleep(500);
}
