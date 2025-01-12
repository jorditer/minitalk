/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jordi <jordi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 00:09:42 by jordi             #+#    #+#             */
/*   Updated: 2025/01/12 15:54:02 by jordi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
// #include <signal.h>

void	sig_handle(int signal, siginfo_t *signal_info, void *context);
void	process_byte(char *buff, int *buff_index, unsigned char byte);
void	ft_putnbr(long nbr);

/**
* @struct sigaction
* 
* Structure used to describe an action to be taken for a signal.
* @var sigaction::sa_handler && sigaction::sa_sigaction
* Pointer to a signal-catching function, sa_sigaction expanded version
* @var sigaction::sa_mask
* Additional set of signals to be blocked during execution.
* @var sigaction::sa_flags
* Special flags to affect the behavior of the signal.
*/

/* @function sigemptyset() Inits the mask to be empty, no signals blocked */
/* @function pause() blocks until a signal is received */
/* @flag SA_SIGINFO toggles to use sig_action instead of sig_handler */
/* @flag SA_RESTART if the signal is interrupted don't crash */
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
	sigact.sa_sigaction = &sig_handle;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO | SA_RESTART;
	if (sigaction(SIGUSR1, &sigact, NULL) == -1
		|| sigaction(SIGUSR2, &sigact, NULL) == -1)
	{
		write(2, "Signal setup failed\n", 19);
		return (1);
	}
	write(1, "\e[4:1mJelou! Vamos a empezar la correccion...\e[4:0m\n", 53);
	write(1, "Server ready!! 		 PID = ", 24);
	ft_putnbr(pid);
	write(1, "\n", 1);
	while (1)
		pause();
}

// if there are more bytes left in byte it doesn't do anything
// if it has reached the end it writes it and sets the index to 0
// The conditional is for security, the terminal wouldn't print uncomplete utf-8
// It would buffer it internally until it reveives all the bytes
// 0x80 = 10000000, 0xC0 = 11000000, 0xE0 = 11100000, 0XF0 = 11110000,...
// The first byte symbols the number of bytes the utf-8 character will have
// 0xxxxx = 1 byte, 110xxx = 2 byte, 1110xx, = 3, 11110xx = 4, etc.
// ASCII is only 7-bits
void	process_byte(char *buff, int *buff_index, unsigned char byte)
{
	buff[(*buff_index)++] = byte;
	buff[*buff_index] = '\0';
	if (((buff[0] & 0x80) == 0)
		|| ((buff[0] & 0xE0) == 0xC0 && *buff_index == 2)
		|| ((buff[0] & 0xF0) == 0xE0 && *buff_index == 3)
		|| ((buff[0] & 0xF8) == 0xF0 && *buff_index == 4))
	{
		write(1, buff, *buff_index);
		*buff_index = 0;
	}
}

// memset initializes to 0 the buff string, size 5, 4 UTF-max + 1 '\0'
// no need to = 0
void	sig_handle(int signal, siginfo_t *signal_info, void *context)
{
	static unsigned char	byte = 0;
	static int				bit_count = 0;
	static char				buff[5];
	static int				buff_index = 0;

	(void)context;
	if (signal == SIGUSR1)
		byte = (byte << 1) | 0;
	else
		byte = (byte << 1) | 1;
	bit_count++;
	if (bit_count == 8)
	{
		process_byte(buff, &buff_index, byte);
		byte = 0;
		bit_count = 0;
	}
	if (kill(signal_info->si_pid, SIGUSR1) == -1)
		write(2, "Error: Failed to send acknowledgment signal\n", 44);
}

void	ft_putnbr(long nbr)
{
	char	temp;

	if (nbr / 10 > 0)
		ft_putnbr(nbr / 10);
	temp = nbr % 10 + '0';
	write(1, &temp, 1);
}
