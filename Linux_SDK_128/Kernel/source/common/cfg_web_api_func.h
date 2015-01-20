#include "wfps_common.h"
#ifndef _CFG_WEB_API_FUNC_H
#define _CFG_WEB_API_FUNC_H

/*Functions which are going to handle the web calls inside the cfg_mgr*/
void Set_Sys_Cfg_page(SYS_CFG_T *sys_cfg);
void Get_Sys_Cfg_page(void);
void Set_Nw_Cfg_page(NW_CFG_T *nw_cfg);
void Get_Nw_Cfg_page(void);
void Set_Wlan_Cfg_page(WLAN_CFG_T *wlan_cfg);
void Get_Wlan_Cfg_page(void);
void Get_Mac_List_page(MAC_LIST_T *mac_list);
void Set_Srv_Cfg_page(SERVER_CFG_T *srv_cfg);
void Get_Srv_Cfg_page(int);
void Set_Mgt_Cfg_page(MGMT_CFG_T *mgmt_cfg);
void Get_Net_Stats_page(void);
void Set_TProf_Cfg_page(TPROFILE_CFG_T *tprof_cfg);
void Get_TProf_Cfg_page(void);

void Get_Scan_Cfg_page(SCAN_CFG_T *scan_cfg);
void Set_Bulb_Cfg_page(BULBSTATS_CFG_T *bulbstats_cfg);
void Set_Blink_Cfg_page(BLINK_CFG_T *blink_cfg);
void Get_Power_Cfg_page(POWER_CFG_T *power_cfg);
void Get_Black_Cfg_page(BLACK_CFG_T *black_cfg);
void Get_Poll_Cfg_page(POLL_CFG_T *poll_cfg);
#endif /*_CFG_WEB_API_FUNC_H*/
