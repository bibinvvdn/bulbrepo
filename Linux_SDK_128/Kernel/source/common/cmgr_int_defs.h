/*
 *  @file   cmgr_int_defs.h
 *
 *  @brief  This file contains the interfacing #defines and enums which will be used
 *          across all the users of connection manager library
 *
 *  @author 
 *
 */
#ifndef __CMGR_INT_DEFS_H__
#define __CMGR_INT_DEFS_H__

#ifdef  IN
#undef  IN
#endif

#ifdef  OUT
#undef  OUT
#endif

#ifdef  INOUT
#undef  INOUT
#endif

#define IN
#define OUT
#define INOUT

/*
 * @enum  cmgr_bool_t
 *
 * @brief   This enumeration is used for boolean purpose and has binary state, TRUE or FALSE
 */
typedef enum {
    CMGR_FALSE = 0,
    CMGR_TRUE,

} cmgr_bool_t   ;

/*
 * @enum  cmgr_return_t
 *
 * @brief   This enumeration is used for indicating return status. It can either be FAILURE or
 *          can be SUCCESS
 */
typedef enum {
    CMGR_FAILURE = 0	   ,
    CMGR_SUCCESS 	   ,

} cmgr_return_t ;

/*
 * @enum  cmgr_sock_type_et
 *
 * @brief   This enum indicates stream type, whether it's UDP, TCP or others (currently unused)
 */
typedef enum {
    CMGR_SOCK_TYPE_UDP_SVR_CLNT ,
    CMGR_SOCK_TYPE_TCP_CLIENT   ,
    CMGR_SOCK_TYPE_TCP_SERVER   ,
    CMGR_SOCK_TYPE_OTHERS       ,

} cmgr_sock_type_et   ;

/*
 * @enum  cmgr_fd_state_et
 *
 * @brief   This enum indicates current FD state. Useful for TCP connections
 */
typedef enum {
    CMGR_FD_STATE_IDLE = 0      ,
    CMGR_FD_STATE_CONNECTING    ,
    CMGR_FD_STATE_LISTENING     ,
    CMGR_FD_STATE_ACCEPT        ,
    CMGR_FD_STATE_READY         ,
    CMGR_FD_STATE_CLOSING       ,
    CMGR_FD_STATE_CLOSED        ,

} cmgr_fd_state_et  ;

#endif /* End of __CMGR_INT_DEFS_H__ */
