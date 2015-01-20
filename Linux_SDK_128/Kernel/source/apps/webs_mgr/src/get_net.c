#include"web_cfg_api.h"
#include"web_api_opcode.h"
#include"wfps_api_common.h"


cmgr_sock_fd_t  g_connection_id;
#define WS_TRACE    printf
cmgr_return_t msg_frm_nw_net_get(
                cmgr_s8bit          *p_buf,
                cmgr_s16bit         buff_len,
                cmgr_ip_addr_st     rem_srcaddr ,
                cmgr_sock_fd_t      conn_id     ,
                cmgr_ecode_t        *p_ecode);

cmgr_return_t timeout_mesg_net_get(cmgr_ecode_t *p_ecode);

CMD_INTERFACE_T *payload_global;

api_return_e Web_Get_Nw_Cfg_Api(NW_CFG_T *nw_cfg, char *nw_cfg_err)
{
        CMD_INTERFACE_T *p_cmd;
        const unsigned short buff_size = sizeof(CMD_INTERFACE_T); //Calculating the buffer size which includes header and payload

        cmgr_ip_addr_st local_ip_addr, remote_ip_port;

        cmgr_ecode_t cmgr_error;

        p_cmd = malloc(buff_size);
        // If malloc fails then code enters following conditions.
        if (!p_cmd) {
                WS_TRACE("[WEB_NW_GET]Malloc fails");
                exit (1) ;
        }

        p_cmd->module_id = MOD_ID_WEB_SRV; // Setting module id in header from web_api_common.h
        p_cmd->cmd_opcode = WEB_CFGM_NW_CFG_GET; // Setting opcode from web_api_opcode.h
        p_cmd->cmd_seq_number = 1; // Setting sequence number which will be a random number later.
        p_cmd->cmd_exec_status = API_CMD_PROCESSED; // To be set as processed or failed.

        p_cmd->buff_length = buff_size;

        if(CMGR_SUCCESS != cmgr_init_conn_lib(MOD_ID_WEB_SRV, msg_frm_nw_net_get,
                                timeout_mesg_net_get, (cmgr_s8bit*)"Web Server",
                                &cmgr_error))
        {
                WS_TRACE("IPC Connection initialisation failed\n");
                free(p_cmd);
                return 0;
        }

        local_ip_addr.ip_addr = INADDR_ANY;
        local_ip_addr.port    = WEBM_PORT_NUM; 
        //to allow more than one person to access the element
        //manager simultaneously

        if(CMGR_SUCCESS != cmgr_open_conn(MOD_ID_WEB_SRV, &local_ip_addr,
                                CMGR_SOCK_TYPE_UDP_SVR_CLNT, &g_connection_id, &cmgr_error))
                // fill structure local ip address - common header file - keep conn id as global variable
        {
                WS_TRACE("Connection Manager Open Connection Failed\n");
                free(p_cmd);
                return 0;
        }

        remote_ip_port.ip_addr = INADDR_ANY;
        remote_ip_port.port    = CFGM_PORT_NUM;

        if(CMGR_SUCCESS != cmgr_add_rem_conn_params (MOD_ID_WEB_SRV, MOD_ID_CFG_MGR,
                                &remote_ip_port, CMGR_SOCK_TYPE_UDP_SVR_CLNT, &cmgr_error))
        {
                WS_TRACE("Connection Manager Open Connection Failed\n");
                free(p_cmd);
                return 0;
        }

        if(CMGR_SUCCESS != cmgr_send_buff_to_module(MOD_ID_WEB_SRV, g_connection_id,
                                (cmgr_s8bit*)p_cmd, buff_size, MOD_ID_CFG_MGR, &cmgr_error))
        {
                WS_TRACE("Connection Manager Send Message Failed\n");
                free(p_cmd);
                return 0;
        }
        cmgr_proc_conn_msg(MOD_ID_WEB_SRV, &cmgr_error); /*Blocking function*/

        if(payload_global->cmd_exec_status == API_CMD_PROCESSED){
                memcpy(nw_cfg,payload_global->buff,sizeof(NW_CFG_T));
	        return SUCCESS;
        }
        else{
                strcpy(nw_cfg_err,payload_global->buff);
                printf("Err Msg: %s\n",nw_cfg_err);
	        return FAILURE;
        }
}

cmgr_return_t msg_frm_nw_net_get(
                cmgr_s8bit          *p_buf,
                cmgr_s16bit         buff_len,
                cmgr_ip_addr_st     rem_srcaddr ,
                cmgr_sock_fd_t      conn_id     ,
                cmgr_ecode_t        *p_ecode)
{

        payload_global = (CMD_INTERFACE_T*)p_buf;

        if (CFGM_WEB_NW_CFG_GET != payload_global->cmd_opcode)
        {
                WS_TRACE("[WEB_NW_GET] Err. Received Invalid message\n") ;
        }
	
        *p_ecode = CMGR_ECODE_MODULE_SHUTDOWN_INIT ;
        return CMGR_FAILURE;
}


cmgr_return_t timeout_mesg_net_get(cmgr_ecode_t *p_ecode)
{
        WS_TRACE("Message timeout, no message received from the network\n");

        exit(0);
        return 1;
}

/*DEMO Code*/
#if 1
main()
{
        NW_CFG_T info_nw;
        char err[100];
	printf("Content-Type:text/html\n\n");
      /*  printf("Before Calling API\n"); */
        if(SUCCESS == Web_Get_Nw_Cfg_Api(&info_nw, err)){
		printf("net=%s %s %s %s %s",info_nw.ip_addr,info_nw.netmask,info_nw.gateway,info_nw.dns_pri,info_nw.dns_sec);
           /*     printf("After Called API\n");  
                printf("Got structure from cfg:::\n");
                printf("dhcp  		: %d\n", info_nw.dhcp);
                printf("IP Address	: %s\n", info_nw.ip_addr);
                printf("netmask  	: %s\n", info_nw.netmask);
                printf("Gateway  	: %s\n", info_nw.gateway);
                printf("dns_pri 	: %s\n", info_nw.dns_pri);
                printf("dns_sec 	: %s\n", info_nw.dns_sec);
        }else{
                printf("After Called API\n");  
                printf("Err msg   : %s\n", err); 
	  */
        }
	fflush(stdout);
}
#endif

