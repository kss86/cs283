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
		if md5_is_different()
			whatever
		else whatever
	sleep timeinterval
}*/
#include <openssl/md5.h>
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


void monitorfile(char *filename, int wait) {
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
	static time_t cmon = fileStat.st_mtime;
	fprintf(f,"Last Status Change: %d\n", fileStat.st_ctime);
	
	fprintf(f,"Mode: %d\n", fileStat.st_mode);
	fprintf(f,"User ID: %d\n", fileStat.st_uid);
	fprintf(f,"Group ID: %d\n", fileStat.st_gid);
	fprintf(f,"Size: %d\n", fileStat.st_size);
	fp = fopen(filename, "r");
	fp2 = fopen(content, "w");
	char line[128];
	while( fgets(line,sizeof line, fp) != NULL )
        {
            fputs(line, fp2);
        }
        fclose(fp);

	fclose(f);
	struct stat fileStatNEW;	
	while (1) {
		int x;
		x = stat(filename,&fileStatNEW);
		if(x < 0) {
                		printf("Error: Cannot stat %s :: error code: %i \n", filename, x);
               		 	}
		if (cmon != &fileStatNEW.st_mtime) { 
			//file attrs have changed, also sent email of attr diff
			printf("File Changed!!");
			
			} 
		else {printf("Check passed %d : %d\n", cmon, fileStatNEW.st_mtime);

		
		}
	sleep(wait);
	}	


}

int main(int argc, char *argv[] ) {

//kyle w write parser for config file 
//kyle w write way to fork or thread this function below. IDK what would work better
// I think forking. He said in lecture forking was used to daemonize things
//we can discuss


monitorfile("test", 3);


}
