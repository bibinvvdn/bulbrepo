/*
 *  @file   cmgr_defs.h
 *
 *  @brief  This file contains the enumerations and #defines used within Conn Mgr library
 *
 *  @author 
 *
 */
#ifndef __CMGR_DEFS_H__
#define __CMGR_DEFS_H__

#include "cmgr_int_defs.h"

#define CMGR_MAX_NUM_USERS          10
#define CMGR_MAX_NUM_FD_PER_USER    5
#define CMGR_MAX_USER_STR_LEN       20
#define CMGR_MAX_SOCK_BUFF_LEN      30000

#ifdef  USE_TRACE_PRINTF
#define CMGR_TRACE(tr_type, tr_level, tr_str)  \
    do {    \
        printf tr_str ; \
    } while (0)
#else
#define CMGR_TRACE(tr_type, tr_level, tr_str)
#endif


/* Memory Functions */
#ifdef  TU_MEM_CHECK
#define __MALLOC__  tu_malloc
#define __FREE__    tu_free

#else   /* If TU_MEM_CHECK is not defined then this */
#define __MALLOC__  malloc
#define __FREE__    free
#endif  /* End of TU_MEM_CHECK */

#define __CALLOC__              calloc
#define         __REALLOC__     realloc



/*
 * @enum  cmgr_trace_type_et
 *
 * @brief   This enumeration is used for describing the trace type for traces
 */
typedef enum cmgr_trace_type_enum {
    CMGR_TRACE_TYPE_CONFIG = 0  ,
    CMGR_TRACE_TYPE_IPC         ,
    CMGR_TRACE_TYPE_INTERFACE   ,
    CMGR_TRACE_TYPE_SYSTEM      ,

} cmgr_trace_type_et ;

/*
 * @enum  cmgr_trace_level_et
 *
 * @brief   This enumeration is used for selecting the trace level based upon it's severity
 */
typedef enum cmgr_trace_level_enum {
    CMGR_TRACE_LEVEL_CRITICAL = 0   ,
    CMGR_TRACE_LEVEL_WARNING        ,
    CMGR_TRACE_LEVEL_BRIEF_INFO     ,
    CMGR_TRACE_LEVEL_DETAILED_INFO  ,
    CMGR_TRACE_LEVEL_DEBUG          ,

} cmgr_trace_level_et   ;

#endif /* End of __CMGR_DEFS_H__ */

