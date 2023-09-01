#include "systemcalls.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    bool ret = true;
    
    if(-1 == system(cmd))
    {
        ret = false;
    }
   
    return ret;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
 
    int status;
    pid_t pid;
    pid = fork();
    bool ret = false;


    /* Debug */
    printf("PID:%d - Path to command is: %s\r\n", pid, command[0]);

    /* -1 is error, skip past to return false */
    if(pid >= 0)
    {
        /* PID of 0 means child process running */
        if(0 == pid)
        {
            printf("PID:%d - Execv: %s\r\n", pid, command[0]);	

            /* Command 0 is the path to the command, from command[1] onwards is argv */
            execv(command[0], command);

	    printf("PID:%d - Execv failed!\r\n", pid);

            /* If execv returns it must have failed */
	    exit(EXIT_FAILURE);
        }

        /* Fork returns PID of child if successful */
        if(pid > 0)
        {   
            /* Wait on the process to complete */
            if(waitpid(pid, &status, 0) > 0)
            {
                printf("Wait PID returns\r\n");

                /* Check the exit status */                
                if(WIFEXITED(status))
		{
		    /* Normal exit status */
		    if(0 == WEXITSTATUS(status))
		    {
			/* We only return true if exited normally and exited without error */
		        ret = true;
		    }
		}       
            }
        }
    }

    va_end(args);

    return ret;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
    bool ret = false;
    int status;
    pid_t pid;

    /* Create file to write to, new file if exists */
    int fd = open(outputfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);

    /* Check file created */
    if(fd >= 0)
    {
        pid = fork();

        /* -1 is error, skip past to return false */
        if(pid >= 0)
        {
            /* PID of 0 means child process running */
            if(0 == pid)
            {
                /* Replace file descriptor with 1 (std out) so std out goes to file */
                if(dup2(fd, 1) >= 0)
                {           
                    close(fd);

                    /* Command 0 is the path to the command, from command[1] onwards is argv */
                    execv(command[0], command);

                    /* If execv returns it must have failed */
		    exit(EXIT_FAILURE);
                }
            }

            /* Fork returns PID of child if successful */
            if(pid > 0)
            {
                /* Wait on the process to complete */
                if(waitpid(pid, &status, 0) > 0)
                {
                    /* Check the exit status */
                    if(WIFEXITED(status))
                    {
                        /* Normal exit status */
                        if(0 == WEXITSTATUS(status))
                        {
                            /* We only return true if exited normally and exited without error */
                            ret = true;
                        }
                    }
		}

                close(fd);
            }
        }
    }
    va_end(args);

    return ret;
}

