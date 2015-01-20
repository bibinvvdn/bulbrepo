#ifndef __WFPS_API_COMMON_H__
#define __WFPS_API_COMMON_H__
/* Generic API command/Response Data Structure */
typedef struct cmd_interface
{
        unsigned short  module_id;      /* Id for module identification */
        unsigned short  cmd_opcode;     /* Command or response ID number */
        unsigned long   cmd_seq_number; /* Command sequence number */
        int             cmd_exec_status;/* Command execution status */
	unsigned short  cli_cmd_id;
#define API_CMD_PROCESSED       0
#define API_CMD_FAILED          1

        unsigned short  buff_length;    /* Buffer length */
        unsigned char   buff[0];                /* Command buffer */

} CMD_INTERFACE_T;

/*MODULE ID DEFINATION*/
#define MOD_ID_CFG_MGR 			0X1001	//CONFIGURATION MANAGER
#define MOD_ID_WDT_MGR 			0X1002  //WDT Manager
#define MOD_ID_CLI_MGR			0X1004  //CLI Manager
#define MOD_ID_WEB_SRV	                0x1008  //RSTP Application
#define MOD_ID_PRO_MGR			0x1010  //hostapd Application
#define MOD_ID_MAC_UPLD_MGR		0x1012  //MAC UPLOAD MANAGER

/* SOCKET NUMBERS FOR MODULES */
#define CFGM_PORT_NUM		MOD_ID_CFG_MGR
#define WDTM_PORT_NUM		MOD_ID_WDT_MGR
#define	CLIM_PORT_NUM		MOD_ID_CLI_MGR
#define WEBM_PORT_NUM           MOD_ID_WEB_SRV
#define PROXM_PORT_NUM          MOD_ID_PRO_MGR
#define MAC_UPLD_PORT_NUM	MOD_ID_MAC_UPLD_MGR

#define CFG_CMD_BASE 		0x0000
#define WEB_CMD_BASE 		0x0100
#define WDT_CMD_BASE 		0x0200
#define CLI_CMD_BASE		0x0300
#define PROXM_CMD_BASE		0x0400
#define MAC_UPLD_CMD_BASE	0x0600

#define CFGM_ERR_BASE		-(0x100)
#define WEB_ERR_BASE		-(0x200)
#define CLI_ERR_BASE		-(0x300)
#define MAC_UPLD_ERR_BASE	-(0x400)

#endif /*__WFPS_API_COMMON_H__*/
