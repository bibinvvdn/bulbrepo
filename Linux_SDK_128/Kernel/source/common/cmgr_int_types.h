/*
 *  @file   cmgr_int_types.h
 *
 *  @brief  This file contains the interfacing data-type definitions that will be used
 *          across all the users of this library for getting services of this library
 *
 *  @author 
 *
 */
#ifndef __CMGR_INT_TYPES_H__
#define __CMGR_INT_TYPES_H__

#include "cmgr_int_defs.h"
#include "cmgr_ecodes.h"

/* These are the definitions of the basic data-types. These can change later while porting */

/*
 * @typedef  cmgr_s8bit
 *
 * @brief   This type denotes signed 8 bit data-type
 */
typedef signed char     cmgr_s8bit  ;

/*
 * @typedef  cmgr_u8bit
 *
 * @brief   This type denotes unsigned 8 bit data-type
 */
typedef unsigned char   cmgr_u8bit  ;

/*
 * @typedef  cmgr_s16bit
 *
 * @brief   This type denotes signed 16 bit data-type
 */
typedef signed short    cmgr_s16bit ;

/*
 * @typedef  cmgr_u16bit
 *
 * @brief   This type denotes unsigned 16 bit data-type
 */
typedef unsigned short  cmgr_u16bit ;

/*
 * @typedef  cmgr_s32bit
 *
 * @brief   This type denotes signed 32 bit data-type
 */
typedef signed long     cmgr_s32bit ;

/*
 * @typedef  cmgr_u32bit
 *
 * @brief   This type denotes unsigned 32 bit data-type
 */
typedef unsigned long   cmgr_u32bit ;



/* @note    THESE ARE THE TYPES THAT MUST BE USED BY ALL THE INTERFACES */

/*
 * @typedef  cmgr_sock_fd_t
 *
 * @brief   This type denotes socket FD
 */
typedef cmgr_s16bit     cmgr_sock_fd_t  ;
typedef cmgr_sock_fd_t  cmgr_fd_id_t    ;


/*
 * @typedef  cmgr_ipaddr_t
 *
 * @brief   This type denotes IPv4 style numeric address
 */
typedef cmgr_u32bit     cmgr_ipaddr_t   ;

/*
 * @typedef  cmgr_port_t
 *
 * @brief   This type denotes Port number
 */
typedef cmgr_u16bit     cmgr_port_t     ;

/*
 * @typedef  cmgr_mod_id_t
 *
 * @brief   This type denotes Module-ID expressed as 32bit quantity. Modules must exchange
 *          their module/thread-ID with Connection Manager library using this type.
 */
typedef cmgr_u32bit     cmgr_mod_id_t   ;

/*
 * @struct  cmgr_ip_addr_st
 *
 * @brief   This structure has two members viz., port and ip_addr (32 bit numeric)
 */
typedef struct cmgr_ip_addr_tag {
    cmgr_port_t     port    ;
    cmgr_ipaddr_t   ip_addr ;

} cmgr_ip_addr_st   ;


/*
 * @typedef  cmgr_mesg_handler_fn_t
 *
 * @brief   This type denotes received msg handler function pointer type
 */
typedef cmgr_return_t (* cmgr_mesg_handler_fn_t)
(
    IN  cmgr_s8bit          *rcvd_buff  ,
    IN  cmgr_s16bit         buff_len    ,
    IN  cmgr_ip_addr_st     rem_srcaddr ,
    IN  cmgr_sock_fd_t      conn_id     ,
    OUT cmgr_ecode_t        *p_ecode
) ;

/*
 * @typedef  cmgr_timeout_handler_fn_t
 *
 * @brief   This type denotes socket timeout handler function pointer type
 */
typedef cmgr_return_t (* cmgr_timeout_handler_fn_t)
(
    OUT cmgr_ecode_t        *p_ecode
) ;

/*
 * @typedef  cmgr_user_fd_handler_fn_t
 *
 * @brief   This type denotes function handler pointer to process data on User fd
 */
typedef cmgr_return_t (* cmgr_user_fd_handler_fn_t)
(
    IN  cmgr_fd_id_t    fd_id   ,
    IN  void            *fd_arg ,
    OUT cmgr_ecode_t    *p_ecode
) ;

#endif /* End of __CMGR_INT_TYPES_H__ */
