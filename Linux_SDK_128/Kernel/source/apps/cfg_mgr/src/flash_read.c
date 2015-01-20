#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int mac_read(char *mac)
{
        char *ptr;
        int ret, fd;
        FILE *fd1;

        fd = open("/dev/mtd3", O_RDWR);
		perror("OPEN");

        lseek(fd,4,SEEK_CUR);
		perror("LSeek");
        if(fd < 0)
                printf("error opening\n");
      
        ret = read(fd, mac, 17);
		perror("READ");
        if(ret<0)
        	printf("error writing\n");
        /* Need to close the file that we opened for reading */
        close (fd) ;
	return 0;
}

int hw_vrn_read(char *hd_vr)
{
        char *ptr;
        int ret, fd;
        FILE *fd1;

        fd = open("/dev/mtd3", O_RDWR);
		perror("OPEN");

        lseek(fd,22,SEEK_CUR);
		perror("LSeek");
        if(fd < 0)
                printf("error opening\n");
     
        ret = read(fd, hd_vr, 20);
		perror("READ");
        if(ret<0)
            printf("error writing\n");
        /* Need to close the file that we opened for reading */
        close (fd) ;
	return 0;
}

int sw_vrn_read(char *sw_vr)
{       
        char *ptr;
        int ret, fd;
        FILE *fd1;

        fd = open("/dev/mtd3", O_RDWR);
		perror("OPEN");

        lseek(fd,42,SEEK_CUR);
		perror("LSeek");
        if(fd < 0)
                printf("error opening\n");
      
        ret = read(fd,sw_vr,20);
		perror("READ");
        if(ret<0)
            printf("error writing\n");
        /* Need to close the file that we opened for reading */
        close (fd) ;
	return 0;
}
