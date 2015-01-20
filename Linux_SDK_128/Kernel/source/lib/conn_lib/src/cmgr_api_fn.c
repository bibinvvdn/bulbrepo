/*
 *  @file   cmgr_api_fn.c
 *
 *  @brief This file contains the definitions of the interfacing API functions.
 *
 *  @author Sri Harsha V
 *
 */

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>
#include "cmgr_int_api.h"
#include "cmgr_defs.h"
#include "cmgr_types.h"
#include "cmgr_user_db.h"
#include "pthread.h"

/*pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
/int cli_init_status=0;*/

/*
 * @fn  cmgr_init_conn_lib
 *
 * @brief   This function is used by the applications, using the services of the connection
 *          manager library, for initializing the conn manager library in it's context. This
 *          func returns the unique user-id of the application w.r.t conn mgr lib and can return
 *          error code in case func fails.
 *
 * @param[in]   module_id               This is the applications module/thread-id
 * @param[in]   mesg_handler_fn         This is user apps func handler ptr. This handler should be
 *                                      provided by the user app and defines how the app is going
 *                                      to process the received messages on its socket.
 * @param[in]   timeout_handler_fn      This is the function pointer of the timeout handling 
 *                                      callback function
 * @param[in]   p_mod_name_str          The user application's (module) name, a literal string
 * @param[out]  p_error                 Return error code value
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */
cmgr_return_t
cmgr_init_conn_lib (
    IN  cmgr_mod_id_t               module_id           ,
    IN  cmgr_mesg_handler_fn_t      mesg_handler_fn     ,
    IN  cmgr_timeout_handler_fn_t   timeout_handler_fn  ,
    IN  cmgr_s8bit                  *p_mod_name_str     ,
    OUT cmgr_ecode_t                *p_error            )
{
    cmgr_return_t           ret_val = CMGR_SUCCESS  ;
    cmgr_global_user_db_st  *p_usr_dblk = NULL      ;

        /* First check if the pointer params are NULL or not and then try to find free position in
         * the internal database to store the passed parameters of the module */
    if (NULL == p_error) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Ecode return buffer passed NULL\n") ) ;
        ret_val = CMGR_FAILURE ;

    } else if (NULL == p_mod_name_str) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module name string passed NULL\n") ) ;
        *p_error = CMGR_ECODE_PARAM_PASSED_NULL ;
        ret_val = CMGR_FAILURE ;

    } else if (NULL == mesg_handler_fn) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Message handler pointer passed NULL\n") ) ;
        *p_error = CMGR_ECODE_PARAM_PASSED_NULL ;
        ret_val = CMGR_FAILURE ;

    } else if (NULL == timeout_handler_fn) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Timeout handler pointer passed NULL\n") ) ;
        *p_error = CMGR_ECODE_PARAM_PASSED_NULL ;
        ret_val = CMGR_FAILURE ;

    } else if (CMGR_FAILURE == cmgr_create_conn_user_db (module_id, p_error)) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] already initialized\n", module_id) ) ;
        ret_val = CMGR_FAILURE ;
    } else if (CMGR_FAILURE == cmgr_get_user_dblk_for_mod_id (module_id, &p_usr_dblk) ) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] not found in DB even after initialisation\n", 
                        module_id) ) ;
        *p_error = CMGR_ECODE_USER_DBLK_ABSENT ;
        ret_val = CMGR_FAILURE ;
    } else {
        /* Now start filling in the values */
        p_usr_dblk->self_mod_id        = module_id ;
        p_usr_dblk->msg_handler_fn     = mesg_handler_fn   ;
        p_usr_dblk->timeout_handler_fn = timeout_handler_fn    ;
        snprintf ((char *)p_usr_dblk->user_id_str, 
                CMGR_MAX_USER_STR_LEN - 1, "%s", p_mod_name_str);
        p_usr_dblk->user_id_str[CMGR_MAX_USER_STR_LEN - 1] = '\0' ;
    }

    return  ret_val ;
}


/*
 * @fn  cmgr_open_conn
 *
 * @brief   This function opens the socket for the user application.
 *
 * @param[in]   module_id   This is the applications module/thread-id
 * @param[in]   p_ip_port   pointer to the structure containing local IP & Port.
 * @param[in]   sock_type   Enumerated type identifying whether UDP/TCP socket is to be opened.
 * @param[out]  p_conn_id   Connection-ID (FD) of the socket opened by Conn Manager. Returned
 *                          back to the user app through this pointer argument.
 * @param[out]  p_error     Return error code value
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */
cmgr_return_t
cmgr_open_conn (
    IN  cmgr_mod_id_t       module_id   ,
    IN  cmgr_ip_addr_st     *p_ip_port  ,
    IN  cmgr_sock_type_et   sock_type   ,
    OUT cmgr_sock_fd_t      *p_conn_id  ,
    OUT cmgr_ecode_t        *p_error    )
{
    cmgr_conn_info_st       *p_conn_dblk = NULL ;
    struct sockaddr_in      serv_addr   ;
    cmgr_sock_fd_t          conn_id = 0 ;
    cmgr_return_t           ret_val = CMGR_SUCCESS  ;
    cmgr_global_user_db_st  *p_usr_dblk = NULL      ;

        /* First check if the pointer params are NULL or not */
    if (NULL == p_error) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Ecode return buffer passed NULL\n") ) ;
        ret_val = CMGR_FAILURE ;

    } else if (NULL == p_ip_port) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Address info buff passed NULL\n") ) ;
        *p_error = CMGR_ECODE_PARAM_PASSED_NULL ;
        ret_val = CMGR_FAILURE ;

    } else if (NULL == p_conn_id) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Conn ID return buffer passed NULL\n") ) ;
        *p_error = CMGR_ECODE_PARAM_PASSED_NULL ;
        ret_val = CMGR_FAILURE ;

    } else if (CMGR_FAILURE == cmgr_get_user_dblk_for_mod_id (module_id, &p_usr_dblk) ) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] not found in DB \n", module_id) ) ;
        *p_error = CMGR_ECODE_USER_DBLK_ABSENT ;
        ret_val = CMGR_FAILURE ;
    } else if (CMGR_SOCK_TYPE_UDP_SVR_CLNT == sock_type) {
        /* Probable reason of failure is really low, so init sockaddr_in beforehand */
        bzero (&serv_addr, sizeof(serv_addr)) ;
        serv_addr.sin_family = PF_INET  ;
        serv_addr.sin_addr.s_addr = htonl (p_ip_port->ip_addr) ;
        serv_addr.sin_port = htons (p_ip_port->port) ;

        if (0 > (conn_id = (cmgr_sock_fd_t)socket (AF_INET, SOCK_DGRAM, 0))) {
            perror ("[CMGR] Socket Error ") ;
            CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_CRITICAL,
                    ("[CMGR] Err! Can't open socket FD for Module [%lu]\n", module_id) ) ;
            *p_error = CMGR_ECODE_SOCKET_FAILURE ;
            ret_val = CMGR_FAILURE ;

        } else if (0 > bind ((int)conn_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
            perror ("[CMGR] Bind Error ") ;
            CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_CRITICAL,
                    ("[CMGR] Err! Binding Addr [0x%lx:%d] with FD [%d] for Module [%lu]\n", 
                        p_ip_port->ip_addr, p_ip_port->port, conn_id, module_id) ) ;
            close (conn_id) ;
            *p_error = CMGR_ECODE_BIND_FAILURE ;
            ret_val = CMGR_FAILURE ;

        } else if (CMGR_SUCCESS != cmgr_db_get_free_conn_dblk (p_usr_dblk, 
                                                    &p_conn_dblk, p_error)) {
            CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_CRITICAL,
                    ("[CMGR] Err! Conn DB limit reached for User [%lu]\n", module_id) ) ;
            close (conn_id) ;
            ret_val = CMGR_FAILURE ;

        } else {
            p_conn_dblk->sock_fd    = conn_id ;
            p_conn_dblk->sock_type  = CMGR_SOCK_TYPE_UDP_SVR_CLNT ;
            p_conn_dblk->fd_state   = CMGR_FD_STATE_READY ;
            p_conn_dblk->local_addr = *p_ip_port ;
            p_usr_dblk->max_sock_fd_id = (p_usr_dblk->max_sock_fd_id < conn_id)?
                                                (conn_id): (p_usr_dblk->max_sock_fd_id) ;
            *p_conn_id = conn_id ;
        }
    } else {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Connection can't be open for Socket Type [%u]\n", sock_type) ) ;
        ret_val = CMGR_FAILURE ;
    }

    return  ret_val ;
}


/*
 * @fn  cmgr_add_rem_conn_params
 *
 * @brief   This function adds the remote module's connection parameter internally into it's
 *          database
 *
 * @param[in]   self_mod_id     Self module-ID of the user application
 * @param[in]   rem_mod_id      Remote applications module-ID
 * @param[in]   p_rem_ip_port   pointer to the structure containing remote IP & Port.
 * @param[in]   sock_type       Enumerated type identifying whether UDP/TCP socket is to be opened.
 * @param[out]  p_error         Return error code value
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */
cmgr_return_t
cmgr_add_rem_conn_params (
    IN  cmgr_mod_id_t       self_mod_id     ,
    IN  cmgr_mod_id_t       rem_mod_id      ,
    IN  cmgr_ip_addr_st     *p_rem_ip_port  ,
    IN  cmgr_sock_type_et   sock_type       ,
    OUT cmgr_ecode_t        *p_error        )
{
    cmgr_return_t           ret_val = CMGR_SUCCESS  ;
    cmgr_global_user_db_st  *p_usr_dblk = NULL      ;

        /* First check if the pointer params are NULL or not */
    if (NULL == p_error) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Ecode return buffer passed NULL\n") ) ;
        ret_val = CMGR_FAILURE ;

    } else if (NULL == p_rem_ip_port) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Remote IP-Port param passed NULL\n") ) ;
        *p_error = CMGR_ECODE_PARAM_PASSED_NULL ;
        ret_val = CMGR_FAILURE ;

    } else if (CMGR_FAILURE == cmgr_get_user_dblk_for_mod_id (self_mod_id, &p_usr_dblk) ) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] not found in DB \n", self_mod_id) ) ;
        *p_error = CMGR_ECODE_USER_DBLK_ABSENT ;
        ret_val = CMGR_FAILURE ;
    } else {
        /* Note: Right now sock_type is useless for us. This might be useful for TCP sockets */
        p_usr_dblk->rem_mod_info[p_usr_dblk->num_users].module_id = rem_mod_id ;
        p_usr_dblk->rem_mod_info[p_usr_dblk->num_users].module_addr = *p_rem_ip_port ;
        p_usr_dblk->num_users++ ;
    }

    return ret_val ;
}


/*
 * @fn  cmgr_add_user_fd
 *
 * @brief   This function the FD's obtained by the user (For e.g. FD for file open etc) into
 *          the connection manager library. User app has to provide the FD ID, the handler
 *          function etc for this FD
 *
 * @param[in]   module_id       This is the applications module/thread-id
 * @param[in]   fd_id           The user opened FD-ID
 * @param[in]   fd_handler      The func ptr to the handler. This function shall actually do all
 *                              thing, like reading the data on this FD (calling read etc.), and
 *                              then processing that data. Conn Mgr, since doesn't know the nature
 *                              of this FD, thus avoids applying generic "read" like calls.
 * @param[in]   p_callbk_arg    This argument is passed back to the user FD handler. It can be 
 *                              passed NULL, if not reqd.
 * @param[out]  p_error         Return error code value
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */
cmgr_return_t
cmgr_add_user_fd (
    IN  cmgr_mod_id_t               module_id       ,
    IN  cmgr_fd_id_t                fd_id           ,
    IN  cmgr_user_fd_handler_fn_t   fd_handler      ,
    IN  void                        *p_callbk_arg   ,
    OUT cmgr_ecode_t                *p_error        )
{
    cmgr_return_t           ret_val = CMGR_SUCCESS ;
    cmgr_global_user_db_st  *p_usr_dblk = NULL      ;

        /* First let's find the user datablock */
    if (CMGR_FAILURE == cmgr_get_user_dblk_for_mod_id (module_id, &p_usr_dblk) ) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] not found in DB \n", module_id) ) ;
        *p_error = CMGR_ECODE_USER_DBLK_ABSENT ;
        ret_val = CMGR_FAILURE ;
    }
    /* First get a free connection datablock */
    p_usr_dblk->user_fd_info.fd_id = fd_id ;
    p_usr_dblk->user_fd_info.fd_handler = fd_handler ;
    p_usr_dblk->user_fd_info.fd_args = p_callbk_arg ;
        /* Now find the Max FD-ID */
    p_usr_dblk->max_sock_fd_id = (p_usr_dblk->max_sock_fd_id < fd_id)?
                            (fd_id): (p_usr_dblk->max_sock_fd_id) ;

    return ret_val ;
}


/*
 * @fn  cmgr_proc_conn_msg
 *
 * @brief   This function starts listening onto the socket. As soon as any packet is received
 *          on the socket it invokes the callback handler provided by the user application
 *          during conn mgr initiation.
 *
 * @param[in]   module_id   User App's self module-id
 * @param[out]  p_error     Return error code value
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */
cmgr_return_t
cmgr_proc_conn_msg (
    IN  cmgr_mod_id_t   module_id   ,
    OUT cmgr_ecode_t    *p_error    )
{
    fd_set                  conn_readset    ;
    cmgr_s8bit              *buff = NULL;
    cmgr_s8bit              buff_reuse;
    cmgr_global_user_db_st  *p_usr_dblk = NULL      ;

        /* First check if the pointer params are NULL or not */
    if (NULL == p_error) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Ecode return buffer passed NULL\n") ) ;
        return CMGR_FAILURE ;

    } else if (CMGR_FAILURE == cmgr_get_user_dblk_for_mod_id (module_id, &p_usr_dblk) ) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] not found in DB \n", module_id) ) ;
        *p_error = CMGR_ECODE_USER_DBLK_ABSENT ;
        return CMGR_FAILURE ;
    }
    buff_reuse = 0;

        /* Now initialise the readfd set for select system call */
    /* Now start the infinite loop */
    for (;;) {
        struct sockaddr_in      serv_addr   ;
        cmgr_s16bit     flags = 0, selRet = -1 ;
        struct timeval  tmVal ;
        cmgr_return_t   ret_val = CMGR_SUCCESS ;

        *p_error = CMGR_ECODE_NO_ERR ;  /* Initialise error at every step */
        if (buff_reuse != 1) {
            if (!(buff = __MALLOC__(CMGR_MAX_SOCK_BUFF_LEN))) {
                CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_CRITICAL,
                    ("[CMGR] Err !!! Malloc failed while trying to listening on socket\n" )) ;
                return CMGR_FAILURE ;
            }
        }
        bzero ((char *)&serv_addr, sizeof(serv_addr)) ;
        bzero (buff, CMGR_MAX_SOCK_BUFF_LEN) ;
        tmVal = p_usr_dblk->timeout_dura   ;
        cmgr_db_fill_read_fdset (p_usr_dblk, &conn_readset) ;
#if 0
        if(cli_init_status == 1) {
                pthread_mutex_unlock(&mutex1);
                cli_init_status = 0;
        }
#endif
        selRet = select (p_usr_dblk->max_sock_fd_id + 1, &conn_readset, NULL, NULL, &tmVal) ;
        if ((0 > selRet) && (EINTR == errno)) {
            buff_reuse = 1;
            continue ;
        } else if (0 > selRet) {
            perror ("[CMGR] Select error ") ;
            return CMGR_FAILURE ;
        } else if (0 == selRet) {
            /* Timeout has occured. Call the timeout handler of the module */
            ret_val = p_usr_dblk->timeout_handler_fn (p_error) ;
            if ((CMGR_SUCCESS != ret_val) && (CMGR_ECODE_MODULE_SHUTDOWN_INIT == *p_error)) {
                __FREE__ (buff) ;
                buff_reuse = 0 ;
            }
            else
                buff_reuse = 1;
        } else {
            ret_val = cmgr_db_proc_select_event (p_usr_dblk, buff, CMGR_MAX_SOCK_BUFF_LEN,
                        flags, &serv_addr, &conn_readset, p_error) ;
            buff_reuse = 0;
        }
        if ((CMGR_SUCCESS != ret_val) && (CMGR_ECODE_MODULE_SHUTDOWN_INIT == *p_error))
        {
            CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                    ("[CMGR] MODULE [%lu] SHUTDOWN INITIATED. EXITING...\n", 
                     p_usr_dblk->self_mod_id) ) ;
            break ;
        }
    }

    return  CMGR_SUCCESS    ;
}
#if 0
/*
 * @fn  cmgr_proc_cli_msg
 *
 * @brief   This function starts listening onto the socket. As soon as any packet is received
 *          on the socket it invokes the callback handler provided by the user cli application
 *          during conn mgr initiation.
 *
 * @param[in]   module_id   User App's cli module-id
 * @param[out]  p_error     Return error code value
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */
cmgr_return_t
cmgr_proc_cli_msg (
    IN  cmgr_mod_id_t   module_id   ,
    OUT cmgr_ecode_t    *p_error    )
{
    fd_set                  conn_readset    ;
    cmgr_s8bit              *buff = NULL;
    cmgr_s8bit              buff_reuse;
    cmgr_global_user_db_st  *p_usr_dblk = NULL      ;

        /* First check if the pointer params are NULL or not */
    if (NULL == p_error) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Ecode return buffer passed NULL\n") ) ;
        return CMGR_FAILURE ;

    } else if (CMGR_FAILURE == cmgr_get_user_dblk_for_mod_id (module_id, &p_usr_dblk) ) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] not found in DB \n", module_id) ) ;
        *p_error = CMGR_ECODE_USER_DBLK_ABSENT ;
        return CMGR_FAILURE ;
    }
    buff_reuse = 0;
        /* Now initialise the readfd set for select system call */
    /* Now start the infinite loop */
    for (;;) {
        struct sockaddr_in      serv_addr   ;
        cmgr_s16bit     flags = 0, selRet = -1 ;
        struct timeval  tmVal ;
        cmgr_return_t   ret_val = CMGR_SUCCESS ;

        *p_error = CMGR_ECODE_NO_ERR ;  /* Initialise error at every step */
        if (buff_reuse != 1) {
            if (!(buff = __MALLOC__(CMGR_MAX_SOCK_BUFF_LEN))) {
                CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_CRITICAL,
                    ("[CMGR] Err !!! Malloc failed while trying to listening on socket\n" )) ;
                return CMGR_FAILURE ;
            }
        }
        bzero ((char *)&serv_addr, sizeof(serv_addr)) ;
        bzero (buff, CMGR_MAX_SOCK_BUFF_LEN) ;
        tmVal = p_usr_dblk->timeout_dura   ;
//        tmVal = 500;
        cmgr_db_fill_read_fdset (p_usr_dblk, &conn_readset) ;

        selRet = select (p_usr_dblk->max_sock_fd_id + 1, &conn_readset, NULL, NULL, &tmVal) ;
        if ((0 > selRet) && (EINTR == errno)) {
            buff_reuse = 1;
            break;
        } else if (0 > selRet) {
            perror ("[CMGR] Select error ") ;
            return CMGR_FAILURE ;
        } else if (0 == selRet) {
            /* Timeout has occured. Call the timeout handler of the module */
            ret_val = p_usr_dblk->timeout_handler_fn (p_error) ;
            if ((CMGR_SUCCESS != ret_val) && (CMGR_ECODE_MODULE_SHUTDOWN_INIT == *p_error)) {
                __FREE__ (buff) ;
                buff_reuse = 0 ;
                break;
            }
            else
                buff_reuse = 1;
                break;
        } else {
            ret_val = cmgr_db_proc_select_event (p_usr_dblk, buff, CMGR_MAX_SOCK_BUFF_LEN,
                        flags, &serv_addr, &conn_readset, p_error) ;
            buff_reuse = 0;
            break;
        }
        if ((CMGR_SUCCESS != ret_val) && (CMGR_ECODE_MODULE_SHUTDOWN_INIT == *p_error))
        {
            CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                    ("[CMGR] MODULE [%lu] SHUTDOWN INITIATED. EXITING...\n",
                     p_usr_dblk->self_mod_id) ) ;
            break ;
        }
    }
    return  CMGR_SUCCESS    ;
}

#endif

/*
 * @fn  cmgr_send_buff_to_module
 *
 * @brief   This function is used by the application to send a buffer to the remote peer identified
 *          by it's module-ID.
 *
 * @param[in]   self_mod_id User App's self module-id
 * @param[in]   conn_id     Connection ID of the user APP. Now here it's really important
 *                          to pass the connection-ID. 0 Won't DO !!!
 * @param[in]   buff        Pointer to the data buffer that needs to be sent.
 * @param[in]   buff_len    Input data buffer length
 * @param[in]   rem_mod_id  Module-ID of the remote peer to which data is being sent
 * @param[out]  p_error     Return error code value
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */
cmgr_return_t
cmgr_send_buff_to_module (
    IN  cmgr_mod_id_t       self_mod_id ,
    IN  cmgr_sock_fd_t      conn_id     ,
    IN  cmgr_s8bit          buff[]      ,
    IN  cmgr_u16bit         buff_len    ,
    IN  cmgr_mod_id_t       rem_mod_id  ,
    OUT cmgr_ecode_t        *p_error    )
{
    cmgr_ip_addr_st         rem_addr = {0} ;
    cmgr_global_user_db_st  *p_usr_dblk = NULL      ;

    if (NULL == p_error) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Ecode return buffer passed NULL\n") ) ;
        return CMGR_FAILURE ;

    } else if (CMGR_FAILURE == cmgr_get_user_dblk_for_mod_id (self_mod_id, &p_usr_dblk) ) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] not found in DB \n", self_mod_id) ) ;
        *p_error = CMGR_ECODE_USER_DBLK_ABSENT ;
        return CMGR_FAILURE ;
    } else if (CMGR_SUCCESS != cmgr_db_get_addr_for_module (p_usr_dblk, rem_mod_id, 
                &rem_addr, p_error)) {

        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Address for Module-ID [%lu] not found in User DB\n", rem_mod_id)) ;
        return CMGR_FAILURE ;

    } else if (CMGR_SUCCESS != cmgr_send_buff_to_addr (conn_id, buff,
                buff_len, &rem_addr, p_error)) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_CRITICAL,
                ("[CMGR] Err! Buff send to Module-ID [%lu] failed !!!\n", rem_mod_id)) ;
        return CMGR_FAILURE ;
    }
//    printf("cmd rec from cli in conn lib = %s\n",buff);
    return  CMGR_SUCCESS    ;
}


/*
 * @fn  cmgr_send_buff_to_addr
 *
 * @brief   This function is used by the application to send a buffer to the remote peer specified
 *          with it's IP Addr and Port.
 *
 * @param[in]   conn_id     Connection ID of the user APP. Now here it's really important
 *                          to pass the connection-ID. 0 Won't DO !!!
 * @param[in]   buff        Pointer to the data buffer that needs to be sent.
 * @param[in]   buff_len    Input data buffer length
 * @param[in]   p_rem_addr  Remote addr where data is to be sent
 * @param[out]  p_error     Return error code value
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */
cmgr_return_t
cmgr_send_buff_to_addr (
    IN  cmgr_sock_fd_t      conn_id     ,
    IN  cmgr_s8bit          buff[]      ,
    IN  cmgr_u16bit         buff_len    ,
    IN  cmgr_ip_addr_st     *p_rem_addr ,
    OUT cmgr_ecode_t        *p_error    )
{
    struct sockaddr_in      rem_sock    ;
    cmgr_s16bit             buff_sent = 0, flags = 0 ;

    if (NULL == p_error) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Ecode return buffer passed NULL\n") ) ;
        return CMGR_FAILURE ;
    } else if (NULL == p_rem_addr) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Remote Address buff passed NULL\n") ) ;
        *p_error = CMGR_ECODE_PARAM_PASSED_NULL ;
        return CMGR_FAILURE ;
    } else if (NULL == buff) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Send Buffer passed NULL\n") ) ;
        *p_error = CMGR_ECODE_PARAM_PASSED_NULL ;
        return CMGR_FAILURE ;
    } else {
        bzero ( (char *)&rem_sock, sizeof(rem_sock)) ;
        rem_sock.sin_family = PF_INET   ;
        rem_sock.sin_addr.s_addr = htonl (p_rem_addr->ip_addr);
        rem_sock.sin_port = htons (p_rem_addr->port) ;
    }

    if (0 > (buff_sent = sendto (conn_id, buff, buff_len, flags, 
                    (struct sockaddr *)&rem_sock, sizeof(rem_sock) ))) {
        perror ("[CMGR] Sendto Error ") ;
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_CRITICAL,
        ("[CMGR] Err! Sendto. conn_id: %u, buff: %p, buff_len: %u, flags: %u\n", 
             conn_id, buff, buff_len, flags) ) ;
        return CMGR_FAILURE ;
    }

    return  CMGR_SUCCESS    ;
}


/*
 * @fn  cmgr_close_conn
 *
 * @brief   This function closes a socket, identified by conn_id, opened by an application
 *
 * @param[in]   self_mod_id Self Module-ID of the user application
 * @param[in]   conn_id     Connection-ID that needs to be closed
 * @param[out]  p_error     Return error code value
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */
cmgr_return_t
cmgr_close_conn (
    IN  cmgr_mod_id_t       self_mod_id ,
    IN  cmgr_sock_fd_t      conn_id     ,
    OUT cmgr_ecode_t        *p_error    )
{
    cmgr_global_user_db_st  *p_usr_dblk = NULL      ;

    if (CMGR_FAILURE == cmgr_get_user_dblk_for_mod_id (self_mod_id, &p_usr_dblk) ) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] not found in DB \n", self_mod_id) ) ;
        *p_error = CMGR_ECODE_USER_DBLK_ABSENT ;
        return CMGR_FAILURE ;
    }
        /* De-Initialise the connection database */
    cmgr_db_remove_conn_from_db (p_usr_dblk, conn_id, p_error) ;
    close (conn_id) ;

    return  CMGR_SUCCESS    ;
}


/*
 * @fn  cmgr_deinit_conn_lib
 *
 * @brief   This function de-initialises the conn mgr lib DB for the calling application.
 *          This implies cancelling any pending data buffer, closing any pending open sockets etc.
 *
 * @param[in]   self_mod_id     User Application's module-id
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */
cmgr_return_t
cmgr_deinit_conn_lib (
    IN  cmgr_mod_id_t       self_mod_id )
{
    cmgr_global_user_db_st  *p_usr_dblk = NULL      ;

        /* First let's try to find out the User datablock for the calling module */
    if (CMGR_FAILURE == cmgr_get_user_dblk_for_mod_id (self_mod_id, &p_usr_dblk) ) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] not found in DB \n", self_mod_id) ) ;
        return CMGR_FAILURE ;
    }
    cmgr_remove_conn_user_db (p_usr_dblk) ;

    return  CMGR_SUCCESS    ;
}

/*
 * @fn  cmgr_set_timeout_duration
 *
 * @brief   This function sets the timeout duration for the calling module. Then this timeout
 *          is applied for it's select timeout timer value
 *
 * @param[in]   module_id       Self module-ID of the user application
 * @param[in]   timeout         Timeout period value (in milliseconds)
 * @param[out]  p_error         Return error code value
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */

cmgr_return_t
cmgr_set_timeout_duration
(   IN  cmgr_mod_id_t   module_id   ,
    IN  cmgr_u32bit     timeout     ,
    OUT cmgr_ecode_t    *p_error    )
{
    cmgr_global_user_db_st  *p_usr_dblk = NULL      ;

    if (NULL == p_error) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Ecode return buffer passed NULL\n") ) ;
        return CMGR_FAILURE ;

    } else if (CMGR_FAILURE == cmgr_get_user_dblk_for_mod_id (module_id, &p_usr_dblk) ) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] not found in DB \n", module_id) ) ;
        *p_error = CMGR_ECODE_USER_DBLK_ABSENT ;
        return CMGR_FAILURE ;
    }
    p_usr_dblk->timeout_dura.tv_sec = timeout / 1000 ; // Convert to seconds
    timeout %= 1000 ; // Get the sub-second part
    p_usr_dblk->timeout_dura.tv_usec = timeout * 1000 ; // Convert into micro-seconds

    return CMGR_SUCCESS ;
}

/*
 * @fn  cmgr_recv_buff_from_nw
 *
 * @brief   This function starts listening onto the socket. As soon as any packet is received
 *          on the socket it returns back to the caller with the read data filled into the
 *          passed buffer along with the buffer length and connection-ID. If it times-out then
 *          it returns with a failure tht receive has timed out.
 *
 * @param[in]       module_id   User App's self module-id
 * @param[in]       conn_id     Connection ID on which recvfrom is to be called
 * @param[in,out]   p_buff_len  Buffer length tht is passed to this function. Also this is used
 *                              return the length of the buffer read from the network
 * @param[in,out]   p_buff      Buffer passed by the application to connection Mgr. It is used
 *                              return buffer read from the socket.
 * @param[out]      p_error     Return error code value
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */
cmgr_return_t
cmgr_recv_buff_from_nw (
    IN      cmgr_mod_id_t   module_id   ,
    IN      cmgr_sock_fd_t  conn_id     ,
    INOUT   cmgr_s16bit     *p_buff_len ,
    INOUT   cmgr_s8bit      *p_buff     ,
    OUT     cmgr_ecode_t    *p_error    )
{
    fd_set                  conn_readset    ;
    cmgr_global_user_db_st  *p_usr_dblk = NULL      ;

        /* First check if the pointer params are NULL or not */
    if (NULL == p_error) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Ecode return buffer passed NULL\n") ) ;
        return CMGR_FAILURE ;

    } else if (CMGR_FAILURE == cmgr_get_user_dblk_for_mod_id (module_id, &p_usr_dblk) ) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] not found in DB \n", module_id) ) ;
        *p_error = CMGR_ECODE_USER_DBLK_ABSENT ;
        return CMGR_FAILURE ;
    }

        /* Now initialise the readfd set for select system call */
    /* Now start the infinite loop */
    for (;;) {
        struct sockaddr_in      serv_addr   ;
        cmgr_s16bit     flags = 0, selRet = -1, buff_len = 0 ;
        struct timeval  tmVal ;
        socklen_t       saddr_size = sizeof (serv_addr) ;

        *p_error = CMGR_ECODE_NO_ERR ;  /* Initialise error at every step */
        bzero ((char *)&serv_addr, sizeof(serv_addr)) ;
        bzero (p_buff, *p_buff_len) ;
        tmVal = p_usr_dblk->timeout_dura   ;
            /* FIll in the FD set */
        FD_SET (conn_id, &conn_readset) ;
        selRet = select (p_usr_dblk->max_sock_fd_id + 1, &conn_readset, NULL, NULL, &tmVal) ;
        if ((0 > selRet) && (EINTR == errno)) {
            continue ;
        } else if (0 > selRet) {
            perror ("[CMGR] Select error ") ;
            return CMGR_FAILURE ;
        } else if (0 == selRet) {
            /* Timeout has occured. Simply return back */
            *p_error = CMGR_ECODE_RECVFROM_TIMEOUT ;
            return CMGR_FAILURE ;
        } else if (!FD_ISSET (conn_id, &conn_readset)) {
            continue ;
        } else {
                /* Else we receive the message */
            buff_len = recvfrom (conn_id, p_buff, *p_buff_len, flags, 
                                            (struct sockaddr *)&serv_addr, &saddr_size) ;
            if (0 > buff_len) {
                perror (NULL) ;
                return CMGR_FAILURE ;
            } else {
                *p_buff_len = buff_len ;
                break ; // Break out of the for loop
            }
        }
    }

    return  CMGR_SUCCESS    ;
}

/*
 * @fn  cmgr_remove_user_fd
 *
 * @brief   This function removes the user FD's added into CMgr database.
 *
 * @param[in]   module_id       This is the applications module/thread-id
 * @param[in]   fd_id           FD-ID that needs to be removed
 * @param[out]  p_error         Return error code value
 *
 * @return      Returns CMGR_FAILURE if function fails otherwise returns CMGR_SUCCESS
 *
 */
cmgr_return_t
cmgr_remove_user_fd (
            IN  cmgr_mod_id_t               module_id       ,
            IN  cmgr_fd_id_t                fd_id           ,
            OUT cmgr_ecode_t                *p_error        )
{
    cmgr_return_t           ret_val = CMGR_SUCCESS ;
    cmgr_global_user_db_st  *p_usr_dblk = NULL      ;

    /* First let's find the user datablock */
    if (CMGR_FAILURE == cmgr_get_user_dblk_for_mod_id (module_id, &p_usr_dblk) ) {
        CMGR_TRACE (CMGR_TRACE_TYPE_INTERFACE, CMGR_TRACE_LEVEL_WARNING,
                ("[CMGR] Err! Module [%lu] not found in DB \n", module_id) ) ;
        *p_error = CMGR_ECODE_USER_DBLK_ABSENT ;
        ret_val = CMGR_FAILURE ;
    }
    /* First get a free connection datablock */
    p_usr_dblk->user_fd_info.fd_id = 0 ;

    return ret_val ;
}

