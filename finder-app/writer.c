/* 
 * Assignment 2
 *
 * writer.c
 * 
 * Mark Williams
 *
 * First Argument: Path to file (inc filename) - writefile
 * Second Argument: Text string to write into file - writestr
 *
 * Note: Do not need to create directories that do not exist
 */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>

int main (int argc, char* argv[])
{
	/* Assume success unless error */
	int retval = 0;

	/* Names to refer to arguments */
	char* writefile;
	char* writestr;

	/* Log to the console as well as log file */
	openlog("writer", LOG_PERROR, LOG_USER);

	/* For loop for easy-break-out error handling */
	for(;;)
	{
	
		/* Expect the right number of arguments, after program name */
		if(3 != argc)
		{
			/* Incorrect number of arguments */
			syslog(LOG_ERR, "Incorrect number of arguments: %d", argc);
			retval = 1;
			break;
		}

		writefile = argv[1];
		writestr = argv[2];

		/* Try and create a file for writing, if it exists clear */
		int fd = open(writefile, O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH);

		if(-1 == fd)
		{
			/* Error creating / opening file */
			syslog(LOG_ERR, "Error creating / opening file: %s", writefile);
			retval = 1;
			break;
		}

		/* Now let's write to the file */
		syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);

		ssize_t nr;
		nr = write(fd, writestr, strlen(writestr));

		if(-1 == nr)
		{
			/* Error writing to the file */
			syslog(LOG_ERR, "Error writing to file! Errno: %d", errno);
			retval = 1;
			break;
		}

		/* Close file, ignore returned value, attempt close */
		close(fd);

		/* Exit for loop */
		break;
	}

	closelog();

	return retval;
}
