/*
 * @file        wfps_db.c
 *
 * @brief       This file contain read, default_factory and save database file functions
 * 
 * @author      VVDN TECHNOLOGIES
 *      
 * @license     Copyright (C) 2011 
 *              This program is free software; you can redistribute it and/or modify it
 *              under the terms of the GNU General Public License version 2 as
 *              published by the Free Software Foundation.  
 */


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include"wfps_common.h"
#include"wfps_database.h"
#include"wfps_db_enm_defs.h"
#include"wfps_db_defines.h"
#include"wfps_db_cmn_types.h"
#include"wfps_db_proto.h"
#include"cfg_debug.h"

#define NORMAL_BOOT 1
#define FACTORY_DEFAULT_WITHOUT_IP_ADDRESS 2
extern WFPS_DATABASE_T wfps_database ;


/*
 * @fn          wfps_db_write_cfg_2_file
 *
 * @param       None
 *
 * @return      Returns EDB_SUCCESS if success otherwise returns an error ( < 0)
 *
 */
	db_return_et
wfps_db_write_cfg_2_file (void)
	//int wfps_db_set_hcoded_cfg1 (WFPS_DATABASE_T wfps_database);
{
	db_return_et        ret_val  =  DB_SUCCESS ;
	WFPS_DATABASE_T temp = wfps_database;
	ret_val = wfps_db_write_cfg_2_file1(temp, NORMAL_BOOT);

	return ret_val;
}



/*
 * @fn          wfps_db_write_cfg_2_file1
 *
 * @brief       This function call results into writing current configuration onto the 
 *              configuration file present on disk.
 *
 * @param       Database and status value
 *
 * @return      Returns EDB_SUCCESS if success otherwise returns an error ( < 0)
 *
 */
	db_return_et
wfps_db_write_cfg_2_file1 (WFPS_DATABASE_T wfps_database, int status_value )
{
	db_return_et        ret_val = DB_SUCCESS ;
	int i;
	FILE                *cfg_fd;
	const db_s8bit      *open_mode = "w" ;
	db_u8bit            ip_addr[16]={0}, ch='.',
			    mac_addr[16]={0},c=':';

	if (!(cfg_fd = fopen(DB_CFG_FILE_NAME, open_mode))) {
		return DB_ERR_CFG_FILE_ERROR ;
	}
	fprintf(cfg_fd,"%s:~%u\n",FACTORY_DEFAULT_STATUS_VALUE_STR, status_value);
	fprintf(cfg_fd,"%s:~%s\n",DB_SYS_CFG_HOST_NAME_STR,wfps_database.sys_cfg_t.hostname);
	fprintf(cfg_fd,"%s:~%d\n",DB_SYS_CFG_YEAR,wfps_database.sys_cfg_t.year);
	fprintf(cfg_fd,"%s:~%d\n",DB_SYS_CFG_MONTH,wfps_database.sys_cfg_t.month);
	fprintf(cfg_fd,"%s:~%d\n",DB_SYS_CFG_DAY,wfps_database.sys_cfg_t.day);
	fprintf(cfg_fd,"%s:~%d\n",DB_SYS_CFG_HOUR,wfps_database.sys_cfg_t.hour);
	fprintf(cfg_fd,"%s:~%d\n",DB_SYS_CFG_MIN,wfps_database.sys_cfg_t.min);
	fprintf(cfg_fd,"%s:~%d\n",DB_SYS_CFG_SEC,wfps_database.sys_cfg_t.sec);
//TODO
	fprintf(cfg_fd,"%s:~%s\n",DB_SYS_CFG_MAC_ADDR_STR,wfps_database.sys_cfg_t.mac_addr);
	fprintf(cfg_fd,"%s:~%s\n",DB_SYS_CFG_UP_TIME_STR,wfps_database.sys_cfg_t.uptime);
	fprintf(cfg_fd,"%s:~%s\n",DB_SYS_CFG_HW_SNO_STR,wfps_database.sys_cfg_t.hw_sno);
	fprintf(cfg_fd,"%s:~%s\n",DB_SYS_CFG_SW_VER_STR,wfps_database.sys_cfg_t.sw_ver);
	fprintf(cfg_fd,"%s:~%s\n",DB_SYS_CFG_BOOT_LDR_VER_STR,wfps_database.sys_cfg_t.bootloader_ver);
	fprintf(cfg_fd,"%s:~%u\n",DB_SYS_CFG_TOTAL_MAC_COUNT,wfps_database.sys_cfg_t.total_mac_count); 

	fprintf(cfg_fd,"%s:~%d\n",DB_NW_CFG_DHCP,wfps_database.nw_cfg_t.dhcp);
	fprintf(cfg_fd,"%s:~%s\n",DB_NW_CFG_IP_ADDR,wfps_database.nw_cfg_t.ip_addr);
	fprintf(cfg_fd,"%s:~%s\n",DB_NW_CFG_NETMASK ,wfps_database.nw_cfg_t.netmask);
	fprintf(cfg_fd,"%s:~%s\n",DB_NW_CFG_GATEWAY,wfps_database.nw_cfg_t.gateway);
	fprintf(cfg_fd,"%s:~%s\n",DB_NW_CFG_DNS_PRI,wfps_database.nw_cfg_t.dns_pri);
	fprintf(cfg_fd,"%s:~%s\n",DB_NW_CFG_DNS_SEC,wfps_database.nw_cfg_t.dns_sec);


	//fprintf(cfg_fd,"%s:~%s\n",DB_WLAN_CFG_AP_SSID,wfps_database.wlan_cfg_t.ap_ssid);
	//fprintf(cfg_fd,"%s:~%u\n",DB_WLAN_CFG_AP_CHNL,wfps_database.wlan_cfg_t.ap_chnl);
	//fprintf(cfg_fd,"%s:~%d\n",DB_WLAN_CFG_AP_TX_PWR,wfps_database.wlan_cfg_t.ap_tx_pwr);
	fprintf(cfg_fd,"%s:~%s\n",DB_WLAN_CFG_STA_SSID,wfps_database.wlan_cfg_t.sta_ssid);
	//fprintf(cfg_fd,"%s:~%u\n",DB_WLAN_CFG_STA_CHNL,wfps_database.wlan_cfg_t.sta_chnl);
	//fprintf(cfg_fd,"%s:~%u\n",DB_WLAN_CFG_STA_TX_PWR,wfps_database.wlan_cfg_t.sta_tx_pwr);
	fprintf(cfg_fd,"%s:~%d\n",DB_WLAN_CFG_STA_SEC,wfps_database.wlan_cfg_t.sta_sec);
	//fprintf(cfg_fd,"%s:~%d\n",DB_WLAN_CFG_STA_WEP_KEYNO,wfps_database.wlan_cfg_t.sta_wep_keyno);
	//fprintf(cfg_fd,"%s:~%s\n",DB_WLAN_CFG_STA_WEP_SECRET,wfps_database.wlan_cfg_t.sta_wep_secret);
	fprintf(cfg_fd,"%s:~%s\n",DB_WLAN_CFG_STA_WPA_SECRET,wfps_database.wlan_cfg_t.sta_wpa_secret);
	//fprintf(cfg_fd,"%s:~%d\n",DB_WLAN_CFG_STA_WPA_ENC,wfps_database.wlan_cfg_t.sta_wpa_enc);


#if 0 
	fprintf(cfg_fd,"%s:~%u\n",DB_MAC_LIST_NO_OF_MAC,wfps_database.mac_list_t.no_of_mac);
	fprintf(cfg_fd,"%s:~%u\n",DB_MAC_LIST_FRM_MAC_COUNT,wfps_database.mac_list_t.frm_mac_count);
	fprintf(cfg_fd,"%s:~%u\n",DB_MAC_LIST_TO_MAC_COUNT,wfps_database.mac_list_t.to_mac_count);


	for(i=0;i<2;i++){
		fprintf(cfg_fd,"%s:~%s\n",DB_MAC_LIST_MAX_MAC_ENTRY,wfps_database.mac_list_t.mac[i].tag_id);
		fprintf(cfg_fd,"%s:~%s\n",DB_MAC_LIST_MAX_MAC_ENTRY,wfps_database.mac_list_t.mac[i].time_stamp);
		fprintf(cfg_fd,"%s:~%s\n",DB_MAC_LIST_MAX_MAC_ENTRY,wfps_database.mac_list_t.mac[i].mac_addr);
		fprintf(cfg_fd,"%s:~%u\n",DB_MAC_LIST_MAX_MAC_ENTRY,wfps_database.mac_list_t.mac[i].rssi);
		fprintf(cfg_fd,"%s:~%u\n",DB_MAC_LIST_MAX_MAC_ENTRY,wfps_database.mac_list_t.mac[i].channel);
	}


#endif
	fprintf(cfg_fd,"%s:~%s\n",DB_SERVER_CFG_SERVER_URL,wfps_database.server_cfg_t.server_url);
//	fprintf(cfg_fd,"%s:~%u\n",DB_SERVER_CFG_PORT_NO,wfps_database.server_cfg_t.port_no);
//	fprintf(cfg_fd,"%s:~%s\n",DB_SERVER_CFG_UNAME ,wfps_database.server_cfg_t.uname);
//	fprintf(cfg_fd,"%s:~%s\n",DB_SERVER_CFG_PSSWD,wfps_database.server_cfg_t.psswd);
	fprintf(cfg_fd,"%s:~%u\n",DB_SERVER_CFG_UPLOAD_INTERVAL,wfps_database.server_cfg_t.upload_interval);

	fprintf(cfg_fd,"%s:~%d\n",DB_MGMT_CFG_SAVE,wfps_database.mgmt_cfg_t.save);
	fprintf(cfg_fd,"%s:~%d\n",DB_MGMT_CFG_REBOOT,wfps_database.mgmt_cfg_t.reboot);
	fprintf(cfg_fd,"%s:~%d\n",DB_MGMT_CFG_FACTORY_DEF,wfps_database.mgmt_cfg_t.factory_def);
	fprintf(cfg_fd,"%s:~%s\n",DB_MGMT_CFG_IMAGE_PATH,wfps_database.mgmt_cfg_t.image_path);
	fprintf(cfg_fd,"%s:~%s\n",DB_MGMT_CFG_ADMIN_PSSWD,wfps_database.mgmt_cfg_t.admin_psswd);
	fprintf(cfg_fd,"%s:~%d\n",DB_MGMT_CFG_CHANGE_PSSWD,wfps_database.mgmt_cfg_t.change_psswd);

//	fprintf(cfg_fd,"%s:~%d\n",DB_NW_STATS_SERVER_REACHABLE,wfps_database.nw_stats_t.server_reachable);
//	fprintf(cfg_fd,"%s:~%u\n",DB_NW_STATS_TX_PKTS,wfps_database.nw_stats_t.tx_pkts);
///	fprintf(cfg_fd,"%s:~%u\n",DB_NW_STATS_RX_PKTS,wfps_database.nw_stats_t.rx_pkts);
//	fprintf(cfg_fd,"%s:~%u\n",DB_NW_STATS_ERR_PKTS,wfps_database.nw_stats_t.err_pkts);
//	fprintf(cfg_fd,"%s:~%d\n",DB_NW_STATS_RSSI,wfps_database.nw_stats_t.rssi);

	fprintf(cfg_fd,"%s:~%d\n",DB_TPROFILE_CFG_START_TIME_HH,wfps_database.tprofile_cfg_t.start_time_hh);
	fprintf(cfg_fd,"%s:~%d\n",DB_TPROFILE_CFG_START_TIME_MM,wfps_database.tprofile_cfg_t.start_time_mm);
	fprintf(cfg_fd,"%s:~%d\n",DB_TPROFILE_CFG_STOP_TIME_HH,wfps_database.tprofile_cfg_t.stop_time_hh);
	fprintf(cfg_fd,"%s:~%d\n",DB_TPROFILE_CFG_STOP_TIME_MM,wfps_database.tprofile_cfg_t.stop_time_mm);
	fprintf(cfg_fd,"%s:~%d\n",DB_TPROFILE_CFG_REPEAT,wfps_database.tprofile_cfg_t.repeat);

	fclose(cfg_fd);
	return ret_val;
}


/*
 * @fn          wfps_db_set_cfg_frm_file
 *
 * @brief       This function is used for system initialization.
 *
 * @param       None
 *
 * @return      Returns DB_SUCCESS if success otherwise returns an error ( < 0)
 *
 */

	db_return_et
wfps_db_set_cfg_frm_file(db_void)
{
	FILE *cfg_fd;
	WFPS_DATABASE_T wfps_db, temp, temp_db;
	db_return_et ret_val = DB_SUCCESS;
	int status_value=0, i=0;
	const char   *open_mode = "r" ;
	if (!(cfg_fd = fopen(DB_CFG_FILE_NAME, open_mode))) {
	
		return DB_ERR_CFG_FILE_ERROR ;
	}
	fclose(cfg_fd);
	memset(&temp,0,sizeof(temp));
	memset(&temp_db,0,sizeof(temp_db));
	memset(&wfps_db,0,sizeof(wfps_db));
	ret_val=wfps_db_read_cfg_frm_file(&wfps_db, &status_value);
	if(ret_val!= DB_SUCCESS) {
		return ret_val;
	}           

	if(status_value==NORMAL_BOOT) {
		memset(&wfps_database,0,sizeof(WFPS_DATABASE_T));
		memcpy(&wfps_database,&wfps_db,sizeof(WFPS_DATABASE_T));
		return DB_SUCCESS;
	}
//	memset(&wfps_database,0,sizeof(WFPS_DATABASE_T));
//	memcpy(&wfps_database,&wfps_db,sizeof(WFPS_DATABASE_T));
	return ret_val;
}

/*
 * @fn          wfps_db_read_cfg_frm_file
 *
 * @brief       This function is used internally by DB to read configuration from a file.
 *
 * @param       database and status value
 *
 * @return      Returns DB_SUCCESS if success otherwise returns an error ( < 0)
 *
 */

	db_return_et
wfps_db_read_cfg_frm_file (WFPS_DATABASE_T *database, int *status_value)
{
	int                 status = 1;
	db_return_et        ret_val = DB_SUCCESS ;
	FILE                *cfg_fd = NULL ;
	WFPS_DATABASE_T     wfps_base;
	const char          *open_mode = "r" ;
	db_u8bit            cypher_key[DB_CYPHER_KEY_LENGTH], 
			    cfg_val[DB_CYPHER_KEY_VAL_FIELD_LEN] ;
	db_u32bit           tmp_len = 0;
	db_s8bit            *read_line = NULL;
	db_u8bit            mac_addr[18]={0},
			    ip_addr[16]={0};
	size_t              line_sz = 0 ;
	memset(&wfps_base,0,sizeof(wfps_base));
//	int MAC_ENTRY = 0;
	/* open the database file */
	if (!(cfg_fd = fopen(DB_CFG_FILE_NAME, open_mode))) {
		printf("No cfg file\n");
		return DB_ERR_CFG_FILE_ERROR ;
	}

	/* The file is opened now start reading the file and set the configuration */
	while (-1 != getline (&read_line, &line_sz, cfg_fd)) {
		memset (cypher_key, '\0', DB_CYPHER_KEY_LENGTH) ;
		memset (cfg_val, '\0', DB_CYPHER_KEY_VAL_FIELD_LEN) ;
		sscanf (read_line, "%[A-Za-z0-9_]:~%[^\r\n]\n", cypher_key, cfg_val) ;
		read_line = NULL;
		/* A small optimization */
		if (0 == cypher_key[0])
			continue ;

		else if (0 == strcmp(cypher_key,DB_NW_CFG_DHCP))          
			wfps_base.nw_cfg_t.dhcp=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_NW_CFG_IP_ADDR)) 
			strcpy(wfps_base.nw_cfg_t.ip_addr,cfg_val);

		else if (0 == strcmp(cypher_key,DB_NW_CFG_NETMASK)) 
			strcpy(wfps_base.nw_cfg_t.netmask,cfg_val);

		else if (0 == strcmp(cypher_key,DB_NW_CFG_GATEWAY)) 
			strcpy(wfps_base.nw_cfg_t.gateway,cfg_val);

		else if (0 == strcmp(cypher_key, DB_NW_CFG_DNS_PRI)) 
			strcpy(wfps_base.nw_cfg_t.dns_pri,cfg_val);

		else if (0 == strcmp(cypher_key,DB_NW_CFG_DNS_SEC)) 
			strcpy(wfps_base.nw_cfg_t.dns_sec,cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_HOST_NAME_STR)) 
			strcpy(wfps_base.sys_cfg_t.hostname,cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_YEAR)) 
			wfps_base.sys_cfg_t.year=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_MONTH)) 
			wfps_base.sys_cfg_t.month=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_DAY)) 
			wfps_base.sys_cfg_t.day=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_HOUR)) 
			wfps_base.sys_cfg_t.hour=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_MIN)) 
			wfps_base.sys_cfg_t.min=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_SEC))
			wfps_base.sys_cfg_t.sec=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_MAC_ADDR_STR)) 
			strcpy(wfps_base.sys_cfg_t.mac_addr,cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_UP_TIME_STR)) 
			strcpy(wfps_base.sys_cfg_t.uptime,cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_HW_SNO_STR)) 
			strcpy(wfps_base.sys_cfg_t.hw_sno,cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_SW_VER_STR)) 
			strcpy(wfps_base.sys_cfg_t.sw_ver,cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_BOOT_LDR_VER_STR)) 
			strcpy(wfps_base.sys_cfg_t.bootloader_ver,cfg_val);

		else if (0 == strcmp(cypher_key,DB_SYS_CFG_TOTAL_MAC_COUNT)) 
			wfps_base.sys_cfg_t.total_mac_count=atoi(cfg_val);

//		else if (0 == strcmp(cypher_key,DB_WLAN_CFG_AP_SSID)) 
//			strcpy(wfps_base.wlan_cfg_t.ap_ssid,cfg_val);

//		else if (0 == strcmp(cypher_key,DB_WLAN_CFG_AP_CHNL)) 
//			wfps_base.wlan_cfg_t.ap_chnl=atoi(cfg_val);

//		else if (0 == strcmp(cypher_key,DB_WLAN_CFG_AP_TX_PWR)) 
//			wfps_base.wlan_cfg_t.ap_tx_pwr=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_WLAN_CFG_STA_SSID)) 
			strcpy(wfps_base.wlan_cfg_t.sta_ssid,cfg_val);

//		else if (0 == strcmp(cypher_key,DB_WLAN_CFG_STA_CHNL)) 
//			wfps_base.wlan_cfg_t.sta_chnl=atoi(cfg_val);

//		else if (0 == strcmp(cypher_key,DB_WLAN_CFG_STA_TX_PWR )) 
//			wfps_base.wlan_cfg_t.sta_tx_pwr=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_WLAN_CFG_STA_SEC)) 
			wfps_base.wlan_cfg_t.sta_sec=atoi(cfg_val);

//		else if (0 == strcmp(cypher_key,DB_WLAN_CFG_STA_WEP_KEYNO)) 
//			wfps_base.wlan_cfg_t.sta_wep_keyno=atoi(cfg_val);

//		else if (0 == strcmp(cypher_key,DB_WLAN_CFG_STA_WEP_SECRET)) 
//			strcpy(wfps_base.wlan_cfg_t.sta_wep_secret,cfg_val);

		else if (0 == strcmp(cypher_key,DB_WLAN_CFG_STA_WPA_SECRET)) 
			strcpy(wfps_base.wlan_cfg_t.sta_wpa_secret,cfg_val);

//		else if (0 == strcmp(cypher_key,DB_WLAN_CFG_STA_WPA_ENC)) 
//			wfps_base.wlan_cfg_t.sta_wpa_enc=atoi(cfg_val);
#if 0

		else if (0 == strcmp(cypher_key,DB_MAC_LIST_NO_OF_MAC)) 
			wfps_base.mac_list_t.no_of_mac=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_MAC_LIST_FRM_MAC_COUNT)) 
			wfps_base.mac_list_t.frm_mac_count=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_MAC_LIST_TO_MAC_COUNT)) 
			wfps_base.mac_list_t.to_mac_count=atoi(cfg_val);

		else   if (0 == strcmp(cypher_key,DB_MAC_LIST_MAX_MAC_ENTRY)){

			strcpy(wfps_base.mac_list_t.mac[MAC_ENTRY].tag_id,cfg_val);
			strcpy(wfps_base.mac_list_t.mac[MAC_ENTRY].time_stamp,cfg_val);
			strcpy(wfps_base.mac_list_t.mac[MAC_ENTRY].mac_addr,cfg_val);

			wfps_base.mac_list_t.mac[MAC_ENTRY].rssi=atoi(cfg_val);

			wfps_base.mac_list_t.mac[MAC_ENTRY].channel=atoi(cfg_val);
			MAC_ENTRY++;
		}
#endif
		else if (0 == strcmp(cypher_key,DB_SERVER_CFG_SERVER_URL)) 
			strcpy(wfps_base.server_cfg_t.server_url,cfg_val);
#if 0
		else if (0 == strcmp(cypher_key,DB_SERVER_CFG_PORT_NO)) 
			wfps_base.server_cfg_t.port_no=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_SERVER_CFG_UNAME)) 
			strcpy(wfps_base.server_cfg_t.uname,cfg_val);

		else if (0 == strcmp(cypher_key,DB_SERVER_CFG_PSSWD)) 
			strcpy(wfps_base.server_cfg_t.psswd,cfg_val);
#endif
		else if (0 == strcmp(cypher_key,DB_SERVER_CFG_UPLOAD_INTERVAL)) 
			wfps_base.server_cfg_t.upload_interval=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_MGMT_CFG_SAVE)) 
			wfps_base.mgmt_cfg_t.save=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_MGMT_CFG_REBOOT)) 
			wfps_base.mgmt_cfg_t.reboot=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_MGMT_CFG_FACTORY_DEF)) 
			wfps_base.mgmt_cfg_t.factory_def=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_MGMT_CFG_IMAGE_PATH)) 
			strcpy(wfps_base.mgmt_cfg_t.image_path,cfg_val);

		else if (0 == strcmp(cypher_key,DB_MGMT_CFG_ADMIN_PSSWD)) 
			strcpy(wfps_base.mgmt_cfg_t.admin_psswd,cfg_val);

		else if (0 == strcmp(cypher_key,DB_MGMT_CFG_CHANGE_PSSWD)) 
			wfps_base.mgmt_cfg_t.change_psswd=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_TPROFILE_CFG_START_TIME_HH)) 
			wfps_base.tprofile_cfg_t.start_time_hh=atoi(cfg_val);    

		else if (0 == strcmp(cypher_key,DB_TPROFILE_CFG_START_TIME_MM)) 
			wfps_base.tprofile_cfg_t.start_time_mm=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_TPROFILE_CFG_STOP_TIME_HH)) 
			wfps_base.tprofile_cfg_t.stop_time_hh=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_TPROFILE_CFG_STOP_TIME_MM)) 
			wfps_base.tprofile_cfg_t.stop_time_mm=atoi(cfg_val);

		else if (0 == strcmp(cypher_key,DB_TPROFILE_CFG_REPEAT)) 
			wfps_base.tprofile_cfg_t.repeat=cfg_val;

		/*		//else if(0==strcmp(cypher_key,DB_VALID_STR)) 
		// wfps_base.db_valid = atoi(cfg_val);   */
	}

	if(read_line){
		free(read_line);
	}
	if(cfg_fd){
		fclose(cfg_fd);
	}
	memcpy(database, &wfps_base, sizeof(WFPS_DATABASE_T));
	memcpy(status_value, &status, sizeof(status));
	return ret_val;
}

	/*end of read data from flash memory function*/ 



	/*
	 * @fn          ip_str_byte
	 *
	 * @brief       This function convert IP address dotted decimal string into integer bytes       
	 *
	 * @param[in]   str     A pointer to the ip address string      
	 *              ptr     A pointer to the ip address bytes
	 * @return      Returns 0 if success 
	 */

	db_s32bit ip_str_byte(db_u8bit *str,db_u8bit *ptr)
	{
		unsigned char a=0,num=0,i=0,j=0;

		while(str[i]) {
			while(str[i]!='.' && str[i]) {
				a=str[i]-48;
				num=num*10+a;
				i++;
			}
			ptr[j++]=num;
			i++;
			num=0;
			a=0;
		}
		return 0;
	}



	/*
	 * @fn          convert_mac_addr_str_into_byte
	 *
	 * @brief       This function convert mac address string into bytes and used by 
	 *
	 * @param[in]   mac_addr_str    A pointer to mac address string
	 *              mac_addr        A pointer to mac address byte 
	 *
	 * @return      Returns 0 if success otherwise returns 1
	 */

	db_s32bit convert_mac_addr_str_into_byte(db_u8bit *mac_addr_str, db_u8bit *mac_addr)
	{
		unsigned short i=0;
		unsigned long value=0;
		char ch=0;
		for(i=0;i<6;++i) {
			ch = tolower( *mac_addr_str++);
			if((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f')) {
				return 1;
			}
			value = isdigit(ch) ? (ch - '0') : (ch - 'a' + 10);
			ch = tolower(*mac_addr_str);
			if((i < 5 && ch != ':') || (i ==5 && ch != '\0' && !isspace(ch))) {
				++mac_addr_str;
				if((ch < '0' || ch >'9') && (ch < 'a' || ch > 'f' )) {
					return 1;
				}
				value <<= 4;
				value += isdigit(ch) ? (ch - '0') : (ch - 'a' + 10);
				ch = *mac_addr_str;
				if(i < 5 && ch != ':') {
					return 1;
				}
			}
			mac_addr[i] = (unsigned char)value;
			++mac_addr_str;
		}
		return 0;
	}




	/*
	 * @fn          wfps_db_set_hcoded_cfg
	 *
	 * @param       none
	 *
	 * @return      Returns DB_SUCCESS if success otherwise returns an error ( < 0)
	 *
	 */



	db_return_et
		wfps_db_set_hcoded_cfg (db_void)
		{

			db_return_et ret_val = DB_SUCCESS;
			wfps_db_set_hcoded_cfg1(&wfps_database);
			return ret_val;

		}



	/*
	 * @fn          wfps_db_set_hcoded_cfg1
	 *
	 * @brief       This function is used internally by DB to set factory default configuration
	 *
	 * @param       wfps_database
	 *
	 * @return      Returns DB_SUCCESS if success otherwise returns an error ( < 0)
	 *
	 */






	//WFPS_DATABASE_T wfps_db_set_hcoded_cfg1 (WFPS_DATABASE_T *database)    
	int wfps_db_set_hcoded_cfg1 (WFPS_DATABASE_T *database)    
	{
		db_return_et  ret_val = DB_SUCCESS ;
		WFPS_DATABASE_T wfps_data;
		memset(&wfps_database,'\0',sizeof(WFPS_DATABASE_T));
		db_u8bit        cnt = 0, idx=0, port=0;
		/*database valid number*/
		strcpy(wfps_data.sys_cfg_t.hostname,"vvdntech");
		wfps_data.sys_cfg_t.year=0;
		wfps_data.sys_cfg_t.month=0;
		wfps_data.sys_cfg_t.day=0;
		wfps_data.sys_cfg_t.hour=0;
		wfps_data.sys_cfg_t.min=0;
		wfps_data.sys_cfg_t.sec=0;
		strcpy(wfps_data.sys_cfg_t.mac_addr,"xx:xx:xx:xx:xx:xx");
		strcpy(wfps_data.sys_cfg_t.uptime,"00:00");
		strcpy(wfps_data.sys_cfg_t.hw_sno,"v0.1");
		strcpy(wfps_data.sys_cfg_t.sw_ver,"v0.1");
		strcpy(wfps_data.sys_cfg_t.bootloader_ver,"U-Boot-2009.08");
		wfps_data.sys_cfg_t.total_mac_count=0;

		wfps_data.nw_cfg_t.dhcp=0;
		strcpy(wfps_data.nw_cfg_t.ip_addr,"192.168.100.1");
		strcpy(wfps_data.nw_cfg_t.netmask,"255.255.255.0");
		strcpy(wfps_data.nw_cfg_t.gateway,"192.168.100.1");
		strcpy(wfps_data.nw_cfg_t.dns_pri,"0.0.0.0");
		strcpy(wfps_data.nw_cfg_t.dns_sec,"0.0.0.0");

//		strcpy(wfps_data.wlan_cfg_t.ap_ssid,"CLTU_LEDC_AP");
//		wfps_data.wlan_cfg_t.ap_chnl=0;
//		wfps_data.wlan_cfg_t.ap_tx_pwr=18;

		strcpy(wfps_data.wlan_cfg_t.sta_ssid,"CLTU_LEDC");
//		wfps_data.wlan_cfg_t.sta_chnl=0;
//		wfps_data.wlan_cfg_t.sta_tx_pwr=18;

//		wfps_data.wlan_cfg_t.sta_sec=0;
//		wfps_data.wlan_cfg_t.sta_wep_keyno=1;
//		strcpy(wfps_data.wlan_cfg_t.sta_wep_secret," ");
		strcpy(wfps_data.wlan_cfg_t.sta_wpa_secret," ");
//		wfps_data.wlan_cfg_t.sta_wpa_enc=0;

//		wfps_data.mac_list_t.no_of_mac=0;
//		wfps_data.mac_list_t.frm_mac_count=0;
//		wfps_data.mac_list_t.to_mac_count=0;

//		strcpy(wfps_data.mac_list_t.mac[1].tag_id,"\0");
//		strcpy(wfps_data.mac_list_t.mac[1].time_stamp,"\0");
//		strcpy(wfps_data.mac_list_t.mac[1].mac_addr,"\0");
//		wfps_data.mac_list_t.mac[1].rssi=0;
//		wfps_data.mac_list_t.mac[1].channel=0;
		strcpy(wfps_data.server_cfg_t.server_url,"http://beeceptible.herokuapp.com/wificlient/stamp");
//		wfps_data.server_cfg_t.port_no=80;
//		strcpy(wfps_data.server_cfg_t.uname,"\0");
//		strcpy(wfps_data.server_cfg_t.psswd,"\0");
		wfps_data.server_cfg_t.upload_interval=10;

		wfps_data.mgmt_cfg_t.save=0;
		wfps_data.mgmt_cfg_t.reboot=0;
		wfps_data.mgmt_cfg_t.factory_def=0;
		strcpy(wfps_data.mgmt_cfg_t.image_path," ");
		strcpy(wfps_data.mgmt_cfg_t.admin_psswd,"passwd");
		wfps_data.mgmt_cfg_t.change_psswd=0;

//		wfps_database.nw_stats_t.server_reachable=0;
//		wfps_database.nw_stats_t.tx_pkts=0;
//		wfps_database.nw_stats_t.rx_pkts=0;
//		wfps_database.nw_stats_t.err_pkts=0;
//		wfps_database.nw_stats_t.rssi=0;

		wfps_data.tprofile_cfg_t.start_time_hh=0;
		wfps_data.tprofile_cfg_t.start_time_mm=0;
		wfps_data.tprofile_cfg_t.stop_time_hh=0;
		wfps_data.tprofile_cfg_t.stop_time_mm=0;
		wfps_data.tprofile_cfg_t.repeat=0;
		memcpy(&wfps_database, &wfps_data, sizeof(WFPS_DATABASE_T));
		return 0;	    

	}

	db_return_et wfps_db_config_db_init(db_void) 
	{
		db_return_et       ret_val = DB_SUCCESS;
		db_u16bit           cnt = 0,idx=0;

		/* Read the configuration file if present */
		if (DB_ERR_CFG_FILE_ERROR == (ret_val = wfps_db_set_cfg_frm_file ())) {
			ret_val = wfps_db_set_hcoded_cfg ();
			wfps_db_write_cfg_2_file();
		}
		return ret_val;
	}





