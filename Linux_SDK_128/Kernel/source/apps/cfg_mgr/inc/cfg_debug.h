/* 
*  @file      cfg_debug.h                
*
*  @brief     This file holds common dicleartion of debug 
*
*  @author    VVDN TECHNOLOGIES
*      
*  @license   Copyright (C) 2011 
*             This program is free software; you can redistribute it and/or modify it
*             under the terms of the GNU General Public License version 2 as
*             published by the Free Software Foundation.  
*/

#ifndef __CFG_DEBUG_H
#define __CFG_DEBUG_H

#define DEBUG

#define CFG_ALL      0xff
#define CFG_DEBUG    0x01
#define CFG_TRACE    0x02
#define CFG_INFO     0x04
#define CFG_ALM      0x08
#define CFG_WARN     0x10
#define CFG_ERROR    0x20

#ifdef DEBUG

#define DEBUG_MASK  CFG_ERROR

#define debug(level,str...)\
	do{\
	if(level & DEBUG_MASK)\
		printf(str);\
	} while(0);

#else
#define debug(x) 
#define DEBUG_MASK 0x00
#endif


#endif /*cfg_debug.h*/
/*
USAGE:
*
debug(TDM_DEBUG,"E1 port:%d\n",tdm_e1_no);
debug(TDM_TRACE, "E1 port is enabled");
debug(TDM_INFO, "E1 port %d is disabled\n",tdm_e1_no);
debug(TDM_ALM, "E1 link %d is down\n",tdm_e1_no);
etc....
*/
