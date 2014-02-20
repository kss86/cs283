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
int main(int argc, char *argv[] ) {









}
