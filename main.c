#include <stdio.h> //printf(3)
#include <stdlib.h>   //exit(3)
#include <unistd.h>   //fork(3), chdir(3), sysconf(3)
#include <signal.h>   //signal(3)
#include <sys/stat.h> //umask(3)
#include <syslog.h>   //syslog(3), openlog(3), closelog(3)
#include "MQTTClient.h"

#define DAEMON_NAME "delta"

int daemonize(){
	char* path = "/";
	char* outfile = "/dev/null";
	char* errfile = "/dev/null";
	char* infile = "/dev/null";
	pid_t pid = 0;
	pid_t sid = 0;
	int fd;

	pid = fork();
	if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    sid=setsid();
    if (sid < 0)
        exit(EXIT_FAILURE);

    signal(SIGCHLD,SIG_IGN);
    signal(SIGHUP,SIG_IGN);

    pid = fork();
	if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);
    chdir(path);

    for( fd=sysconf(_SC_OPEN_MAX); fd>0; --fd )
        close(fd);

    stdin=fopen(infile,"r");   //fd=0
    stdout=fopen(outfile,"w+");  //fd=1
    stderr=fopen(errfile,"w+");  //fd=2

    openlog(DAEMON_NAME,LOG_PID,LOG_DAEMON);
    return(EXIT_SUCCESS);

}

int hotspot(){
	
	return(EXIT_SUCCESS);
}

int init_mosquitto(){
	
}

int main(int argc, char* argv){
	int ttl=120;
    int delay=5;
	int res = daemonize();
	if(res!=0)
		exit(EXIT_FAILURE);

	while(ttl>0){
		syslog(LOG_NOTICE,"daemon ttl %d",ttl);
        sleep(delay);
        ttl-=delay;
	}
	syslog(LOG_NOTICE,"daemon ttl expired");
    closelog();
    return(EXIT_SUCCESS);
	//return 0;
}