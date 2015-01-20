/*
*    @file     cfg_mgr.c
*
*    @brife    This file contain the initialization of the config manager
*    
*    @author   VVDN TECHNOLOGIES
*      
*    @license  Copyright (C) 2011 
*              This program is free software; you can redistribute it and/or modify it
*              under the terms of the GNU General Public License version 2 as
*              published by the Free Software Foundation.  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "common_header.h"

#define COPPER	0
#define FIBER	1
//#define HOSTAPD_CONF_FILE_NAME	"/etc/hostapd_3.conf"
//#define AP_INTERFACE 		"wlan0"
#define AP_DRIVER 		"nl80211"
#define AP_HW_MODE 		"g"
#define AP_AUTH_ALGS 		1
#define AP_MAC_ACL 		0
#define NW_DHCP_AUTO_IP_ENABLE	1

int get_client_channel ();


WFPS_DATABASE_T wfps_database;
cmgr_sock_fd_t      g_connection_id;
int board_in=0;
typedef unsigned char U8;
static unsigned int channel;


typedef struct 
{
        uint8_t channel;
        int8_t  rssi;
        int16_t timestamp[4];
        uint8_t src[6];

}probe;



/*
*       @func    wdt_keepalive_handler
*        
*       @param   *p_buf
*       @*p_buf  pointer to keepalive massage
*	
*	@brief   It processed the WDT keepalive request signal 
*       
*       @return  cfg_return_t type failure or success massage
*/      

cfg_return_t wdt_keepalive_handler(void *p_buf)
{
    CMD_INTERFACE_T     *p_api_hdr = (CMD_INTERFACE_T*)p_buf;
    CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
    cmgr_ecode_t        cmgr_error;
    char                *p_send_buffer;
    cfg_return_t        retval = CFGM_SUCCESS;


 p_send_buffer = (char*) malloc(sizeof(CMD_INTERFACE_T));
    if(NULL == p_send_buffer){
        debug(CFG_ERROR,"Memory allocation failure\n\n");
        return CFGM_ERR_END;
    }

    //Copy the original meesage first, then update the parameters required
    memcpy((void*)p_send_buffer, p_buf, sizeof(CMD_INTERFACE_T));

    p_tx_api_hdr = (CMD_INTERFACE_T*)p_send_buffer;
    p_tx_api_hdr->cmd_opcode = CMD_WD_RES_KEEPALIVE;
    p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;
    p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;

    //Send message on the network
    if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                (cmgr_s8bit*)p_tx_api_hdr, sizeof(CMD_INTERFACE_T), p_api_hdr->module_id,
                &cmgr_error))
    {
        debug(CFG_ERROR,"Send message to MOD_ID_WDT failed\n\n");
        retval = CFGM_ERR_END;
    }

    //Free the API buffer
    free(p_send_buffer);
    free(p_buf);
    return retval;
}


/*
*       @func    reboot_notification
*       
*       @parm    *p_buf
*       @*p_buf  pointer to reboot notification massage
*
*	@brief   It processed the WDT reboot signal 
*       
*       @return  cfg_return_t type success or fail massage
*/

cfg_return_t reboot_notification(void *p_buf)
{
    CMD_INTERFACE_T     *p_api_hdr = (CMD_INTERFACE_T*)p_buf;
    CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
    cmgr_ecode_t        cmgr_error;
    unsigned int        api_size = p_api_hdr->buff_length;
    char                *p_send_buffer;
    cfg_return_t        retval = CFGM_SUCCESS;
/*
    if(g_image_upgrade_ongoing) //firmware upgrade is ongoing, hence dropping the API now
    {
        EM_TRACE(LOG_DEBUG, "Image upgrade is ongoing, dropping the reboot notification now...\n");
        __DEMALLOC__(p_buf);
        return retval;
    }
*/
    p_send_buffer = (char*) malloc(api_size);
    if(NULL == p_send_buffer)
    {
        debug(CFG_ERROR,"Memory allocation failure\n\n");
       
       free(p_buf);
        return CFGM_ERR_END;
    }

    //Copy the original meesage first, then update the parameters required
    memcpy((void*)p_send_buffer, p_buf, api_size);

    p_tx_api_hdr = (CMD_INTERFACE_T*)p_send_buffer;
    p_tx_api_hdr->cmd_opcode = CMD_WD_RES_SAVE_AND_CLOSE;
    p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;
    p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;

    //Send message on the network
    if(CMGR_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                (cmgr_s8bit*)p_tx_api_hdr, api_size, p_api_hdr->module_id,
                &cmgr_error))
    {
        debug(CFG_WARN, "Send message to MOD_ID_TDMOIP_APPL failed\n\n");
     
        retval = CFGM_ERR_END;
    }

    //Free the API buffer
    free(p_send_buffer);
    free(p_buf);
    return retval;
}

/*
*       @func     cfgm_process_msg_frm_nw
*       
*       @parm     *p_buf
*       @*p_buf   pointer to the remort massage comming from the socket
*
*	@brief    It handler massage comming from different module 
*       
*       @return   cfg_return_t type success or fail massage
*/
cmgr_return_t cfgm_process_msg_frm_nw(cmgr_s8bit * p_buf, cmgr_s16bit buff_len,
                                    cmgr_ip_addr_st rem_srcaddr,
                                    cmgr_sock_fd_t conn_id,
                                    cmgr_ecode_t * p_ecode)
{
        CMD_INTERFACE_T     *p_api_buf=(CMD_INTERFACE_T*)p_buf;
        p_ecode=NULL;

        /*printf("[cfg_mgr] cmd rec in cfg mgr= %s\n",p_api_buf->buff);*/
	int qos_loop = 0; 
	FILE *fp;

	 probe pr;

        if (MOD_ID_WDT_MGR != p_api_buf->module_id)
                debug(CFG_INFO,"recieving mesg from %04x\n",rem_srcaddr.port);

        switch(p_api_buf->cmd_opcode)
        {
		case WEB_CFGM_SYS_CFG_GET:
			printf("[CFG_MGR] Called Get_Sys_Cfg_page()\n");
			Get_Sys_Cfg_page();
			break;
		case WEB_CFGM_SYS_CFG_SET:
                        printf("[CFG_MGR] Called Set_Sys_Cfg_page()\n");
                        Set_Sys_Cfg_page((SYS_CFG_T *)p_api_buf->buff);
                        break;
		case WEB_CFGM_NW_CFG_GET:
                        printf("[CFG_MGR] Called Get_Nw_Cfg_page()\n");
                        Get_Nw_Cfg_page();
                        break;
		case WEB_CFGM_NW_CFG_SET:
                        printf("[CFG_MGR] Called Set_Nw_Cfg_page()\n");
                        Set_Nw_Cfg_page((NW_CFG_T *) p_api_buf->buff);
                        break;
                case WEB_CFGM_WLAN_CFG_GET:
                        printf("[CFG_MGR] Called Get_Wlan_Cfg_page()\n");
                        Get_Wlan_Cfg_page();
                        break;
                case WEB_CFGM_WLAN_CFG_SET:
                        printf("[CFG_MGR] Called Set_Wlan_Cfg_page()\n");
                        Set_Wlan_Cfg_page((WLAN_CFG_T *)p_api_buf->buff);
                        break;
		case MAC_UPLD_CFGM_SRV_CFG_GET:
                        printf("[CFG_MGR] Called Get_Srv_Cfg_page()\n");
                        Get_Srv_Cfg_page(MOD_ID_MAC_UPLD_MGR);
                        break;
		case WEB_CFGM_SRV_CFG_GET:
                        printf("[CFG_MGR] Called Get_Srv_Cfg_page()\n");
                        Get_Srv_Cfg_page(MOD_ID_WEB_SRV);
                        break;
                case WEB_CFGM_SRV_CFG_SET:
                        printf("[CFG_MGR] Called Set_Srv_Cfg_page()\n");
                        Set_Srv_Cfg_page((SERVER_CFG_T *)p_api_buf->buff);
                        break;
		case WEB_CFGM_TPROF_CFG_GET:
                        printf("[CFG_MGR] Called Get_TProf_Cfg_page()\n");
                        Get_TProf_Cfg_page();
                        break;
                case WEB_CFGM_TPROF_CFG_SET:
                        printf("[CFG_MGR] Called Set_TProf_Cfg_page()\n");
                        Set_TProf_Cfg_page((TPROFILE_CFG_T *)p_api_buf->buff);
                        break;
		case WEB_CFGM_MAC_LIST_GET:
                        printf("[CFG_MGR] Called Get_Mac_List_page()\n");
                        Get_Mac_List_page((MAC_LIST_T *)p_api_buf->buff);
                        break;
                case WEB_CFGM_MGT_CFG_SET:
                        printf("[CFG_MGR] Called Set_Mgt_Cfg_page()\n");
                        Set_Mgt_Cfg_page((MGMT_CFG_T *)p_api_buf->buff);
                        break;
		case WEB_CFGM_NW_STATS_GET:
			printf("[CFG_MGR] Called Get_Net_Stats_page()\n");
			Get_Net_Stats_page();
			break;
		case WEB_CFGM_SCAN_LIST_GET:
                        printf("[CFG_MGR] Called Get_Scan_List_page()\n");
                        Get_Scan_Cfg_page((SCAN_CFG_T *)p_api_buf->buff);
			break;
		case WEB_CFGM_BULBSTATS_CFG_SET:
			printf("[CFG_MGR] Called Set_bulbstats_List_page()\n");
			Set_Bulb_Cfg_page((BULBSTATS_CFG_T *)p_api_buf->buff);
			break;
		case WEB_CFGM_BLINK_CFG_SET:
			printf("[CFG_MGR] Called Set_blink_List_page()\n");
			Set_Blink_Cfg_page((BLINK_CFG_T *)p_api_buf->buff);
			break;
		case WEB_CFGM_POWER_CFG_GET:
			printf("[CFG_MGR] Called Get_power_Cfg_page()\n");
			Get_Power_Cfg_page((POWER_CFG_T *)p_api_buf->buff);
			break;
		case WEB_CFGM_BLACK_CFG_GET:
			printf("[CFG_MGR] Called Get_black_Cfg_page()\n");
			Get_Black_Cfg_page((BLACK_CFG_T *)p_api_buf->buff);
			break;
		 case WEB_CFGM_POLL_CFG_GET:
                        printf("[CFG_MGR] Called Get_poll_Cfg_page()\n");
                        Get_Poll_Cfg_page((POLL_CFG_T *)p_api_buf->buff);
                        break;
		case PRO_CFGM_TEST:
			fp=fopen("/mnt/maclist.txt","a");
			memcpy(&pr,&(p_api_buf->buff), sizeof(probe));
			fwrite(&pr,sizeof(probe),1,fp);
                     //   printf("rate:%u channel:%u rssi:%d timestamp:%u:%u:%u:%u mac:%x:%x:%x:%x:%x:%x\n",pr.rate,pr.channel,pr.rssi,pr.timestamp[0],pr.timestamp[1],pr.timestamp[2],pr.timestamp[3],pr.src[0],pr.src[1],pr.src[2],pr.src[3],pr.src[4],pr.src[5]);
			wfps_database.sys_cfg_t.total_mac_count +=1;
			wfps_db_write_cfg_2_file();
			fclose(fp);
		case CMD_WD_REQ_KEEPALIVE: 
			debug(CFG_INFO,"recieving mesg from %04x\n",rem_srcaddr.port);
			wdt_keepalive_handler(p_buf);
			break;
		 case WEB_CFGM_IMAGE_UPGRADE:

                     printf("[CFG_MGR] Called upgrade\n");

                     break;
		case CMD_WD_SAVE_AND_CLOSE:
			printf("Received reboot notification from WD- CFG MGR\n");
			reboot_notification(p_buf);
			break;
		case CMD_WD_RES_BOOT_STATUS:
			//showbootstatus(p_api_buf->buff);
			// cfg_response_cmd(CLI_GENERAL_SHOW_BOOT_STATUS,p_api_buf->buff);
			break;
#if 0
                case CLI_CFGM_RECV_INCOMING:
			resp =1;
                        mgr_opcode_info = CLI_CFGM_RECV_INCOMING;
                        debug(CFG_INFO,"[cfg_mgr] in handler fn recving msg from %04x\n",rem_srcaddr.port);
                        if(CFGM_SUCCESS != process_cmd("CLI Manager",p_api_buf->buff)){
                             debug(CFG_ERROR,"PROCESS COMMAND FAILED\n");
                             return CMGR_FAILURE;
                        }
                        break;
#endif                
		default:
                        debug(CFG_ERROR,"invalid msg\n");
                        return CMGR_FAILURE;
        }
        
        debug(CFG_DEBUG,"Processing Cfg Mgr Msg\n");
        return CMGR_SUCCESS;
}

/*
*       @func      cfgm_timeout_mesg
*
*       @brief     handl cfg time out funtion
*
*       @parm      *p_ecode
*	@*p_ecode  pointer to error handler 
*       
*       @return    cmgr_return_t success or fail massage
*/
cmgr_return_t cfgm_process_timeout_mesg (cmgr_ecode_t * p_ecode)
{
        
        debug(CFG_DEBUG,"Processing Cfg Mgr timeout\n");
	signed int ret_temp,ret_psu1,ret_psu2;
	static signed int temp=0, psu1 =0, psu2 =0,t=0,indx =1;
	static int temp_low_alarm_state=1,temp_high_alarm_state=1,temp_high_clear=1,temp_low_clear=1;
	if(board_in){
#if 0
		if(CFGM_SUCCESS != cfgm_temp_sensor_init())
                {
                	debug(CFG_WARN, "Temperature Sensor initialisation failed\n");
                	CFGM_RAISE_ALARM(ALM_ID_TEMPERATURE, ALM_LVL_CRITICAL, 0,0, 0, "Temperature Sensor initialisation failed");
                	return CFGM_ERR_END;
                }
#endif
	}

	return CMGR_SUCCESS;
}

/**
 *    @func   get_client_channel
 * 
 *    @Param  None
 *
 *    @brief  Function get the client connected channel if Associated otherwise gives defaulr channnel "1".
 * 
 *    @return - 0 for success.
 */

int get_client_channel () 
{
#define CMD1 "iwconfig wlan1 > channel1.txt"
#define RM_CMD1 "rm channel1.txt"
	FILE *fd;
	char *freq;	
        system(CMD1);

        if ((fd = fopen("channel1.txt","r")) != NULL) {
                char line [ 128 ]; /* or other suitable maximum line size */
                fgets(line,sizeof line, fd);
                if ( fgets ( line, sizeof line, fd ) != NULL ) /* read a line 2*/
                {
                        if((strstr(line,"Access Point: Not-Associated")) == NULL) {
                                strtok(line," ");
                                strtok(NULL,":");
                                freq = strtok(NULL," ");
                                if( ! (strncmp(freq,"2.412",5))) {
                                        channel = 1;
                                }else if( ! (strncmp(freq,"2.417",5))) {
                                        channel = 2;
                                }else if( ! (strncmp(freq,"2.422",5))) {
                                        channel = 3;
                                }else if( ! (strncmp(freq,"2.427",5))) {
                                        channel = 4;
                                }else if( ! (strncmp(freq,"2.432",5))) {
                                        channel = 5;
                                }else if( ! (strncmp(freq,"2.437",5))) {
                                        channel = 6;
                                }else if( ! (strncmp(freq,"2.442",5))) {
                                        channel = 7;
                                }else if( ! (strncmp(freq,"2.447",5))) {
                                        channel = 8;
                                }else if( ! (strncmp(freq,"2.452",5))) {
                                        channel = 9;
                                }else if( ! (strncmp(freq,"2.457",5))) {
                                        channel = 10;
                                }else if( ! (strncmp(freq,"2.462",5))) {
                                        channel = 11;
                                }else if(! (strncmp(freq,"2.467",5))) {
                                        channel = 12;
                                }else {         // if(! (strncmp(freq,"2.472",4))) {
                                        channel = 13;
                                }

                        } else {
                                channel = 1;
                        }
                }
                fclose(fd);
                system(RM_CMD1);
        } else {
                debug(CFG_ERROR,"file open failure\n\n");
                fclose(fd);
         	return -1;      
	}
	return 0;
}


/**
 *    @func   cfgm_nw_init
 * 
 *    @Param  None
 *
 *    @brief  Function responsible for configuring the system network based on the values in the databse
 * 
 *    @return cfg_return_t - Parameter to indicate the return value of function.
 */
cfg_return_t cfgm_nw_init()
{
        debug(CFG_TRACE,"configuring the system network from database \n");
	char pri_if_name[16], str_route[100];
        unsigned char ip_addr[16]={'\0'}, net_mask_addr[16]={'\0'}, gw_addr[16]={'\0'};
	in_addr_t    ip_address, ip_subnet_mask, ip_gateway;
	int count;

	/*puting address into local variable from database */
	sprintf(ip_addr,"%s",wfps_database.nw_cfg_t.ip_addr);
	sprintf(net_mask_addr,"%s",wfps_database.nw_cfg_t.netmask);
	sprintf(gw_addr,"%s",wfps_database.nw_cfg_t.gateway);

	 
        debug(CFG_INFO,"ip address %s\n",ip_addr);
        debug(CFG_INFO,"net mask address %s\n",net_mask_addr);
        debug(CFG_INFO,"gate way address %s\n",gw_addr);
	
	get_managment_if_name(pri_if_name);
//to remove autoip

//	if(wfps_database.nw_cfg_t.dhcp == NW_DHCP_AUTO_IP_ENABLE){
//		 dhcp_start(pri_if_name);
//	}
//	else{
	    //Set the IP Address
	    ip_address = inet_addr((const char*)(ip_addr));
	    if((0 == ip_address) || (-1 == ip_address))
	    {
            	debug(CFG_ERROR, "Invalid IP Address.\n");
        	return CFGM_INIT_ERROR;
    	    }
    	    if( 0 > set_ipaddress(pri_if_name, ip_address))
    	    {
    	        debug(CFG_WARN, "Unable to configure the IP Address.\n");
    	        return CFGM_INIT_ERROR;
    	    }

	    //set net mask address
	    ip_subnet_mask = inet_addr((const char*)(net_mask_addr));

            if((0 == ip_subnet_mask) || (-1 == ip_subnet_mask))
            {
                debug(CFG_ERROR, "Invalid MET MASK Address.\n");
                return CFGM_INIT_ERROR;
            }
            if( 0 > set_netmask(pri_if_name, ip_subnet_mask))
            {
    	        debug(CFG_WARN, "Unable to configure the NET MASK Address.\n");
    	        return CFGM_INIT_ERROR;
            }

	    //set gateway address
	    ip_gateway = inet_addr((const char*)(gw_addr));

            if((0 == ip_gateway) || (-1 == ip_gateway))
            {
	        debug(CFG_ERROR, "Invalid GATEWAY Address.\n");
                return CFGM_INIT_ERROR;
            }
#if 0
	    sprintf(str_route, "del default gw %s", gw_addr);

           if( 0 > configure_route(str_route))
           {
                debug(CFG_WARN, "Unable to delete the gateway. if exist\n");
                return CFGM_INIT_ERROR;
            }
#endif
            sprintf(str_route, "add default gw %s", gw_addr);

            if( 0 > configure_route(str_route))
            {
                debug(CFG_WARN, "Unable to configure the gateway.\n");
                return CFGM_INIT_ERROR;
            }
//	}
	system("ifconfig lo up ");	

return CFGM_SUCCESS;
}


/**
 *    @func   cfgm_wlan_init
 * 
 *    @Param  None
 *
 *    @brief  Function responsible for configuring the system wireless settings based on the values in the databse
 * 
 *    @return cfg_return_t - Parameter to indicate the return value of function.
 */
#if 0
cfg_return_t cfgm_wlan_init()
{
        debug(CFG_TRACE,"configuring the system wireless parameters from database \n");
	char ap_ssid[32]= {'\0'};
	char sta_ssid[32]= {'\0'};
	char wpa_pass[32]={'\0'};
	char wep_key[32]={'\0'};
	char pri_if_name[16];
	char *str;
        unsigned int ap_chnl;
	unsigned char sta_tx_pwr;
	FILE *ap;
	
	get_managment_if_name(pri_if_name);
	str = malloc(100*sizeof(char));
	sprintf(sta_ssid,"%s",wfps_database.wlan_cfg_t.sta_ssid);
	sprintf(wpa_pass,"%s",wfps_database.wlan_cfg_t.sta_wpa_secret);
	sprintf(wep_key,"%s",wfps_database.wlan_cfg_t.sta_wep_secret);
	wep_num=wfps_database.wlan_cfg_t.sta_wep_keyno;		
	wpa_enc=wfps_database.wlan_cfg_t.sta_wpa_enc;		
	/*replace ssid information obtained from database */
	system("/usr/sbin/ssid");

	if((wpa_pass[0] == '\0') && (wep_key[0] == '\0'))        // check for preconfigured wpa and wep values and start wpa_supplicant 
	{                                                        //  and connect to open network if no default configuration
		printf("connecing to WPA/WPA2\n");
		memset(str,0,sizeof(str));
		sprintf(str,"/usr/local/bin/wpa_supplicant -B -Dwext -i%s -c/etc/wpa_supplicant.conf",pri_if_name);
		system(str);
	}
	/*check for preconfigured wpa passphrase and start wpa supplicant to connect to wpa secure access point */
	else if(wpa_pass[0] != '\0')                                    
	{
		if(wpa_enc == 0 )
		{
			system("echo pairwise=CCMP > /etc/enc.conf");
			system("echo group=CCMP >> /etc/enc.conf");
		}
		else if(wpa_enc == 1)
		{ 
			system("echo pairwise=TKIP > /etc/enc.conf");
			system("echo group=TKIP >> /etc/enc.conf");
		}
		else if(wpa_enc == 2)
		{ 
			system("echo pairwise=CCMP > /etc/enc.conf");
			system("echo group=CCMP >> /etc/enc.conf");
		}
		/*obtain psk using ssid and wpa passphrase */
		sprintf(str,"/usr/local/bin/wpa_passphrase %s %s > /etc/wpa.conf",sta_ssid,wfps_database.wlan_cfg_t.sta_wpa_secret);
		printf("writing to wpa.conf\n");
		system(str);
		memset(str,0,sizeof(str));

		printf("deleting psk\n");
		system("sed -i '/pairwise/d' ./etc/wpa_supplicant.conf");
		system("sed -i '/group/d' ./etc/wpa_supplicant.conf"); 
		system("sed -i '/psk/d' ./etc/wpa_supplicant.conf");
		/* replace pairwise ,group and psk information */
		system("/usr/sbin/replace");

		printf("CONNECTING TO WPA/WPA2\n");
		memset(str,0,sizeof(str));
		sprintf(str,"/usr/local/bin/wpa_supplicant -B -Dwext -i%s -c/etc/wpa_supplicant.conf",pri_if_name);
		system(str);
	}
	/*check for preconfigured wep secret key and start connecting to wep secure access point */
	else if(wep_key[0] != '\0')
	{

		memset(str,0,sizeof(str));
		sprintf(str,"iwconfig %s key %s essid %s",pri_if_name,wep_key,sta_ssid);
		system(str);
	}

	sta_tx_pwr = wfps_database.wlan_cfg_t.sta_tx_pwr;
	//	sprintf(str,"iwconfig %s essid %s ",pri_if_name,sta_ssid);
	sleep(10);
	//	sprintf(str,"iwconfig %s txpower %u ",pri_if_name,sta_tx_pwr);
	free(str);
	
	/*puting address into local variable from database */
	sprintf(ap_ssid,"%s",wfps_database.wlan_cfg_t.ap_ssid);

 	if ( 0 > get_client_channel ()) 
	{
		debug(CFG_WARN, "Unable to get the client channel.\n");
                return CFGM_INIT_ERROR;
	}
	ap_chnl = channel;
        debug(CFG_INFO,"ap ssid %s\n",ap_ssid);
        debug(CFG_INFO,"ap channel %u\n",ap_chnl);

	if ((ap = fopen(HOSTAPD_CONF_FILE_NAME,"w")) != NULL) {
		fprintf(ap,"interface=%s\n",AP_INTERFACE);           
		fprintf(ap,"driver=%s\n",AP_DRIVER);           
		fprintf(ap,"hw_mode=%s\n",AP_HW_MODE);           
		fprintf(ap,"auth_algs=%d\n",AP_AUTH_ALGS);           
		fprintf(ap,"macaddr_acl=%d\n",AP_MAC_ACL);           
		fprintf(ap,"channel=%u\n",ap_chnl);           
		fprintf(ap,"ssid=%s\n",ap_ssid);
		fclose(ap);
	}else {
                debug(CFG_ERROR,"hostapd conf. file opening failure\n\n");
		return -1;
	}

	system("/usr/local/bin/hostapd /etc/hostapd_1.conf -B");
	return CFGM_SUCCESS;
}

#endif
cfg_return_t cfgm_fru_read()
{	
	char mac[18];
        mac[17]='\0';
	char hd_vr[20];
	char sw_vr[20];
//	char *str;
//	char pri_if_name[16];

//        get_managment_if_name(pri_if_name);

//        str = malloc(100*sizeof(char));

	mac_read(mac);
	hw_vrn_read(hd_vr);
	sw_vrn_read(sw_vr);
	printf("mac is %s\n",mac);
	printf("hw is %s\n",hd_vr);
	printf("sw is %s\n",sw_vr);

	strcpy(wfps_database.sys_cfg_t.mac_addr,mac);
	strcpy(wfps_database.sys_cfg_t.hw_sno,hd_vr);
	strcpy(wfps_database.sys_cfg_t.sw_ver,sw_vr);

//        sprintf(str,"ifconfig %s hw ether %s ",pri_if_name,mac);
//	system(str);

	wfps_db_write_cfg_2_file();  //write to flash
 	
//	free(str);
        return CFGM_SUCCESS;
}


/*
*    @func  main
*
*    @brief  Function for initialization of Configure Manager
*/
int main(int argc, char *argv[])
{
	FILE *fp;
        cmgr_ecode_t    cmgr_error;
        cmgr_ip_addr_st     local_ip_addr, remote_ip_port;
	/*Initialize DB*/
	wfps_db_config_db_init();
	debug(CFG_DEBUG,"[cfg_mgr] after wfps db init in cfg\n");
/*			#ap	

	if(0 > sethostname(wfps_database.sys_cfg_t.hostname,(sizeof(wfps_database.sys_cfg_t.hostname)))){
		debug(CFG_ERROR,"Setting hostname failure\n\n");
			
	}
#if 0
	if(CFGM_SUCCESS != cfgm_fru_read())
	{
		debug(CFG_WARN, "FRU read failed\n");
		return CFGM_ERR_END;
	}
#endif

*/
#if 1        
	/*Assign each configuration settings*/
	if(CFGM_SUCCESS != cfgm_nw_init())
	{
		debug(CFG_WARN, "Network initialisation failed\n");
		return CFGM_ERR_END;
	}
#endif 
/*
#if 1

	if(CFGM_SUCCESS != cfgm_wlan_init())
	{
		debug(CFG_WARN, "Wireless initialisation failed\n");
		return CFGM_ERR_END;
	}
#endif
*/		/* ap */


	debug(CFG_DEBUG,"[cfg_mgr] going to init socket \n");
	/*Initialize IPC*/
	if(CFGM_SUCCESS != cmgr_init_conn_lib(MOD_ID_CFG_MGR, cfgm_process_msg_frm_nw, 
                cfgm_process_timeout_mesg, (cmgr_s8bit*)"Configuration Manager", 
                &cmgr_error))
        {
                debug(CFG_ERROR,"IPC Connection initialisation failed\n");
                return CFGM_CONN_MGR_INIT_ERROR;        
        }

        local_ip_addr.port = CFGM_PORT_NUM;
        local_ip_addr.ip_addr = INADDR_ANY;
        if(CFGM_SUCCESS != cmgr_open_conn(MOD_ID_CFG_MGR, &local_ip_addr, 
                CMGR_SOCK_TYPE_UDP_SVR_CLNT, &g_connection_id, &cmgr_error))
        {
                debug(CFG_ERROR,"Connection Manager Open Connection Failed\n");
                return CFGM_CONN_MGR_OPEN_FAILED;        
        }

        remote_ip_port.port = WDTM_PORT_NUM;
        remote_ip_port.ip_addr = INADDR_ANY;

        if(CFGM_SUCCESS != cmgr_add_rem_conn_params (MOD_ID_CFG_MGR, MOD_ID_WDT_MGR, 
                &remote_ip_port, CMGR_SOCK_TYPE_UDP_SVR_CLNT, &cmgr_error))
        {
                debug(CFG_WARN,"Connection Manager Remote WDT Connection Failed\n");
                return CFGM_CONN_MGR_OPEN_FAILED;        
        }
        
        remote_ip_port.port = CLIM_PORT_NUM;
        remote_ip_port.ip_addr = INADDR_ANY;

        if(CFGM_SUCCESS != cmgr_add_rem_conn_params (MOD_ID_CFG_MGR, MOD_ID_CLI_MGR,
                &remote_ip_port, CMGR_SOCK_TYPE_UDP_SVR_CLNT, &cmgr_error))
        {
                debug(CFG_WARN,"Connection Manager Remote CLI Manager Connection Failed\n");
                return CFGM_CONN_MGR_OPEN_FAILED;
        }
        
	remote_ip_port.port = WEBM_PORT_NUM;
        remote_ip_port.ip_addr = INADDR_ANY;

        if(CFGM_SUCCESS != cmgr_add_rem_conn_params (MOD_ID_CFG_MGR, MOD_ID_WEB_SRV,
                &remote_ip_port, CMGR_SOCK_TYPE_UDP_SVR_CLNT, &cmgr_error))
        {
                debug(CFG_WARN,"Connection Manager Remote CLI Manager Connection Failed\n");
                return CFGM_CONN_MGR_OPEN_FAILED;
        }

        remote_ip_port.port = PROXM_PORT_NUM;
        remote_ip_port.ip_addr = INADDR_ANY;

	if(CFGM_SUCCESS != cmgr_add_rem_conn_params (MOD_ID_CFG_MGR, MOD_ID_PRO_MGR,
                &remote_ip_port, CMGR_SOCK_TYPE_UDP_SVR_CLNT, &cmgr_error))
        {
                debug(CFG_WARN,"Connection Manager Remote Prox Manager Connection Failed\n");
                return CFGM_CONN_MGR_OPEN_FAILED;
        }

	remote_ip_port.port = MAC_UPLD_PORT_NUM;
        remote_ip_port.ip_addr = INADDR_ANY;

        if(CFGM_SUCCESS != cmgr_add_rem_conn_params (MOD_ID_CFG_MGR,MOD_ID_MAC_UPLD_MGR,
                &remote_ip_port, CMGR_SOCK_TYPE_UDP_SVR_CLNT, &cmgr_error))
        {
                debug(CFG_WARN,"Connection Manager Remote MAC Upload Manager Connection Failed\n");
                return CFGM_CONN_MGR_OPEN_FAILED;
        }


	
        /*init cli cmd*/
	debug(CFG_DEBUG,"[cfg_mgr] init socket done\n");

        //Blocking function call for the socket manager to wait for the messge from other layers
        cmgr_proc_conn_msg(MOD_ID_CFG_MGR, &cmgr_error);
        
                        
        /* Close the connection and de-initialize the conn. mgr library */
        cmgr_close_conn (MOD_ID_CFG_MGR, g_connection_id, &cmgr_error) ;
        cmgr_deinit_conn_lib (MOD_ID_CFG_MGR) ;
        /* Un-initialize the DB TODO*/
        /* Wait for cfg manager to finish up */
        debug(CFG_WARN,"Configuration Manager exited\n");

        return 1;
}
