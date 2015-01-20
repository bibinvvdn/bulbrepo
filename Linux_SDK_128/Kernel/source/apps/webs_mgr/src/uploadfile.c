#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "web_cfg_api.h"
#include "web_api_opcode.h"
#include "wfps_api_common.h"

#include "types.h"
#include "funcs_cgi.h"
#include "cli_get_set_funcs.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "address.h"
//#include "wfps_api_ta.h"
#include "wfps_api_common.h"
//#include "tdmoip_api_cm_sm_am.h"
#include "cmgr_int_api.h"

#define WS_TRACE(x)    fprintf(stderr,x)
#define FILE_UPLOAD_BOUNDARY_LINE_LENGTH 29
cmgr_sock_fd_t  g_connection_id;

char lineptr[100];
cmgr_return_t msg_frm_nw_sys_get(
		cmgr_s8bit          *p_buf,
		cmgr_s16bit         buff_len,
		cmgr_ip_addr_st     rem_srcaddr ,
		cmgr_sock_fd_t      conn_id     ,
		cmgr_ecode_t        *p_ecode);

cmgr_return_t timeout_mesg_sys_get(cmgr_ecode_t *p_ecode);
CMD_INTERFACE_T *payload_global;

int set_firmware_settings()
{
	char	*fdata;		// CGI POST Request
	char	saved_uploaded_data[] = "/dev/shm/uploaded_data";	// Uploaded file/files saved to a single file
	char	ldfile[] = UPLOADED_FILE_NAME;	// Saved firmware File
        FILE *fd;
    	char ch;
	int	ldfile_status	= -1;
	//	int	status = -1;
	//	int	cmd_status = 0;


	/******************************************
	 *	Get CGI POST Request
	 ******************************************/
	fdata = get_post_request();
	if(fdata == (char*) NULL)
	{
		free(fdata);
		exit(1);
	}

	write_to_file(fdata,atoi(getenv("CONTENT_LENGTH")), saved_uploaded_data);
        fd = fopen(saved_uploaded_data,"r");

	int var=0;

        while((ch=fgetc(fd)) != '\n'){
        lineptr[var]=ch;
        var++;
        }
        lineptr[var]='\0';

        //getline(&lineptr,0,fd);
        printf("boundary is %s\n",lineptr);
        fclose(fd);
	ldfile_status  = decode_file_upload(saved_uploaded_data, "fufile", ldfile);
	
//	char str[100];
//	strcpy(str,"sh /system_upgrade.sh");
//	system("cd /var/www/cgi-bin");
//	strcpy(str,"./image_upgrade.cgi");
//	system(str);	
	CMD_INTERFACE_T *p_cmd; // structure for the complete message including the header.
	const unsigned short buff_size = sizeof(CMD_INTERFACE_T);
	cmgr_ip_addr_st local_ip_addr, remote_ip_port;
	cmgr_ecode_t cmgr_error;
	p_cmd = malloc(buff_size); // allocating the total memory

	if (!p_cmd) {
		printf("[CFG_OMCIP_LINK]Malloc fails");
		exit (1) ;
	}
	p_cmd->module_id = MOD_ID_WEB_SRV; // Setting module id in header from tdmoip_api_common.h
	p_cmd->cmd_opcode = WEB_CFGM_IMAGE_UPGRADE; // Setting opcode from tdmoip_api_cm_sm_am.h
	p_cmd->cmd_seq_number = 1; // Setting sequence number which will be a random number later.
	p_cmd->cmd_exec_status = API_CMD_PROCESSED; // To be set as processed or failed.
	p_cmd->buff_length = buff_size;

	// Connection manager code begins here

	if(CMGR_SUCCESS != cmgr_init_conn_lib(MOD_ID_WEB_SRV, msg_frm_nw_sys_get,
				timeout_mesg_sys_get, (cmgr_s8bit*)"Web Server",&cmgr_error))
	{
		WS_TRACE("IPC Connection initialisation failed\n");
		return 0;
	}

	local_ip_addr.ip_addr = INADDR_ANY;
	local_ip_addr.port    = WEBM_PORT_NUM;

	if(CMGR_SUCCESS != cmgr_open_conn(MOD_ID_WEB_SRV, &local_ip_addr,
				CMGR_SOCK_TYPE_UDP_SVR_CLNT, &g_connection_id, &cmgr_error))
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

	//            cmgr_proc_conn_msg(MOD_ID_WEB_SERVER, &cmgr_error);

	return 1;

} 


int main()
{
	set_firmware_settings();
	printf("Content-Type: text/html\r\n\r\n");
	printf("<html><head><script>\n");
//	printf("function loadhtml(){ window.location='../upgrade_new.html';}\n");
	printf("function loadhtml(){ window.location='../okpage.html';}\n");
	printf("</script></head>\n");
	printf("<body onLoad=\"setInterval('loadhtml()', 400000 )\">\n");
//	printf("<br><br><Font size='2'><b><center>File Transfer Complete. Please wait while system Saves the file..<br>You can activate the new image after Imag Upgarade sucess alarm</b></center></font>\n");
	printf("<br><br><Font size='2'><b><center>File Transfer Complete. Please Click on Activate for Firmware upgradation..<br>and please wait for some time</b></center></font><br>\n");
	printf("<form action=\"../cgi-bin/image_upgrade.cgi\" name=\"main_form\"  method=\"POST\">");	
	printf("<center><input name=activate type=submit value=\"Activate\"></center>");
	printf("</form>");
	printf("</body></html>\n");
	printf("\r\n\r\n");
	return 0;
}


cmgr_return_t msg_frm_nw_sys_get(
		cmgr_s8bit          *p_buf,
		cmgr_s16bit         buff_len,
		cmgr_ip_addr_st     rem_srcaddr ,
		cmgr_sock_fd_t      conn_id     ,
		cmgr_ecode_t        *p_ecode)
{
	payload_global =   (CMD_INTERFACE_T *)p_buf;
	if (CFGM_WEB_SYS_CFG_GET != payload_global->cmd_opcode)
	{
		WS_TRACE("[WEB_SYS_GET] Err. Received Invalid message\n") ;
	}

	*p_ecode = CMGR_ECODE_MODULE_SHUTDOWN_INIT ;
	return CMGR_FAILURE;

}

cmgr_return_t timeout_mesg_sys_get(cmgr_ecode_t *p_ecode)
{
	WS_TRACE("Message timeout, no message received from the network\n");

	exit(0);
	return 1;
}

int decode_file_upload(char *uploaded_files, char *file_name, char *file_to_save)
{
	char  *line;
	FILE  *fh1;
	FILE  *fh2;
	uint  len = 0;
	uint  boundary_line_found	= 0;
	uint  file_name_found		= 0;	
	uint  content_type_found	= 0;	
	uint  start_of_file		= 0;	
	uint  pos1 =0;
	uint  pos2 =0;
	uint  i =0;
	char  ch;

	len =  atoi(getenv("CONTENT_LENGTH"));

	line = malloc((len) * sizeof(char));

	if(line == (char*) NULL)
	{
		fprintf(stderr, "Func : decode_file_upload : param \"line\" : Memory Allocation Failure\n");
		return -1;
	}

	fh1 = fopen(uploaded_files, "rb");
	if(fh1 ==  NULL)
	{
		fprintf(stderr, "Func : decode_file_upload : Unable to open file \"%s\" for read operation\n", uploaded_files);
		return -1;
	}
	fh2 = fopen(file_to_save, "wb");
	printf("image file is opened 206 \n");
	if(fh2 ==  NULL)
	{
		fprintf(stderr, "Func : decode_file_upload : Unable to open file \"%s\" for write operation\n", file_to_save);
		return -1;
	}

	while(fgets(line, len, fh1) != ((char*) NULL))
	{
		/*	Check for boundary line "-----------------------------XXXXXXXXXXX"   */
		if((start_of_file == 0) && (strncmp(line,lineptr,10) == 0))
		{
			boundary_line_found  = 1;
			continue;
		}
		if((boundary_line_found ==1) && (strncmp(line,"Content-Disposition: form-data;", 31) == 0))
		{
		printf("checking disposition\n");
			if(strstr(line,  file_name) !=  NULL)      /* If file searched is uploaded */
			{
				//	print_html_msg("file_name_found");
				file_name_found = 1;
				boundary_line_found = 0;
				continue;
			}
			else	/* If file searched is not uploaded, then continue */
			{
				file_name_found = 0;
				boundary_line_found = 0;
				continue;
			}
		}
		/*

		   else	// Else If First Boundary Line not found, Invalid File Upload 

		   {

		   return -1;

		   }

		 */

		if((file_name_found == 1) && (strstr(line, "Content-Type:")))
		{
			printf("checking content type\n");
			file_name_found = 0;
			//	print_html_msg("content_type_found");
			content_type_found = 1;
			continue;
		}
		if(content_type_found == 1)
		{
			start_of_file = 1;
			content_type_found = 0;
			fseek(fh1, 0, SEEK_CUR);
			pos1 = ftell(fh1);
			continue;
		}
		if((start_of_file == 1)  && 
				((strncmp(line,lineptr,10) != 0)))  	
		{
			//	print_html_msg("writing_to_file");  
			fseek(fh1, 0, SEEK_CUR);
			pos2 = ftell(fh1);
			continue;
		}
		else if((start_of_file == 1) && 
				(strncmp(line,lineptr,10) == 0))
		{
			//	print_html_msg("next_boundary_hit");
			start_of_file = 0;
			break;
		}
		else
		{
			continue;
		}
	}
	fclose(fh1);

	fh1 = fopen(uploaded_files, "rb");
	if(fh1 == NULL)
	{
		fprintf(stderr, "Func : decode_file_upload : Unable to open file \"%s\" for read operation\n", uploaded_files);
		return -1;
	}

	while(1)
	{
		ch = fgetc(fh1);
		if(i == pos2)
		{
			break;
		}
		//changed "pos2" to "pos2 - 2" as 2 extra bytes were written
		if((i >= pos1) && (i < (pos2 - 2)))
		{
			fputc(ch, fh2);
		}
		i++;
	}
	fclose(fh2);
	fclose(fh1);
	free(line);
	return 0;
}

char* get_post_request(void)

{
	char* data;
	int len  = 0;
	if(getenv("CONTENT_LENGTH") != NULL)
	{
		len  = atoi(getenv("CONTENT_LENGTH"));
		if(len == 0)
		{
			fprintf(stderr, "Func : get_post_request : CGI ENV VAR \"CONTENT_LENGTH\" value is  Zero\n");
			return (char*) NULL;
		}
		data = malloc(len+1 * sizeof(char));
		if(data == NULL)
		{
			fprintf(stderr, "Func : get_post_request : Memory Allocation Failure for  param \"data\"\n");
			return (char*) NULL;
		}
		else
		{
			//	fgets(data, len+1, stdin);
			fread(data, 1, len, stdin);	// changed to take care of file upload
			return data;
		}
	}
	else
	{
		fprintf(stderr, "Func : get_post_request : CGI ENV VAR \"CONTENT_LENGTH\" is  Undefined\n");
		return (char*) NULL;
	}
}

int write_to_file(char *mem_block, int size, char *file_path)
{
	FILE *fh;

	fh = fopen(file_path, "w");
	if(fh !=  NULL)
	{
		fwrite(mem_block, 1, size, fh);
		fclose(fh);
		return 0;
	}
	else
	{
		fprintf(stderr, "Func : write_to_file : Unable to open file \"%s\" for write operation\n", file_path);
		return -1;
	}
}
