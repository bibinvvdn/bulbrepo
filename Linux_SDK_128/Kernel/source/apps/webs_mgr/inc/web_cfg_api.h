#ifndef _WEB_CFG_API_H
#define _WEB_CFG_API_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmgr_user_db.h"
#include "cmgr_int_api.h"
#include "wfps_common.h"
typedef enum{
        SUCCESS = 0,
        FAILURE
}api_return_e;

api_return_e Web_Set_Sys_Cfg(SYS_CFG_T *, char *);
api_return_e Web_Get_Sys_Cfg(SYS_CFG_T *, char *);
api_return_e Web_Set_Nw_Cfg(NW_CFG_T *, char *);
api_return_e Web_Get_Nw_Cfg(NW_CFG_T *, char *);
api_return_e Web_Set_Wlan_Cfg(WLAN_CFG_T *, char *);
api_return_e Web_Get_Wlan_Cfg(WLAN_CFG_T *, char *);
api_return_e Web_Get_Mac_List(MAC_LIST_T *, char *);
api_return_e Web_Set_Srv_Cfg(SERVER_CFG_T *, char *);
api_return_e Web_Get_Srv_Cfg(SERVER_CFG_T *, char *);
api_return_e Web_Set_Mgt_Cfg(MGMT_CFG_T *, char *);
api_return_e Web_Get_Net_Stats(NW_STATS_T *, char *);
api_return_e Web_Set_TProf_Cfg(TPROFILE_CFG_T *, char *);
api_return_e Web_Get_TProf_Cfg(TPROFILE_CFG_T *, char *);

api_return_e Web_Get_Scan_Cfg(SCAN_CFG_T *, char *);
api_return_e Web_Get_Power_Cfg(POWER_CFG_T *, char *);
api_return_e Web_Set_Bulb_Cfg(BULBSTATS_CFG_T *, char *);
api_return_e Web_Set_blink_Cfg(BLINK_CFG_T *, char *);

#endif /*_WEB_CFG_API_H*/
