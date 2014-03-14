/*
//kyle w

for line in configfile {
	parse(line) gives => path, time, notification_email
fork_to_backround( monitor(path,time,notification_email) }

//kyle s
function monitor {
	put md5sum in mem somewhere
	copyfile to storage directory
	while (true)
		if modtime_differet()
			whatever
		else whatever
	sleep timeinterval
}*/
#include <pthread.h>
#include <netdb.h>       
#include <netinet/in.h> 
#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

//global temp storage directory to keep the files
char tmpdir[100]="tmpdir/\0";
typedef struct {
	char *filename;
	int wait;
	char *email;
} mon_prop;

void *monitorfile(void *ptr) {

	mon_prop *frompthread;
	frompthread = (mon_prop *) ptr;
	char *filename;
	char *email;
	int wait;
	filename=frompthread->filename;
	email=frompthread->email;
	wait=frompthread->wait;
        printf("::Monitoring Started::\nfilename=%s email=%s wait=%i\n", filename,email,wait);
	struct stat fileStat;
        if(stat(filename,&fileStat) < 0) {
                printf("Error: Cannot stat %s\n", filename);
                }
	char path[100];
	strcpy(path, tmpdir);                
	strcat(path, filename);
	char content[100];
	strcpy(content, path);
	strcat(content, ".content");
	strcat(path, ".stat");
	FILE *f,*fp,*fp2;
	f = fopen(path, "w");
	fprintf(f,"Last Access: %d\n", fileStat.st_atime);	
	fprintf(f,"Last Modified: %d\n", fileStat.st_mtime);
	time_t *cmon = fileStat.st_mtime;
	fprintf(f,"Last Status Change: %d\n", fileStat.st_ctime);
	
	fprintf(f,"Mode: %d\n", fileStat.st_mode);
	fprintf(f,"User ID: %d\n", fileStat.st_uid);
	fprintf(f,"Group ID: %d\n", fileStat.st_gid);
	fprintf(f,"Size: %d\n", fileStat.st_size);
	fp = fopen(filename, "r");
	fp2 = fopen(content, "w");
	char line[128];
	char ch;
	while(1)
    	{
    	   ch = fgetc(fp);

       	   if(ch==EOF)
               break;
       	   else
               putc(ch,fp2);
    	}
        fclose(fp);
	fclose(fp2);
	fclose(f);
	struct stat fileStatNEW;	
	int *x;
	while (1) {
		struct stat fileStatNEW;
		printf("%s :: ",filename);
		x = stat(filename,&fileStatNEW);
		if(x < 0) {
                	printf("Error: Cannot stat %s :: error code: %i \n", filename, x);
               		 }

		 if (cmon != fileStatNEW.st_mtime) { 
			printf("File Changed!!\n");
			printf("Check NOT passed %d : %d\n", cmon, fileStatNEW.st_mtime);
			char buff[100];
			strcpy(buff, "diff -y ");
			strcat(buff, filename);
			strcat(buff, "  ");
			strcat(buff, content);
			FILE *p;
    			p = popen(buff, "r");
			char diff[150];
			char line[100];

			strcpy(diff, "DIFF REPORT:");
		    	while(fgets(line, sizeof(line)-1, p) != NULL) {
        			strcat(diff, line);
    			}
			pclose(p);

			char mailcmd[400];
			strcpy(mailcmd, "echo \'");
			strcat(mailcmd, diff);
			strcat(mailcmd, "\' | mail -s \'");
			strcat(mailcmd, filename);

			strcat(mailcmd, " has changed!!\' ");
			strcat(mailcmd, email);
			system(mailcmd);
			//old respawn 
			//monitorfile(filename, wait, email);
			//cant respawn to continue monitoring like this anymore
			// we need something in main that catches when this thread ends and respawns it
			printf("Thread done, change detected, respawning.");
			} 
			else{printf("Check passed %d : %d\n", cmon, fileStatNEW.st_mtime);
			}
	sleep(wait);
	}	


}

int main(int argc, char *argv[] ) {

//kyle w write parser for config file 
//kyle w write way to fork or thread this function below. IDK what would work better
// I think forking. He said in lecture forking was used to daemonize things
//we can discuss

//foreach parsed line do; {
int pid;
//thread identifier
pthread_t some_thread,some_thread2;
//an instance of our void* struct
mon_prop data,data2;
//set the crap
data.filename="test";
data.wait=15;
data.email="killerkyle113@gmail.com";
data2.filename="test2";
data2.wait=7;
data2.email="killerkyle113@gmail.com";
//make a thread , add check for if pid != 0, error
pid = pthread_create(&some_thread, NULL, (void *) &monitorfile, (void*) &data);
pid = pthread_create(&some_thread, NULL, (void *) &monitorfile, (void*) &data2);
//i forgot about this and it fucked me. If you dont have this for
//each spawned thread the program just coninues on and exits. Then
//since it exits, all its thread die. 
pthread_join(some_thread, NULL);
pthread_join(some_thread2, NULL);
//}
}

