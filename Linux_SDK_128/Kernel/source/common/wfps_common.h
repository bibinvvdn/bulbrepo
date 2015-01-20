#ifndef _WFPS_COMMON_H
#define _WFPS_COMMON_H

#include <stdbool.h>

#define SCAN_LIST_FILE_PATH "/mnt/scanlist.txt"
//#define SCAN  "iwlist scan | grep ESSID | head -n10 |awk '{print $1}' > /mnt/scanlist.txt"
#define SCAN "iw wlan1 scan | grep SSID | awk '{print $2}' > /mnt/scanlist.txt"
#define BACKLIGHT_VALUE "/sys/devices/platform/pwm-backlight.3/backlight/pwm-backlight.3/brightness"

#define MAC_LIST_FILE_PATH "/mnt/maclist.txt"
#define MAX_MAC_ENTRY 1000
/* System configuration */
typedef struct sys_cfg {
        char hostname[32];                      /* tag id */
        unsigned short year;                    // year
        unsigned char month;                    // month
        unsigned char day;                      // date
        unsigned char hour;                     // hour
        unsigned char min;                      // minute
        unsigned char sec;                      // seconds
        char mac_addr[18];                      // system MAC addr.
        char uptime[16];                        // system uptime
        char hw_sno[20];                        // device HW serial no
        char sw_ver[20];                        // SW version
        char bootloader_ver[20];                // boot_loader version
        unsigned int total_mac_count;           // total mac collected
}SYS_CFG_T;

/* Network configuration */
typedef struct nw_cfg {
        bool dhcp;                              // dhcp enable/disable
        char ip_addr[16];                       // device IP
        char netmask[16];                       // netmask
        char gateway[16];                       // gateway
        char dns_pri[16];                       // primary dns
        char dns_sec[16];                       // secondary dns
}NW_CFG_T;


/* Wireless configurations */
typedef struct wlan_cfg {
//        char ap_ssid[32];                       // ap_mode ssid of the tag
//        unsigned int ap_chnl;                   // ap_mode channel
//        unsigned char ap_tx_pwr;                // ap_mode tx power
        char sta_ssid[32];                      // station_mode ssid to connect to
//        unsigned int sta_chnl;                   // station_mode channel
//        unsigned char sta_tx_pwr;       // station_mode tx power
        unsigned char sta_sec;                  // security mode open,wep & wpa/wpa2
//        unsigned char sta_wep_keyno;            // wep key no 
//        char sta_wep_secret[17];                // wep password
        char sta_wpa_secret[33];                // wpa/wpa2 passphrase
//        unsigned char sta_wpa_enc;              // encryption type
}WLAN_CFG_T;

typedef struct mac_table {
        char tag_id[32];                        // node identifier
        char time_stamp[21];                    // time stamp(MAC captured time)
        char mac_addr[18];                      // MAC address
        signed int rssi;                        // Rec. Signal strength 
        char channel;                           // Channel
}MAC_TABLE_T;


/* Discovered MAC List  */
typedef struct mac_list {
        unsigned int no_of_mac;                 // no of MAC in the list
        unsigned int frm_mac_count;		//starting MAC to display
        unsigned int to_mac_count;		//end MAC to display
        MAC_TABLE_T mac[MAX_MAC_ENTRY];         // MAX_MAC_ENTRY = 1000
}MAC_LIST_T;

/* Server configuration */
typedef struct server_cfg {
        char server_url[256];                   // Server URL
//        unsigned int port_no;                 // server Port
//        char uname[32];                       // user_name
//        char psswd[32];                       // password
        unsigned int upload_interval;           // upload interval
}SERVER_CFG_T;

/* Management configuration */
typedef struct mgmt_cfg {
        bool save;                              // save configuration
        bool reboot;                            // reboot the tag
        bool factory_def;                       // reboot with factory default
        char image_path[128];                   // image path
	bool upload_img;			// upload image
        char admin_psswd[32];                   // admin password
	bool change_psswd;			// change password
}MGMT_CFG_T;

/* Network Statistics */
typedef struct nw_stats {
        bool server_reachable;                  // server reachable or not
        unsigned int tx_pkts;                   // tx packets
        unsigned int rx_pkts;                   // rx packets
        unsigned int err_pkts;                  // error packets
        signed int rssi;                        // received signal strength
}NW_STATS_T;

/* Time Profile configurations */
typedef struct tprofile_cfg {
        unsigned char start_time_hh;            // start time_hour
        unsigned char start_time_mm;            // start time_minute
        unsigned char stop_time_hh;             // stop time_hour
        unsigned char stop_time_mm;             // stop time_minute
        bool repeat;                            // repeat start and stop time 
}TPROFILE_CFG_T;

/*scan table configuration*/
typedef struct scan_table {
	char ssid[100];			// Access point SSID
}SCAN_TABLE_T;

/* Scan configuration */
typedef struct scan_cfg {
	SCAN_TABLE_T scan[100];		// SSID List max 100
	int no_count;			// countinf of SSID
}SCAN_CFG_T;

/*bulb status configuration*/
typedef struct bulbstats_cfg {
	int stat_on;			//Bulb on State
	int stat_off;			//Bulb off State
	int stat_dim;			//Bulb Dim State
}BULBSTATS_CFG_T;

/*bulb blink configuration*/
typedef struct blink_cfg {
	int blink_on;			//Blink the Bulb
}BLINK_CFG_T;

/*power configuration*/
typedef struct power_cfg {
	float pwr;			//Bulb power
}POWER_CFG_T;

typedef struct poll_cfg {		//polling of current value
	int poll;
}POLL_CFG_T;

/*blackscholes configuration*/
typedef struct black_cfg {
	float  blk;
}BLACK_CFG_T;

#endif /*_WFPS_COMMON_H*/
