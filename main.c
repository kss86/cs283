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
#include <diff.h>
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


void monitorfile(char *filename, int wait, char *email) {
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
		x = stat(filename,&fileStatNEW);
		 if(x < 0) {
                		printf("Error: Cannot stat %s :: error code: %i \n", filename, x);
               		 }

		 if (cmon != fileStatNEW.st_mtime) { 
			printf("File Changed!!\n");
			
			printf("Check NOT passed %d : %d\n", cmon, fileStatNEW.st_mtime);
			FILE *em = fopen("emtmp", "wb");
			fprintf(em, "%s %s %s", "To: ", email, "\r\n");
			fprintf(em, "From: filemonitor@somesite.com\r\n" );
			fprintf(em, "\r\n" );
			char *buff;
			strcpy(buff, "diff -y ");
			strcat(buff, filename);
			strcat(buff, "  ");
			strcat(buff, content);
			strcat(buff, " >> emtmp");
			printf("%s\n",buff);
			fclose(em);
			system(buff);
			monitorfile(filename, wait, email);

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

//foreach parsed line do; fork {
monitorfile("test", 1,"killerkyle113@gmail.com");
//}


}
