/*
 * @file       wfps_db_proto.h
 *
 * @brief      This file holds prototypes of database modules
 *
 * @author     VVDN TECHNOLOGIES
 *      
 * @license    Copyright (C) 2011 
 *             This program is free software; you can redistribute it and/or modify it
 *             under the terms of the GNU General Public License version 2 as
 *             published by the Free Software Foundation.  
*/


#ifndef __WFPS_DB_PROTO_H__
#define __WFPS_DB_PROTO_H__

#include"wfps_db_cmn_types.h"
#include"wfps_db_enm_defs.h"
#include "wfps_database.h"

db_return_et 
p_db_config_db_init (db_void);

db_return_et 
wfps_db_set_hcoded_cfg (db_void);

db_return_et 
wfps_db_set_cfg_frm_file (db_void);

db_return_et 
wfps_db_write_cfg_2_file (db_void);

db_s32bit
convert_mac_addr_str_into_byte (db_u8bit * ,db_u8bit *);

db_s32bit
ip_str_byte(db_u8bit  *,db_u8bit *);


/* WFPS_DATABASE_T
wfps_db_set_hcoded_cfg1 (WFPS_DATABASE_T wfps_database)   ;
*/
int wfps_db_set_hcoded_cfg1 (WFPS_DATABASE_T *wfps_database)   ;

db_return_et
wfps_db_read_cfg_frm_file (WFPS_DATABASE_T *database, int *status_value);


db_return_et
wfps_db_write_cfg_2_file1 (WFPS_DATABASE_T wfps_database, int status_value );

int wfps_set_default_cfg_excludip(void);
int wfps_set_default_cfg_interface(void);

int wfps_set_default_cfg(void);

#endif
