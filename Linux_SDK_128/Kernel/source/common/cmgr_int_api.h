/*
 *  @file   cmgr_int_api.h
 *
 *  @brief This file contains the interfacing functional interface for operating the sockets
 *      for communication purpose. User applications doesn't need to explicitly include
 *      cmgr_ecodes.h if this header file is included. Same applies to cmgr_int_types.h
 *
 *  @author 
 *
 */
#ifndef __CMGR_INT_API_H__
#define __CMGR_INT_API_H__

#include "cmgr_int_types.h"
#include "cmgr_ecodes.h"


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
    OUT cmgr_ecode_t                *p_error
) ;


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
    OUT cmgr_ecode_t        *p_error
) ;


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
    OUT cmgr_ecode_t        *p_error
) ;


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
    OUT cmgr_ecode_t                *p_error
) ;

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
            OUT cmgr_ecode_t                *p_error        
            );


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
    OUT cmgr_ecode_t    *p_error    ) ;

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
    OUT cmgr_ecode_t    *p_error    ) ;


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
    OUT cmgr_ecode_t        *p_error 
) ;


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
    OUT cmgr_ecode_t        *p_error
) ;


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
    OUT cmgr_ecode_t        *p_error    ) ;


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
    IN  cmgr_mod_id_t       self_mod_id ) ;

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
    OUT cmgr_ecode_t    *p_error    ) ;

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
    OUT     cmgr_ecode_t    *p_error    ) ;

#endif /* End of __CMGR_INT_API_H__ */
