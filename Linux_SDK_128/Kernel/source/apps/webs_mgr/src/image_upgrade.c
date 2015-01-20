#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{

upgrade();

}

int upgrade()
{

	char cmp1[32],cmp2[32],cmp3[32],cmp4[32];
	int erase_mtd1,write_uImage,write_rootfs;
	int tar_image,sum1_read,sum2_read,sum3_read,sum4_read;
/* image.img file untar function */
	tar_image=system("/bin/tar -xvf /dev/shm/image.img -C /dev/shm/");
		if(tar_image < 0){
		fprintf(stderr,"Error in untar of image.img\n");
		return 0;
		}
		else {
		fprintf(stderr,"File image.img , untar successfully\n");
		}

	system("/bin/md5sum /dev/shm/uImage > /dev/shm/sum3");
	system("/bin/md5sum /dev/shm/rootfs.ext2.gz.uboot > /dev/shm/sum4");
	fprintf(stderr,"md5sum created of uImage(sum3) and root file system(sum4)\n");
         
	system("/bin/cat /dev/shm/sum1");
	system("/bin/cat /dev/shm/sum2");
	system("/bin/cat /dev/shm/sum3");
	system("/bin/cat /dev/shm/sum4");
	fprintf(stderr,"All md5sum files opend for checksum\n");

	FILE *fp1,*fp2;
	fp1=fopen("/dev/shm/sum1","r");
	fp2=fopen("/dev/shm/sum3","r");
	fprintf(stderr,"files open for reading of uImage md5sum(sum1 and sum3)\n");
		if(sum1_read=(fread(cmp1,32,1,fp1)) < 0){
		fprintf(stderr,"error in reading sum1\n");
		}
		if(sum3_read=(fread(cmp2,32,1,fp2)) < 0){
		fprintf(stderr,"error in reading sum3\n");
		}
		if(0 != strncmp(cmp1,cmp2,32)){
		fprintf(stderr,"error in uImage\n");
		}
	fclose(fp1);
	fclose(fp2);
	
	fp1=fopen("/dev/shm/sum2","r");
	fp2=fopen("/dev/shm/sum4","r");
	fprintf(stderr,"files open for reading of root file system md5sum(sum2 and sum4)\n");
		if(sum2_read=(fread(cmp3,32,1,fp1)) < 0){
		fprintf(stderr,"error in reading sum2\n");
		}
		if(sum4_read=(fread(cmp4,32,1,fp2)) < 0){
		fprintf(stderr,"error in reading sum4\n");
		}
		if(0 != strncmp(cmp3,cmp4,32)){
		fprintf(stderr,"error in rootfs.ext2.gz.uboot\n");
		}
	fclose(fp1);
	fclose(fp2);

	char *str;
        str = (malloc(sizeof(char)*200));

/*flash erase command*/
	//erase_mtd1=system("/usr/bin/flash_erase /dev/mtd1 0 0");
	fprintf(stderr,"flash erase start of /dev/mtd1\n");
	sprintf(str,"flash_erase /dev/mtd1 0 0");
	erase_mtd1=system(str);
	//free(str);
		if(erase_mtd1 < 0){
		fprintf(stderr,"error in erasing partition\n");
		return 0;	
		}
		else {
		fprintf(stderr,"partition /dev/mtd1 is erased\n");
		}

/*copying uImage in flash*/
	//write_uImage=system("/bin/dd if=/dev/shm/uImage of=/dev/mtd1 bs=512");
	fprintf(stderr,"Copying uImage in /dev/mtd1\n");
	sprintf(str,"dd if=/dev/shm/uImage of=/dev/mtd1 bs=512");
	write_uImage=system(str);
	//free(str);
		if(write_uImage < 0){
		fprintf(stderr,"error in flashing uImage\n");
		return 0;
		}
		else {
		fprintf(stderr,"uImage flashed\n");
		}

/*copying root file system in flash*/
	//write_rootfs=system("/bin/dd if=/dev/shm/rootfs.ext2.gz.uboot of=/dev/mtd1 bs=512 seek=8192");
	fprintf(stderr,"Copying root file system in /dev/mtd1\n");
	sprintf(str,"dd if=/dev/shm/rootfs.ext2.gz.uboot of=/dev/mtd1 bs=512 seek=8192");
	write_rootfs=system(str);
	//free(str);
		if(write_rootfs < 0){
		fprintf(stderr,"error in flashing rootfs\n");	
		return 0;
		}
		else {
		fprintf(stderr,"kernel flashed\n");
		fprintf(stderr,"Image flash success\n");
		}
	return 1;
}
