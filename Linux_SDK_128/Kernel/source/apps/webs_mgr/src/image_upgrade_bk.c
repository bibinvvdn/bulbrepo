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


	system("/bin/tar -xvf /dev/shm/image.img -C /dev/shm/");
	//if(tar_image < 0){
	//printf("error in untar\n");
	//return 0;
	//}
	system("/bin/md5sum /dev/shm/uImage > /dev/shm/sum3");
	system("/bin/md5sum /dev/shm/rootfs.ext2.gz.uboot > /dev/shm/sum4");
	fprintf(stderr,"md5sum created for sum3 and sum4\n");
         system("/bin/cat /dev/shm/sum1");
	 system("/bin/cat /dev/shm/sum2");
	 system("/bin/cat /dev/shm/sum3");
	 system("/bin/cat /dev/shm/sum4");
	fprintf(stderr,"md5sum file opend\n");

	FILE *fp1,*fp2;
	fp1=fopen("/dev/shm/sum1","r");
	fp2=fopen("/dev/shm/sum3","r");
	fprintf(stderr,"file open for read for 1 and 3\n");
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
	fprintf(stderr,"file open for read for 2 and 4\n");
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
//	erase_mtd1=system("/usr/bin/flash_erase /dev/mtd1 0 0");
	sprintf(str,"flash_erase /dev/mtd1 0 0");
	system(str);
//	free(str);
	if(erase_mtd1 < 0){
	fprintf(stderr,"error in erasing partition\n");
	return 0;	
	}
	fprintf(stderr,"partition is erased\n");
//	write_uImage=system("/bin/dd if=/dev/shm/uImage of=/dev/mtd1 bs=512");
	sprintf(str,"dd if=/dev/shm/uImage of=/dev/mtd1 bs=512");
	system(str);
//	free(str);
	if(write_uImage < 0){
	fprintf(stderr,"error in flashing uImage\n");
	return 0;
	}
	fprintf(stderr,"uImage is flashed\n");
//	write_rootfs=system("/bin/dd if=/dev/shm/rootfs.ext2.gz.uboot of=/dev/mtd1 bs=512 seek=8192");
	sprintf(str,"dd if=/dev/shm/rootfs.ext2.gz.uboot of=/dev/mtd1 bs=512 seek=8192");
	system(str);
//	free(str);
	if(write_rootfs < 0){
	fprintf(stderr,"error in flashing rootfs\n");	
	return 0;
	}
	fprintf(stderr,"kernel is flashed\n");
	fprintf(stderr,"Image flashing success\n");
	return 1;
}
