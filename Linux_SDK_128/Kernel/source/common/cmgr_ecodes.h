/*
 *  @file   cmgr_ecodes.h
 *
 *  @brief  This file contains the error code definitions. To be included by the user apps
 *          and the Connection Manager library source file itself.
 *
 *  @author 
 *
 */
#ifndef __CMGR_ECODES_H__
#define __CMGR_ECODES_H__

/*
 * @enum  cmgr_ecode_t
 *
 * @brief   This enumeration is used for indicating the error types
 */
typedef enum {
    CMGR_ECODE_NO_ERR = 0           ,
    CMGR_ECODE_MEM_ALLOC_FAILURE    ,
    CMGR_ECODE_MODULE_SHUTDOWN_INIT ,
    CMGR_ECODE_USER_DB_UNINIT       ,
    CMGR_ECODE_INTERNAL_ERROR       ,
    CMGR_ECODE_INIT_FAILURE         ,
    CMGR_ECODE_BIND_FAILURE         ,
    CMGR_ECODE_SOCKET_FAILURE       ,
    CMGR_ECODE_PARAM_PASSED_NULL    ,
    CMGR_ECODE_USER_DBLK_ABSENT     ,
    CMGR_ECODE_CONN_DBLK_ABSENT     ,
    CMGR_ECODE_DUPLICATE_INSERTION  ,
    CMGR_ECODE_USER_DB_FULL         ,
    CMGR_ECODE_CONN_DB_FULL         ,
    CMGR_ECODE_RECVFROM_TIMEOUT     ,

} cmgr_ecode_t  ;


typedef enum cfgm_error_codes_enum{
        CFGM_ERR_START ,
        CFGM_CONN_MGR_INIT_ERROR,
        CFGM_INIT_ERROR,
        CFGM_CONN_MGR_OPEN_FAILED,      
        CFGM_ERR_END = -1,
        CFGM_SUCCESS = 1,
}CFGM_ERROR_CODES_T;

typedef CFGM_ERROR_CODES_T      cfg_return_t;

typedef enum mac_upld_error_codes_enum{
        MAC_UPLD_MGR_ERR_START ,
        MAC_UPLD_MGR_CONN_MGR_INIT_ERROR,
        MAC_UPLD_MGR_INIT_ERROR,
        MAC_UPLD_MGR_CONN_MGR_OPEN_FAILED,      
        MAC_UPLD_MGR_ERR_END = -1,
        MAC_UPLD_MGR_SUCCESS = 1,
}MAC_UPLD_MGR_ERROR_CODES_T;

typedef MAC_UPLD_MGR_ERROR_CODES_T      mac_upld_return_t;
#endif /* End of __CMGR_ECODES_H__ */
