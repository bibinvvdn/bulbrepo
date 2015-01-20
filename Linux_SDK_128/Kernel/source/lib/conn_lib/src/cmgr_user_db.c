/*
 *  @file   cmgr_user_db.c
 *
 *  @brief This file contains the definitions of the Connection Manager user database
 *
 *  @author Sri Harsha V
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "cmgr_user_db.h"

/* Introducing global DB table */
cmgr_global_user_db_st     *gp_cmgr_glb_db[CMGR_MAX_NUM_USERS] ;

/* Right now many of these functions are not thread-safe. Next version of the code
 * would be aimed at addressing these issues */

cmgr_return_t
cmgr_create_conn_user_db (
    IN  cmgr_mod_id_t   mod_id  ,
    OUT cmgr_ecode_t    *p_error)
{
    cmgr_u16bit                 cntr = 0 ;

        /* First check if the User DB is full */
    for (; (cntr < CMGR_MAX_NUM_USERS) && (gp_cmgr_glb_db[cntr]); cntr++) 
        if (gp_cmgr_glb_db[cntr]->self_mod_id == mod_id) {
            *p_error = CMGR_ECODE_DUPLICATE_INSERTION ;
            return CMGR_FAILURE ;
        }
        ;
    if (cntr == CMGR_MAX_NUM_USERS) {
        *p_error = CMGR_ECODE_USER_DB_FULL ;
        return CMGR_FAILURE ;
    }
        /* First allocate memory from the heap for Conn Mgr Internal database */
    if (NULL == (gp_cmgr_glb_db[cntr] = (cmgr_global_user_db_st *)__MALLOC__(
                    sizeof(cmgr_global_user_db_st)))) {
        perror (NULL) ;
        *p_error = CMGR_ECODE_MEM_ALLOC_FAILURE ;
        return CMGR_FAILURE ;
    } else {
        bzero ((cmgr_s8bit *)gp_cmgr_glb_db[cntr], sizeof(cmgr_global_user_db_st)) ;
    }
    gp_cmgr_glb_db[cntr]->self_mod_id = mod_id ;
    gp_cmgr_glb_db[cntr]->timeout_dura.tv_sec = 5 ;
    gp_cmgr_glb_db[cntr]->timeout_dura.tv_usec = 0 ;

    return CMGR_SUCCESS ;
}

cmgr_return_t
cmgr_remove_conn_user_db (
    IN  cmgr_global_user_db_st  *p_user_dblk)
{
    cmgr_u16bit                 cntr = 0 ;
    if (!p_user_dblk)
        return CMGR_SUCCESS ;

        /* Try to find the block */
    for (; (cntr < CMGR_MAX_NUM_USERS) && (p_user_dblk != gp_cmgr_glb_db[cntr]); cntr++)
        ;
    /* Check if some record was found or not */
    if (cntr == CMGR_MAX_NUM_USERS) {
        return CMGR_FAILURE ;
    } else {
        __FREE__ (p_user_dblk) ;
        gp_cmgr_glb_db[cntr] = NULL;
    }

    return CMGR_SUCCESS ;
}

cmgr_return_t
cmgr_get_user_dblk_for_mod_id (
    IN  cmgr_mod_id_t           mod_id  ,
    OUT cmgr_global_user_db_st  **pp_usr_dblk)
{
    cmgr_u16bit                 cntr = 0 ;
    /* No need for error checking. This is an CMGR Internal function. CMGR shall
     * ensure that it's never passed NULL parameters */
    *pp_usr_dblk = NULL ;

    for (; cntr < CMGR_MAX_NUM_USERS; cntr++) {
        if ((gp_cmgr_glb_db[cntr]) &&
            (gp_cmgr_glb_db[cntr]->self_mod_id == mod_id))
            break ;
    }
    /* Check if some record was found or not */
    if (cntr == CMGR_MAX_NUM_USERS) {
        return CMGR_FAILURE ;
    } else {
        *pp_usr_dblk = gp_cmgr_glb_db[cntr] ;
    }

    return CMGR_SUCCESS ;
}

cmgr_return_t
cmgr_db_get_free_conn_dblk (
    IN  cmgr_global_user_db_st  *p_user_dblk    ,
    OUT cmgr_conn_info_st       **pp_conn_info  ,
    OUT cmgr_ecode_t            *p_error        )
{
    cmgr_s16bit     lp_cntr ;

    for (lp_cntr = 0; (lp_cntr < CMGR_MAX_NUM_FD_PER_USER) && 
            (0 != p_user_dblk->conn_info[lp_cntr].sock_fd); lp_cntr++)
        ;
    /* Check if something was found or not */
    if (lp_cntr != CMGR_MAX_NUM_FD_PER_USER) {
        *pp_conn_info = &p_user_dblk->conn_info[lp_cntr] ;
        return CMGR_SUCCESS ;
    }

    *p_error = CMGR_ECODE_CONN_DB_FULL ;
    return  CMGR_FAILURE ;
}

cmgr_return_t
cmgr_db_remove_conn_from_db (
    IN  cmgr_global_user_db_st  *p_user_dblk,
    OUT cmgr_sock_fd_t          conn_id     ,
    OUT cmgr_ecode_t            *p_error    )
{
    cmgr_s16bit     lp_cntr ;

    for (lp_cntr = 0; (lp_cntr < CMGR_MAX_NUM_FD_PER_USER) && 
            (conn_id != p_user_dblk->conn_info[lp_cntr].sock_fd); lp_cntr++)
        ;
    /* Check if something was found or not */
    if (lp_cntr != CMGR_MAX_NUM_FD_PER_USER) {
        cmgr_conn_info_st       *p_conn_blk = &p_user_dblk->conn_info[lp_cntr] ;

            /* Now de-init the connection block */
        bzero ((cmgr_s8bit *)p_conn_blk, sizeof (cmgr_conn_info_st)) ;
            /* Now few more things to be done */
        if (conn_id == p_user_dblk->max_sock_fd_id) {
            /* We need to find the new bigger max */
            p_user_dblk->max_sock_fd_id = 0 ;
            for (lp_cntr = 0; lp_cntr < CMGR_MAX_NUM_FD_PER_USER; lp_cntr++)
                p_user_dblk->max_sock_fd_id = (p_user_dblk->conn_info[lp_cntr].sock_fd > 
                    p_user_dblk->max_sock_fd_id)? (p_user_dblk->conn_info[lp_cntr].sock_fd):
                    (p_user_dblk->max_sock_fd_id) ;
        }
    }

    *p_error = CMGR_ECODE_CONN_DBLK_ABSENT ;
    return  CMGR_FAILURE ;
}

void
cmgr_db_fill_read_fdset (
    IN  cmgr_global_user_db_st  *p_usr_dblk ,
    OUT fd_set                  *p_fdset    )
{
    cmgr_s16bit     cntr = 0 ;

    FD_ZERO (p_fdset) ;
    for (; cntr < CMGR_MAX_NUM_FD_PER_USER; cntr++) {
        if(0 != p_usr_dblk->conn_info[cntr].sock_fd)
            FD_SET (p_usr_dblk->conn_info[cntr].sock_fd, p_fdset) ;
    }
        /* Now add the User-FD into it */
    if (p_usr_dblk->user_fd_info.fd_id)
        FD_SET (p_usr_dblk->user_fd_info.fd_id, p_fdset) ;


    return ;
}

cmgr_return_t
cmgr_db_proc_select_event (
    IN  cmgr_global_user_db_st  *p_usr_dblk ,
    IN  cmgr_s8bit              buff[]      ,
    IN  cmgr_s16bit             buf_maxlen  ,
    IN  cmgr_s16bit             flags       ,
    IN  struct sockaddr_in      *p_sock     ,
    IN  fd_set                  *p_fdset    ,
    OUT cmgr_ecode_t            *p_error    )
{
    cmgr_return_t       ret_val = CMGR_SUCCESS  ;
    cmgr_s16bit         cntr = 0 ;

    for (; cntr < CMGR_MAX_NUM_FD_PER_USER; cntr++) {
        cmgr_s16bit         buff_len = 0    ;
        socklen_t           saddr_size = sizeof (*p_sock) ;
        cmgr_ip_addr_st     rem_srcaddr = {0} ;

            /* Check each FD if it's set or not */
        if (!p_usr_dblk->conn_info[cntr].sock_fd)
            continue ;
        else if (!FD_ISSET (p_usr_dblk->conn_info[cntr].sock_fd, p_fdset))
            continue ;
        else if (CMGR_SOCK_TYPE_UDP_SVR_CLNT == p_usr_dblk->conn_info[cntr].sock_type) {
                /* Else we receive the message */
            buff_len = recvfrom (p_usr_dblk->conn_info[cntr].sock_fd, buff, 
                            buf_maxlen, flags, (struct sockaddr *)p_sock, &saddr_size) ;
            if (0 > buff_len) {
                perror (NULL) ;
                ret_val = CMGR_FAILURE ;
            } else {
                /* Find the source address */
                rem_srcaddr.ip_addr = ntohl (p_sock->sin_addr.s_addr) ;
                rem_srcaddr.port    = ntohs (p_sock->sin_port) ;
            }
            if (CMGR_SUCCESS != ret_val) {
                continue ;
            } else if (CMGR_SUCCESS != p_usr_dblk->msg_handler_fn (buff, buff_len,
                        rem_srcaddr, p_usr_dblk->conn_info[cntr].sock_fd, p_error)) {
                ret_val = CMGR_FAILURE ;
            }
						return ret_val;
        }
    }
        /* Now also check whether the User-FD is set or not */
    if (FD_ISSET (p_usr_dblk->user_fd_info.fd_id, p_fdset)) {
        if (CMGR_SUCCESS != p_usr_dblk->user_fd_info.fd_handler (
                                p_usr_dblk->user_fd_info.fd_id, 
                                p_usr_dblk->user_fd_info.fd_args, p_error) ) {
            ret_val = CMGR_FAILURE ;
        }
        __FREE__ (buff) ;
    } else if (cntr >= CMGR_MAX_NUM_FD_PER_USER) {
        __FREE__ (buff) ;
	}


    return  ret_val ;
}

cmgr_return_t
cmgr_db_get_addr_for_module (
    IN  cmgr_global_user_db_st  *p_usr_dblk     ,
    IN  cmgr_mod_id_t           rem_module_id   ,
    OUT cmgr_ip_addr_st         *p_rem_addr     ,
    OUT cmgr_ecode_t            *p_error        )
{
    cmgr_s16bit     cntr = 0 ;

    for (; cntr < CMGR_MAX_NUM_USERS; cntr++)
        if (rem_module_id == p_usr_dblk->rem_mod_info[cntr].module_id) {
            *p_rem_addr = p_usr_dblk->rem_mod_info[cntr].module_addr ;
            return CMGR_SUCCESS ;
        }

    return CMGR_FAILURE ;
}

