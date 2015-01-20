/*
 *  @file   cmgr_types.h
 *
 *  @brief  This file contains the Conn Mgr internal data-type definitions
 *
 *  @author Sri Harsha V
 *
 */
#ifndef __CMGR_TYPES_H__
#define __CMGR_TYPES_H__

#include "cmgr_int_types.h"
#include "cmgr_ecodes.h"
#include "cmgr_defs.h"


/*
 * @struct  cmgr_sock_data_buff_st
 *
 * @brief   This structure defines the storage pattern for socket data buffer. It could be used for
 *          the sending and receiving buffer over a socket.
 */
typedef struct cmgr_sock_data_buff_tag {
    cmgr_s8bit      *p_buff         ;
    cmgr_s32bit     buff_len        ;
    cmgr_s32bit     pend_buff_len   ;

} cmgr_sock_data_buff_st   ;


/*
 * @struct  cmgr_conn_info_st
 *
 * @brief   This structure defines the database type for maintaining connection/FD info per user
 */
typedef struct cmgr_conn_info_tag {
    cmgr_sock_fd_t          sock_fd     ;
    cmgr_sock_type_et       sock_type   ;
    cmgr_fd_state_et        fd_state    ;
    cmgr_ip_addr_st         local_addr  ;
    cmgr_ip_addr_st         rem_addr    ;
    cmgr_sock_data_buff_st  send_buff_info  ; /* Both of these won't be currently used */
    cmgr_sock_data_buff_st  recv_buff_info  ;

} cmgr_conn_info_st  ;


/*
 * @struct  cmgr_user_fd_info_st
 *
 * @brief   This structure defines the database type for maintaining User opened FDs
 */
typedef struct cmgr_user_fd_info_tag {
    cmgr_sock_fd_t              fd_id       ;
    cmgr_user_fd_handler_fn_t   fd_handler  ;
    void                        *fd_args    ;

} cmgr_user_fd_info_st  ;


/*
 * @struct  cmgr_module_info_st
 *
 * @brief   This structure stores module information like mapping betweem mod-id and Address
 */
typedef struct cmgr_user_info_tag {
    cmgr_mod_id_t               module_id   ;
    cmgr_ip_addr_st             module_addr ;

} cmgr_module_info_st  ;

#endif /* End of __CMGR_TYPES_H__ */
