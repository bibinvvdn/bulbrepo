/*
 * @file        wfps_db_enm_defs.h
 *
 * @brief       This file holds element Mgr DB Module return types. The error codes here can be passed to return error command status in the                   IPC APIs.
 *
 * @author      VVDN TECHNOLOGIES
 *      
 * @license     Copyright (C) 2011 
 *              This program is free software; you can redistribute it and/or modify it
 *              under the terms of the GNU General Public License version 2 as
 *              published by the Free Software Foundation.  
 */
 
#ifndef __TDMOIP_DB_ENM_DEFS_H__
#define __TDMOIP_DB_ENM_DEFS_H__
  
typedef enum db_return_enum {
//  DB_ERR_START                = DB_ERR_BASE  ,
    DB_ERR_START                = 0x700       ,        
    DB_ERR_CFG_FILE_ERROR                      ,
/*  DB_ERR_INVALID_IP_ADDR                     ,
    DB_ERR_INVALID_SUBNET_MASK                 ,
    DB_ERR_INVALID_ADMIN_STATUS                ,
    DB_ERR_INVALID_BUNDLE_NUMBER               ,
    DB_ERR_INVALID_SRC_BUNDLE_ID               ,
    DB_ERR_INVALID_DST_BUNDLE_ID               ,
    DB_ERR_DEST_BUNDLE_ID_BUSY                 ,
*/  DB_ERR_IP_ADDR_STR                         ,
    DB_ERR_MAC_ADDR_STR                 ,
    DB_ERR_END     ,   /* XXX: Insert all DB err codes above this line. This is the boundary */
    DB_SUCCESS   = 0   /* XXX: Keep it always as the last member of this enum */
} db_return_et ;



#endif /*wfps_db_enm_defs.h*/
