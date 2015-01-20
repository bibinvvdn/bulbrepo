#include"web_cfg_api.h"
#include"web_api_opcode.h"
#include"wfps_api_common.h"


cmgr_sock_fd_t  g_connection_id;
#define WS_TRACE    printf
cmgr_return_t msg_frm_nw_mgt_set(
                cmgr_s8bit          *p_buf,
                cmgr_s16bit         buff_len,
                cmgr_ip_addr_st     rem_srcaddr ,
                cmgr_sock_fd_t      conn_id     ,
                cmgr_ecode_t        *p_ecode);

cmgr_return_t timeout_mesg_mgt_set(cmgr_ecode_t *p_ecode);

CMD_INTERFACE_T *payload_global;

api_return_e Web_Set_Mgt_Cfg_Api(MGMT_CFG_T *mgt_cfg, char *mgt_cfg_err)
{
        CMD_INTERFACE_T *p_cmd;
        const unsigned short buff_size = sizeof(CMD_INTERFACE_T) + sizeof(MGMT_CFG_T); //Calculating the buffer size which includes header and payload

        cmgr_ip_addr_st local_ip_addr, remote_ip_port;

        cmgr_ecode_t cmgr_error;

        p_cmd = malloc(buff_size);
        // If malloc fails then code enters following conditions.
        if (!p_cmd) {
                WS_TRACE("[WEB_MGT_SET] Malloc fails");
                exit (1) ;
        }

        p_cmd->module_id = MOD_ID_WEB_SRV; // Setting module id in header from web_api_common.h
        p_cmd->cmd_opcode = WEB_CFGM_MGT_CFG_SET; // Setting opcode from web_api_opcode.h
        p_cmd->cmd_seq_number = 1; // Setting sequence number which will be a random number later.
        p_cmd->cmd_exec_status = API_CMD_PROCESSED; // To be set as processed or failed.

        p_cmd->buff_length = buff_size;

        memcpy(p_cmd->buff,mgt_cfg,sizeof(MGMT_CFG_T));
        
        if(CMGR_SUCCESS != cmgr_init_conn_lib(MOD_ID_WEB_SRV, msg_frm_nw_mgt_set,
                                timeout_mesg_mgt_set, (cmgr_s8bit*)"Web Server",
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
	        return SUCCESS;
        }
        else{
                strcpy(mgt_cfg_err,payload_global->buff);
	        return FAILURE;
        }
}

cmgr_return_t msg_frm_nw_mgt_set(
                cmgr_s8bit          *p_buf,
                cmgr_s16bit         buff_len,
                cmgr_ip_addr_st     rem_srcaddr ,
                cmgr_sock_fd_t      conn_id     ,
                cmgr_ecode_t        *p_ecode)
{
        payload_global = (CMD_INTERFACE_T*)p_buf;

        if (CFGM_WEB_MGT_CFG_SET != payload_global->cmd_opcode)
        {
                WS_TRACE("[WEB_MGT_SET] Err. Received Invalid message\n") ;
        }
	
        *p_ecode = CMGR_ECODE_MODULE_SHUTDOWN_INIT ;
        return CMGR_FAILURE;
}


cmgr_return_t timeout_mesg_mgt_set(cmgr_ecode_t *p_ecode)
{
        WS_TRACE("Message timeout, no message received from the network\n");

        exit(0);
        return 1;
}



/*DEMO Code*/
#if 1
main()
{
        MGMT_CFG_T info_mgt;
        char err[100];
        fprintf(stderr,"Before Calling API\n");
      //  info_mgt.save   	= 0;
      //  info_mgt.reboot 	= 1;
      // info_mgt.factory_def    = 0;

	char *qstr=getenv("QUERY_STRING");
        int qlen=strlen(qstr);

        if(qlen==0) {
                fprintf(stderr, "No Method specified");
                return 0;
        }
        fprintf(stderr, "Query String = %s\n", qstr);
        printf("Content-Type:text/html\n\n");
//Query String =

	int i=0,j=0,k=0,l=0;
        char request[200],variable[100],value[100];

        for(i=0;i<qlen-1;i++) {
                j=0;
                while(qstr[i]!='&' && qstr[i]!='\0') {
                        request[j++]=qstr[i++];
                }
                request[j]='\0';
                k=0;
                while(request[k]!='=' && request[k]!='\0') {
                        variable[k]=request[k++];
                }
                variable[k++]='\0';
                l=0;
                while(request[k]!='\0') {
                        value[l++]=request[k++];
                }
                value[l]='\0';

                fprintf(stderr, "%s=%s\n",variable, value);	
		
//parsing 
		if(! strncmp(variable,"save",4)){
                        info_mgt.save = atoi(value);
                } else if(! strncmp(variable,"reboot",6)){
                        info_mgt.reboot = atoi(value);
                } else if(! strncmp(variable,"factory_def",11)){
                        info_mgt.factory_def = atoi(value);	
	
	}
}
		fprintf(stderr,"%d,%d,%d\n",info_mgt.save,info_mgt.reboot,info_mgt.factory_def);
// API call
        if(SUCCESS == Web_Set_Mgt_Cfg_Api(&info_mgt, err)){
                fprintf(stderr,"After Called API\n");
                fprintf(stderr,"Got structure from cfg:::\n");
        }else{
                fprintf(stderr,"After Called API\n");
                fprintf(stderr,"Err msg   : %s\n", err);
        }
}
#endif