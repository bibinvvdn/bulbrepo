#ifndef _WFPS_DATABASE_H
#define _WFPS_DATABASE_H
#include "wfps_common.h"

typedef struct wfps_database{
        
        SYS_CFG_T               sys_cfg_t;
        NW_CFG_T                nw_cfg_t;
        WLAN_CFG_T              wlan_cfg_t;
        MAC_LIST_T              mac_list_t;
        SERVER_CFG_T            server_cfg_t;
        MGMT_CFG_T              mgmt_cfg_t;
        NW_STATS_T              nw_stats_t;
        TPROFILE_CFG_T          tprofile_cfg_t;
	SCAN_CFG_T		scan_cfg_t;
}WFPS_DATABASE_T;
#endif /*_WFPS_DATABASE_H*/
