#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>

void proc_exit()
{
		int wstat;
		union wait wstat;
		pid_t	pid;

		while (TRUE) {
			pid = wait3 (&wstat, WNOHANG, (struct rusage *)NULL );
			if (pid == 0)
				return;
			else if (pid == -1)
				return;
			else
				printf ("Return code: %d\n", wstat.w_retcode);
		}
}
main ()
{
		signal (SIGCHLD, proc_exit);
		switch (fork()) {
			case -1:
				perror ("main: fork");
				exit (0);
			case 0:
				printf ("I'm alive (temporarily)\n");
				exit (rand());
			default:
				pause();
		}
}