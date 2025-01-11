/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jordi <jordi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:09:42 by jordi             #+#    #+#             */
/*   Updated: 2025/01/11 12:42:24 by jordi            ###   ########.fr       */
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

void	sig_handle(int signal, siginfo_t *signal_info, void *context)
{
	static int	i;
	static int	n;
	int			nb;

	(void)context;

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
	kill(signal_info->si_pid, SIGUSR1);  // Send acknowledgment back
}

//sigemptyset() Initializes the signal mask
//SA_RESTART the program restarts after sending the signal no EINTR
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
	sigact.sa_sigaction = &sig_handle;  // Use sa_sigaction instead of sa_handler
	sigemptyset(&sigact.sa_mask);
	// sigact.sa_handler = &sig_handle;
	sigact.sa_flags = SA_SIGINFO | SA_RESTART;
	if (sigaction(SIGUSR1, &sigact, NULL) == -1
		|| sigaction(SIGUSR2, &sigact, NULL) == -1)
	{
		write(2, "Signal setup failed\n", 19);
		return (1);
	}
	write(1, "\e[4:1mJelou! Vamos a empezar la correccion...\e[4:0m\n", 53);
	write(1, "Server ready, PID = ", 20);
	ft_putnbr(pid);
	write(1, "\n", 1);
	while (1)
		usleep(250);
}
