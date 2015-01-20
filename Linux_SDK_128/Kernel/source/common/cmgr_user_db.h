/*
 *  @file   cmgr_user_db.h
 *
 *  @brief  This file contains the Conn Mgr User database type.
 *
 *  @author
 *
 */
#ifndef __CMGR_USER_DB_H__
#define __CMGR_USER_DB_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "cmgr_types.h"


/*
 * @struct  cmgr_global_user_db_st
 *
 * @brief   This structure defines the storage pattern for global database maintained for all the
 *          users of the connection manager library
 */
typedef struct cmgr_global_user_db_tag {
    cmgr_s16bit                 num_users   ;
    cmgr_sock_fd_t              max_sock_fd_id  ;
    cmgr_mod_id_t               self_mod_id ;
    cmgr_mesg_handler_fn_t      msg_handler_fn  ;
    cmgr_timeout_handler_fn_t   timeout_handler_fn  ;
    struct timeval              timeout_dura    ;
    cmgr_s8bit                  user_id_str[CMGR_MAX_USER_STR_LEN]  ;
    cmgr_module_info_st         rem_mod_info[CMGR_MAX_NUM_USERS]    ;
    cmgr_conn_info_st           conn_info[CMGR_MAX_NUM_FD_PER_USER] ;
    cmgr_user_fd_info_st        user_fd_info    ;

} cmgr_global_user_db_st   ;


cmgr_return_t
cmgr_create_conn_user_db (
    IN  cmgr_mod_id_t   mod_id  ,
    OUT cmgr_ecode_t    *p_error) ;

cmgr_return_t
cmgr_remove_conn_user_db (
    IN  cmgr_global_user_db_st  *p_user_dblk) ;

cmgr_return_t
cmgr_get_user_dblk_for_mod_id (
    IN  cmgr_mod_id_t           mod_id  ,
    OUT cmgr_global_user_db_st  **pp_usr_dblk)  ;

cmgr_return_t
cmgr_db_get_free_conn_dblk (
    IN  cmgr_global_user_db_st  *p_user_dblk    ,
    OUT cmgr_conn_info_st       **pp_conn_info  ,
    OUT cmgr_ecode_t            *p_error        ) ;

cmgr_return_t
cmgr_db_remove_conn_from_db (
    IN  cmgr_global_user_db_st  *p_user_dblk,
    OUT cmgr_sock_fd_t          conn_id     ,
    OUT cmgr_ecode_t            *p_error    ) ;

void
cmgr_db_fill_read_fdset (
    IN  cmgr_global_user_db_st  *p_usr_dblk ,
    OUT fd_set                  *p_fdset    ) ;

cmgr_return_t
cmgr_db_proc_select_event (
    IN  cmgr_global_user_db_st  *p_usr_dblk ,
    IN  cmgr_s8bit              buff[]      ,
    IN  cmgr_s16bit             buf_maxlen  ,
    IN  cmgr_s16bit             flags       ,
    IN  struct sockaddr_in      *p_sock     ,
    IN  fd_set                  *p_fdset    ,
    OUT cmgr_ecode_t            *p_error    )   ;

cmgr_return_t
cmgr_db_get_addr_for_module (
    IN  cmgr_global_user_db_st  *p_usr_dblk     ,
    IN  cmgr_mod_id_t           rem_module_id   ,
    OUT cmgr_ip_addr_st         *p_rem_addr     ,
    OUT cmgr_ecode_t            *p_error        ) ;

#endif /* End of __CMGR_USER_DB_H__ */
