/*
 * @file       tdmoip_db_defines.h
 *      
 * @brief      This file contains the DB modules internal #defines and enumerations 
 * @author     VVDN TECHNOLOGIES
 *      
 * @license    Copyright (C) 2011 
 *             This program is free software; you can redistribute it and/or modify it
 *             under the terms of the GNU General Public License version 2 as
 *             published by the Free Software Foundation.  
 */

#ifndef     __TDMOIP_DB_DEFINES_H__




#ifndef     DB_CFG_FILE_NAME
#define     DB_CFG_FILE_NAME                    "/mnt/WFPS_Config.cfg"

#else
#warning "Compiling with command line passed DB Cfg file Path"
#endif  /* End of DB_CFG_FILE_NAME */



/* Various configuration cypher string */
#define     DB_CYPHER_KEY_LENGTH                64
#define     DB_CYPHER_KEY_VAL_FIELD_LEN         128

#define     DB_VALID_STR			"WFPS_DB_VALID"


#define     FACTORY_DEFAULT_STATUS_VALUE_STR    "FACTORY_STATUS_VALUE"

#define    DB_NW_CFG_DHCP                       "DHCP_ENABLE"
#define    DB_NW_CFG_IP_ADDR                    "DEVICE_IP"
#define    DB_NW_CFG_NETMASK                    "NETMASK"
#define    DB_NW_CFG_GATEWAY                    "GATEWAY"
#define    DB_NW_CFG_DNS_PRI                    "PRIMARY_DNS"
#define    DB_NW_CFG_DNS_SEC                     "SECONDARY_DNS"


#define     DB_SYS_CFG_HOST_NAME_STR            "HOSTNAME"
#define     DB_SYS_CFG_YEAR                     "YEAR"             
#define     DB_SYS_CFG_MONTH                    "MONTH"
#define     DB_SYS_CFG_DAY                      "DAY"
#define     DB_SYS_CFG_HOUR                     "HOUR"
#define     DB_SYS_CFG_MIN                      "MINUTES"
#define     DB_SYS_CFG_SEC                      "SECONDS"
#define     DB_SYS_CFG_MAC_ADDR_STR             "DEVICE_MAC"
#define     DB_SYS_CFG_UP_TIME_STR              "UPTIME"
#define     DB_SYS_CFG_HW_SNO_STR               "HW_SERIAL_NO"
#define     DB_SYS_CFG_SW_VER_STR               "SW_SERIAL_NO"
#define     DB_SYS_CFG_BOOT_LDR_VER_STR         "BOOT_LDR_VER"
#define     DB_SYS_CFG_TOTAL_MAC_COUNT          "TOTAL_MAC_COUNT"


#define     DB_WLAN_CFG_AP_SSID                 "AP_SSID"
#define     DB_WLAN_CFG_AP_CHNL                 "AP_CHANNEL"
#define     DB_WLAN_CFG_AP_TX_PWR               "AP_TRANSMISSION_POWER" 
#define     DB_WLAN_CFG_STA_SSID                "STA_SSID"
#define     DB_WLAN_CFG_STA_CHNL                "STA_CHANNEL"
#define     DB_WLAN_CFG_STA_TX_PWR              "STA_TRANSMISSON_POWER"
#define     DB_WLAN_CFG_STA_SEC                 "STA_SECURITY"
#define     DB_WLAN_CFG_STA_WEP_KEYNO           "WEP_KEYNO"
#define     DB_WLAN_CFG_STA_WEP_SECRET          "WEP_PASSWORD"
#define     DB_WLAN_CFG_STA_WPA_SECRET          "WPA_PASSPHRASE"
#define     DB_WLAN_CFG_STA_WPA_ENC             "STA_WPA_ENCRYPTION"



#define    DB_MAC_LIST_NO_OF_MAC                "NO_OF_MAC_IN_LIST"
#define    DB_MAC_LIST_FRM_MAC_COUNT            "STARTING_MAC_TO_DISPLAY"
#define    DB_MAC_LIST_TO_MAC_COUNT             "END_MAC_TO_DISPLAY"
#define    DB_MAC_LIST_MAX_MAC_ENTRY            "MAX_MAC_ENTRY"


#define    DB_MAC_TABLE_MAX_MAC_ENTRY           "MAX_MAC_ENTRY"        
#define    DB_MAC_TABLE_TAG_ID                  "NODE_IDENTIFIER"
#define    DB_MAC_TABLE_TIME_STAMP              "TIME_STAMP"
#define    DB_MAC_TABLE_MAC_ADDR                "MAC_ADDRESS"
#define    DB_MAC_TABLE_RSSI                    "RECEIVED_SIGNAL_STRENGTH"
#define    DB_MAC_TABLE_CHANNEL                 "CHANNEL"




#define    DB_SERVER_CFG_SERVER_URL             "SERVER_URL"
//#define    DB_SERVER_CFG_PORT_NO              "PORT_NO"
//#define    DB_SERVER_CFG_UNAME                "USER_NAME"
//#define    DB_SERVER_CFG_PSSWD                "PASSWORD"
#define    DB_SERVER_CFG_UPLOAD_INTERVAL        "UPLOAD_INTERVAL"



#define     DB_MGMT_CFG_SAVE                    "SAVE_CONFIGURATION"
#define     DB_MGMT_CFG_REBOOT                  "REBOOT_THE_TAG"
#define     DB_MGMT_CFG_FACTORY_DEF             "REBOOT_WITH_FACTORY_DEFAULT"
#define     DB_MGMT_CFG_IMAGE_PATH              "IMAGE_PATH"
#define     DB_MGMT_CFG_ADMIN_PSSWD             "ADMIN_PASSWORD"
#define     DB_MGMT_CFG_CHANGE_PSSWD            "CHANGE_PASSWORD"


#define     DB_NW_STATS_SERVER_REACHABLE         "SERVER_REACHABLE_OR_NOT"
#define     DB_NW_STATS_TX_PKTS                  "TX_PKTS"
#define     DB_NW_STATS_RX_PKTS                  "RX_PKTS"
#define     DB_NW_STATS_ERR_PKTS                 "ERR_PKTS"
#define     DB_NW_STATS_RSSI                     "RSSI"

#define    DB_TPROFILE_CFG_START_TIME_HH         "START_TIME_HOUR"
#define    DB_TPROFILE_CFG_START_TIME_MM         "START_TIME_MINUTES"
#define    DB_TPROFILE_CFG_STOP_TIME_HH          "STOP_TIME_HOUR"
#define    DB_TPROFILE_CFG_STOP_TIME_MM          "STOP_TIME_MINUTES"
#define    DB_TPROFILE_CFG_REPEAT                "REPEAT_START_AND_STOP_TIME"


#define     DB_SYSTEM_INFO_LOCATION		"SYSTEM_LOCATION_INFO"
#define     DB_SYSTEM_INFO_CONTACT		"SYSTEM_CONTACT_INFO"
#define     DB_SYSTEM_INFO_SERVICES		"SYSTEM_SERVICES_INFO"
#define     DB_SYSTEM_INFO_OBJECT_ID		"SYSTEM_OBJECTID_INFO"
#define     DB_SYSTEM_INFO_ENTERPRISE_ID	"SYSTEM_ENTERPRISEID_INFO"

#define     DB_SWITCH_SWMODE     		"SWITCH_MODE_STATUS"
#define     DB_SWITCH_PORT_PROTECT_PORT_NUM     "PORTPROTECT_PORT_NUM"
#define     DB_SWITCH_PORT_PROTECT_PORT_STATE   "PORTPROTECT_PORT_STATE"
#define     DB_SWITCH_JUMBO_FRAME_PORT_NUM	"JUMBO_FRAME_PORT_NUM"
#define     DB_SWITCH_JUMBO_FRAME_PORT_STATE    "JUMBO_FRAME_PORT_STATE"
#define     DB_SWITCH_TRUNK_GROUP_NUM		"TRUNK_GROUP_NUM"
#define	    DB_SWITCH_TRUNK_GROUP_STATE		"TRUNK_GROUP_STATE"
#define	    DB_SWITCH_PORT_CONFIG_NUM		"PORT_CONFIG_PORT_NUM"
#define     DB_SWITCH_PORT_CONFIG_SPEED		"PORT_CONFIG_SPEED"
#define     DB_SWITCH_PORT_CONFIG_MODE		"PORT_CONFIG_MODE"
#define     DB_SWITCH_INGRESS_PORT_NUM		"INGRESS_PORT_NUM"
#define	    DB_SWITCH_INGRESS_PORT_RATE		"INGRESS_PORT_RATE"
#define     DB_SWITCH_AUTOPOLL_PORT_NUM         "AUTOPOLL_PORT_NUM"
#define     DB_SWITCH_AUTOPOLL_STATE		"AUTOPOLL_PORT_STATE"
#define     DB_SWITCH_QOS_PORT_NUM		"QOS_PORT_NUM"
#define	    DB_SWITCH_QOS_PORT_TYPE		"QOS_PORT_TYPE"
#define     DB_SWITCH_FLOW_CONTROL_PORT_NUM	"FLOW_CONTROL_PORT_NUM"
#define     DB_SWITCH_FLOW_CONTROL_PORT_FLOW	"FLOW_CONTROL_PORT_FLOW"
#define     DB_SWITCH_TRUNK_GROUP_NUM		"TRUNK_GROUP_NUM"
#define     DB_SWITCH_TRUNK_GROUP_STATE		"TRUNK_GROUP_STATE"
#define     DB_SWITCH_QOS_801P_TYPE		"QOS_PRIORITY_801P_TYPE"
#define     DB_SWITCH_QOS_801P_TAG		"QOS_PRIORITY_801P_TAG"
#define     DB_SWITCH_QOS_801P_QUEUE		"QOS_PRIORITY_801P_QUEUE"
#define     DB_SWITCH_QOS_DSCP_TYPE		"QOS_PRIORITY_DSCP_TYPE"
#define     DB_SWITCH_QOS_DSCP_TAG		"QOS_PRIORITY_DSCP_TAG"
#define     DB_SWITCH_QOS_DSCP_QUEUE		"QOS_PRIORITY_DSCP_QUEUE"
#define     DB_SWITCH_QOS_PORTBASED_TYPE	"QOS_PRIORITY_PORT_TYPE"
#define     DB_SWITCH_QOS_PORTBASED_TAG		"QOS_PRIORITY_PORT_TAG"
#define     DB_SWITCH_QOS_PORTBASED_QUEUE	"QOS_PRIORITY_PORT_QUEUE"
#define     DB_SWITCH_VLAN_ID			"SWITCH_VLAN_ID"
#define     DB_SWITCH_VLAN_PORT_GROUP		"SWITCH_VLAN_PORT_GROUP"
#define     DB_SWITCH_VLAN_PORT_UNTAG		"SWITCH_VLAN_PORT_UNTAG"
#define     DB_SWITCH_VLAN_PORT_VALID		"SWITCH_VLAN_PORT_VALID"
#define     DB_SWITCH_RSTP_STATUS		"SWITCH_RSTP"

#endif/*tdmoip_db_defines.h*/
