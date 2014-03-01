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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

//global temp storage directory to keep the files
char *tmpdir;
tmpdir="./tmpdir/";


void monitorfile(char *filename) {
        struct stat fileStat;
        if(stat(filename,&fileStat) < 0) {
                printf("Error: Cannot stat %s", filename);
                return 1;}
	char *path;
	strcpy(path, tmpdir);                
	strcat(path, filename);
	FILE *f;
	f = fopen(path, "w");
	fprintf(f,"Last Access: %s\n", fileStat.st_atime);	
	fprintf(f,"Last Modified: %s\n", fileStat.st_mtime);
	fprintf(f,"Last Status Change: %s\n", fileStat.st_ctime);
	fprintf(f,"Mode: %s\n", fileStat.st_mode);
	fprintf(f,"User ID: %s\n", fileStat.st_uid);
	fprintf(f,"Group ID:%s\n", fileStat.st_gid);
	fprintf(f,"Size: %s\n", fileStat.st_size);
	f = close(f);


}

int main(int argc, char *argv[] ) {

//kyle w write parser for config file 
//kyle w write way to fork or thread this function below. IDK what would work better
// I think forking. He said in lecture forking was used to daemonize things
//we can discuss


monitorfile("./tmpdir/test");


}
