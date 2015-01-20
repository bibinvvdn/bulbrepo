/*This file defines the various commands and argumnets for the nethra sys call driver*/
#ifndef NETHRA_SYS_USER_H
#define NETHRA_SYS_USER_H

#include<asm/ioctl.h> 
#define NETHRA_MAGIC_NUM	0xA1

#define NETHRA_IOC_SET_ALARM				_IOW(NETHRA_MAGIC_NUM, 0x02, unsigned int)	
#define NETHRA_IOC_SET_LED					_IOW(NETHRA_MAGIC_NUM, 0x04, unsigned int)
#define NETHRA_IOC_GET_SYSTEM_STATUS		_IOR(NETHRA_MAGIC_NUM, 0x08, unsigned int)
#define NETHRA_IOC_RESET_SYSTEM				_IOR(NETHRA_MAGIC_NUM, 0x0a, unsigned int)
#define NETHRA_IOC_ENTER_PMODE				_IOR(NETHRA_MAGIC_NUM, 0x0c, unsigned int)	
#define NETHRA_IOC_EXIT_PMODE				_IOR(NETHRA_MAGIC_NUM, 0x0e, unsigned int)
#define NETHRA_IOC_ENABLE_LED_TOGGLING		_IOR(NETHRA_MAGIC_NUM, 0x10, unsigned int)
#define NETHRA_IOC_DISABLE_LED_TOGGLING     _IOR(NETHRA_MAGIC_NUM, 0x12, unsigned int)

/*The Device would be available as: /dev/nethra-sys*/
/*The config information would be avilable through the proc interface at "driver/nethra-sys"*/

/*This is the start address of the config partition in the NOR Flash*/
/*This following structure and constants are also defined in the redboot code in file "/packages/hal/arm/mx27/ads/current/src/" and 
"packages/hal/redboot/current/cdl/redboot.cdl" in case any changes are made here same should be made at the corresponding place as well*/

#ifdef CONFIG_NETHRA_IPCAM
#define NETHRA_RO_CONFIG_FLASH_ADDR	(0xC0FA0000)
#define NETHRA_CONFIG_FLASH_ADDR	(0xC0FC0000)
#else
#define NETHRA_RO_CONFIG_FLASH_ADDR	(0xC1FA0000)
#define NETHRA_CONFIG_FLASH_ADDR	(0xC1FC0000)
#endif

#define MAGIC_NUM_STATE_CORR        0x00000BAD
#define MAGIC_NUM_STATE_OK          0x0000C00D

#define VER_INFO_SIZE			48
#define RO_RAW_SYS_CONFIG_SIZE     (6 + (VER_INFO_SIZE*9))

#if RO_RAW_SYS_CONFIG_SIZE > 512
#error Size of the RO Config Information Exceeds 512
#endif

/*Split the sys_config into two different parts
1. the ROsys_config : the read only sys config has all the deafult or the one time programmable or rarely changeable features 
2. the sys_config : this sys config will have all the frequently changing parameters.*/

/*The ROsys_config will have all the one time programmable parameters or the ones which are rarely re-programmed
 * we are keeping these on a seperate Segment on the flash to avoid any chances of corruption due to frequent rewriting 
 * of the flash segment*/
struct ROsys_config
{
    unsigned char   camera_serial_no[VER_INFO_SIZE];	/*Camera serial Number programmed in ASCII*/
    unsigned char   bsp_ver_no[VER_INFO_SIZE];			/*This will be the Linux Version Number programmed in ASCII*/
    unsigned char   firmware_ver_no[VER_INFO_SIZE];		/*This will refer to the control plane application etc version number*/
    unsigned char   nethra_fw_ver_no[VER_INFO_SIZE];	/*This is just for compatibility of the struct accross the platform*/
	unsigned char	manuf_daig_ver_no[VER_INFO_SIZE];	/*This will refer to the Manufacuring Diag Code Version for Linux Side, XML RPC side, GUI side*/
    unsigned char   hwr_ver_no[VER_INFO_SIZE];			/*This will refer to the PCB version number	collectively of Encoder Board and Capture Board*/
	unsigned char	encoder_board_ver[VER_INFO_SIZE];	/*This will refer to the Encoder Board Hardware Version Number*/
	unsigned char	capture_board_ver[VER_INFO_SIZE];	/*This will refer to the Capture Board Hardware Version Number*/
	unsigned char	power_board_ver[VER_INFO_SIZE];		/*This will refer to the Power Board Hardware Version Number*/	
    unsigned int    magic_num;                          /*This will mainly be used to check whether the configuration in flash is ok*/
    unsigned char   mac[6];                 			/*The MSB of the MAC is at location [5] of the array and LSB is at location [0]*/
    unsigned char   padding[512 - RO_RAW_SYS_CONFIG_SIZE];
};

#define RAW_SYS_CONFIG_SIZE     (12*4 + (VER_INFO_SIZE*4))
#if RAW_SYS_CONFIG_SIZE > 512
#error Size of the Config Information Exceeds 512
#endif

struct sys_config
{
    unsigned char   bsp_ver_no[VER_INFO_SIZE];			/*This will store the Secondary Linux Version Number*/
    unsigned char   firmware_ver_no[VER_INFO_SIZE];		/*This will store the Secondary Control Plane Application Number*/
    unsigned char   nethra_fw_ver_no[VER_INFO_SIZE];	/*This will store the Current Nethra Firmware Version Number*/
	unsigned char	manuf_daig_ver_no[VER_INFO_SIZE];	/*This will refer to the Manufacuring Diag Code Version for Linux Side, XML RPC side, GUI side*/
    unsigned int    post_level;     					/*for now the values can be 0,1,2, or 3;*/
    unsigned int    img_num;                			/*A value 1 here would choose the failsafe image and a value 2 here will choose the upgraded image*/
    unsigned int    magic_num;              			/*this serves multiple purposes from detecting a corrupt firmware etc to help in image upgradation*/
	unsigned int	boot_mode;							/*This parameter will decide which mode we are booting in encoder_board_test_mode(0x01) , burn_in_test_mode(0x02) or 0x00 Normal Mode*/
    unsigned int    iteration;                          /*This will be used to keep track of the Booting Iterations in case of Test Mode and can be used other wise also*/
    unsigned int    test_status;                        /*This will store the final test status of the different tests in Bits*/
    unsigned int    post_mem_addr_line_test;
    unsigned int    post_mem_data_line_test;
    unsigned int    post_mem_pattern_test;
    unsigned int    post_fec_mac_loopback_test;
    unsigned int    post_fec_phy_loopback_test;
    unsigned int    post_fec_external_loopback_test;
    unsigned char   padding[512 - RAW_SYS_CONFIG_SIZE];
}; 
/*<=================== Above changes in reboot also */

#if 0
Commenting this out to split the sys_config into two different parts
1. the ROsys_config : the read only sys config has all the deafult or the one time programmable or rarely changeable features 
2. the sys_config : this sys config will have all the frequently changing parameters.
#define VER_INFO_SIZE			48
#define RAW_SYS_CONFIG_SIZE     (3*4 + 6 + (VER_INFO_SIZE*5))

struct sys_config
{
	unsigned char	camera_serial_no[VER_INFO_SIZE];
	unsigned char	bsp_ver_no[VER_INFO_SIZE];
	unsigned char	firmware_ver_no[VER_INFO_SIZE];
	unsigned char	nethra_fw_ver_no[VER_INFO_SIZE];/*This is just for compatibility of the struct accross the platform*/
	unsigned char	hwr_ver_no[VER_INFO_SIZE];
	unsigned int	post_level; 	/* for now the values can be 0,1,2, or 3;*/
	unsigned int	img_num; 		/*A value 1 here would choose the failsafe image and a value 2 here will choose the upgraded image*/	
	unsigned int	magic_num;		/*this serves multiple purposes from detecting a corrupt firmware etc to help in image upgradation*/
	unsigned char	mac[6];			/*The MSB of the MAC is at location [5] of the array and LSB is at location [0]*/
	unsigned char	padding[512 - RAW_SYS_CONFIG_SIZE];
};
/*<=================== Above changes in reboot also */
#endif

/*ioctl function prototype*/
/* int nethra_sys_ioctl(struct inode *inode, struct file *file,
           unsigned int cmd, unsigned long arg);
*/
/*SET ALARM: This command will set the alarm output signal*/

/*SET LED: You need to pass a pointer to the value as the argument*/
#define LED_ORANGE	0x00
#define LED_RED		0x01
#define LED_GREEN	0x02
#define LED_OFF		0x03	

/*LED toggling*/
#define ORANGE_GREEN    0x01
#define ORANGE_RED      0x02
#define ORANGE_OFF      0x03
#define GREEN_RED       0x04
#define GREEN_OFF       0x05
#define RED_OFF         0x06

/*The toggling time here would be in 10ms multiples and the color would be a color and color/OFF state pair that need to be toggled*/
struct toggle_led {
	unsigned char color;
	unsigned char interval;
};

/*NETHRA_IOC_GET_SYSTEM_STATUS: A pointer to unsigned in would be passed
The system will need to wait on a select call on the device, once the select call returns the User Application need to issue a 
NETHRA_IOC_GET_SYSTEM_STATUS commmand, this command will return the status whether it was a alarm 1 trigger, alarm 2 trigger a 
press on the reset or a short press on the reset switch, after discovering the status the API can take appropriate action
The status would be retruned as follows
if STATUS	= 0x00000001: It is a short press on reset switch
			= 0x00000002: It is a long press on reset switch
			= 0x00000004: Alarm 1 triggered
			= 0x00000008: Alarm 2 triggered
*/

#endif /*NETHRA_SYS_USER_H*/
