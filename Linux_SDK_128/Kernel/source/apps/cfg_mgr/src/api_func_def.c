#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <errno.h>
#include <linux/unistd.h>       /* for _syscallX macros/related stuff */
#include <linux/kernel.h>      /* for struct sysinfo */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
//#include <net/if.h>
#include <sys/types.h>
#include <math.h>
#include <fcntl.h>
#include <ctype.h>
#include <linux/wireless.h>
#include <netinet/in.h>        
#include "common_header.h"
#include "cfg_api.h"
#include "network.h"

#define SA              struct sockaddr
#define MAXLINE         4096
#define MAXSUB          200
#define LISTENQ         1024


typedef struct
{
	uint8_t channel;
	int8_t  rssi;
	int16_t	timestamp[4];
	uint8_t src[6];
}probe;

#if 0

struct probe
{
	unsigned int mac[6];
	int timestamp[3];
	int ssi;
	int channel;
};

#endif

extern cmgr_sock_fd_t      g_connection_id;

extern WFPS_DATABASE_T wfps_database;
#define NI_MAXHOST 1025
#define NI_NUMERICHOST 1


int iw_getstats(char *ifname,struct iw_statistics *rssi_str)
{
  FILE *f=fopen("/proc/net/wireless","r");
  char buf[256];
  char *bp;
  if(f==NULL)
        return -1;
  while(fgets(buf,255,f))
  {
        bp=buf;
        while(*bp&&isspace(*bp))
                bp++;
        if(strncmp(bp,ifname,strlen(ifname))==0 && bp[strlen(ifname)]==':')
        {
                bp=strchr(bp,':');
                bp++;
                bp = strtok(bp, " .");
                sscanf(bp, "%X", (unsigned int *)&rssi_str->status);
                bp = strtok(NULL, " .");
                sscanf(bp, "%d", (unsigned int *)&rssi_str->qual.qual);
                bp = strtok(NULL, " .");
                sscanf(bp, "%d", (unsigned int *)&rssi_str->qual.level);
                bp = strtok(NULL, " .");
                sscanf(bp, "%d", (unsigned int *)&rssi_str->qual.noise);
                bp = strtok(NULL, " .");
                sscanf(bp, "%d", &rssi_str->discard.nwid);
                bp = strtok(NULL, " .");
                sscanf(bp, "%d", &rssi_str->discard.code);
                bp = strtok(NULL, " .");
                sscanf(bp, "%d", &rssi_str->discard.misc);
                fclose(f);
                return 0;
        }
  }
  fclose(f);
  return 0;
}



/* System Confifuration */
void Set_Sys_Cfg_page(SYS_CFG_T *sys_msg)
{
        SYS_CFG_T *info_sys;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
	char err_str[100];
        unsigned int buff_size;
	struct tm mytime;
    	struct timeval systime;
	char status = 0;
        
	buff_size = sizeof(CMD_INTERFACE_T);
        info_sys = (SYS_CFG_T*)malloc(sizeof(SYS_CFG_T));
/*  malloc check */
	if((NULL == info_sys) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }
//	printf("inside the app functon definition \n");
	memcpy(info_sys,sys_msg,sizeof(SYS_CFG_T));
/*Add some value to check in web server*/

	if(0 > sethostname(info_sys->hostname,(sizeof(info_sys->hostname)))){
		debug(CFG_ERROR,"Setting hostname failure\n\n");
		strcpy(err_str,"Setting hostname failure");
		status = 1;
                goto ret;
	}
	memcpy(&wfps_database.sys_cfg_t.hostname,&info_sys->hostname,32);

	mytime.tm_sec = info_sys->sec;    // Seconds
        mytime.tm_min = info_sys->min;    // Minutes
        mytime.tm_hour = info_sys->hour;   // Hours
        mytime.tm_mday = info_sys->day;      // Day of Month
        mytime.tm_mon = ((info_sys->month ) - 1);     // Month
        mytime.tm_year = ((info_sys->year) - 1900); // Year

        systime.tv_sec = mktime(&mytime);
        systime.tv_usec = 0;

        if(0 > settimeofday(&systime,NULL)){
	        debug(CFG_ERROR,"Setting systemtime  failure\n\n");
		strcpy(err_str,"Setting system time failed");
		status = 1;
                goto ret;
        }
//	wfps_db_write_cfg_2_file(); // write to flash
	
ret:
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}
	
	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
	}

	p_tx_api_hdr->buff_length      = buff_size;
        p_tx_api_hdr->cmd_opcode       = CFGM_WEB_SYS_CFG_SET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;

        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
                free(info_sys);                 
                return ;

        }

        //Free the API buffer
        free(p_tx_api_hdr);

free_ret:
        free(info_sys);
//	printf("end of the app functon definition \n");
        return ;
}/* end of Set_Sys_Cfg */

void Get_Sys_Cfg_page()
{
        SYS_CFG_T *info_sys;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        unsigned int buff_size;
	unsigned short year;
	unsigned short hour;
        unsigned char min;
        unsigned char days;
	unsigned char month;


	time_t ltime;
        ltime=time(NULL);
        struct tm *tm;
 	struct sysinfo s_info;
        int error_s_info;
        char uptime[16];
	char err_str[100];
	char status = 0;

        tm=localtime(&ltime);

	year = ((tm->tm_year)+1900);
        month = ((tm->tm_mon)+1);


        buff_size = sizeof(CMD_INTERFACE_T) + sizeof(SYS_CFG_T);
        info_sys = (SYS_CFG_T*)malloc(sizeof(SYS_CFG_T));
/* check the malloc operaton */
	if((NULL == info_sys) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }
//	printf("inside the app functon definition \n");
	memcpy(info_sys,&wfps_database.sys_cfg_t,sizeof(SYS_CFG_T));

/* update the time filed */	
	memcpy(&(info_sys->year),&year,sizeof(unsigned short));
        memcpy(&(info_sys->month),&month,sizeof(unsigned char));
        memcpy(&(info_sys->day),&(tm->tm_mday),sizeof(unsigned char));
        memcpy((&info_sys->hour),&(tm->tm_hour),sizeof(unsigned char));
        memcpy((&info_sys->min),&(tm->tm_min),sizeof(unsigned char));
        memcpy((&info_sys->sec),&(tm->tm_sec),sizeof(unsigned char));
/* uptime Calculation */
    	error_s_info = sysinfo(&s_info);
    	if(error_s_info != 0)
    	{
        	debug(CFG_ERROR,"uptime failure\n\n");
		strcpy(err_str,"uptime failure");
		status = 1;
		goto ret;
	}

    	min = (s_info.uptime/60%60);
    	hour = (s_info.uptime/60/60%24);
    	days = (s_info.uptime/60/60/24);

    	sprintf(uptime,"%03ddays,%02d:%02d",days,hour,min);
	memcpy(&(info_sys->uptime),&(uptime),(sizeof(char) * 16));

ret:	
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}

	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
        	memcpy(p_tx_api_hdr->buff,info_sys,sizeof(SYS_CFG_T));
	}

/*TESTED for err case*/
         //p_tx_api_hdr->cmd_exec_status  = API_CMD_FAILED;
         //strcpy(p_tx_api_hdr->buff,"Error message from cfg");
	p_tx_api_hdr->buff_length      = buff_size;
	p_tx_api_hdr->cmd_opcode       = CFGM_WEB_SYS_CFG_GET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;
        //Send message on the network
        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
		goto free_ret;

        }

        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_sys);
//	printf("end of the app functon definition \n");
        return ;
}  /* end of the Get_Sys_Cfg */


int val_set_ip(char *ip_arr) {
	char pri_if_name[16];
        in_addr_t    ip_address;
        unsigned char *token, token1[20] = {'\0'};
        int count=0, indx=0,ip[4], ip_mask_gw_add[4]= {0};

	get_managment_if_name(pri_if_name);
        memcpy(token1,ip_arr,strlen(ip_arr));
        token = strtok(token1,".");
        while(token != NULL)
        {
                ip[indx++] = atoi(token);
                token = strtok(NULL,".");
        }
        if(indx != 4){
                debug(CFG_ERROR, "Invalid IP Address.\n");
                return -1;
        }
        ip_address = inet_addr((const char*)(ip_arr));
        if((0 == ip_address) || (-1 == ip_address))
        {
                debug(CFG_ERROR, "Invalid IP Address.\n");
                return -1;
        }
        if( 0 > set_ipaddress(pri_if_name, ip_address))
        {
            debug(CFG_DEBUG, "Unable to configure the IP Address.\n");
            return -1;
        }

        
	strcpy(wfps_database.nw_cfg_t.ip_addr,ip_arr);

        return 0;
}


int val_set_gateway(char *gate_way) {
	char pri_if_name[16], str_route[100];
        in_addr_t ip_gateway;
        unsigned char *token, token1[20]= {'\0'};
        int count =0, indx=0,ip[4],ip_mask_gw_add[6]= {0};

        memcpy(token1,gate_way,strlen(gate_way));
        token = strtok(token1,".");
        while(token != NULL)
        {
                ip[indx++] = atoi(token);
                token = strtok(NULL,".");
        }
        if(indx != 4){
                debug(CFG_ERROR, "Invalid IP Address.\n");
                return -1;
        }

        get_managment_if_name(pri_if_name);
        ip_gateway = inet_addr((const char*)(gate_way));
        if((0 == ip_gateway) || (-1 == ip_gateway))
        {
                debug(CFG_ERROR, "Invalid GATEWAY Address.\n");
                return -1;
        }
        sprintf(str_route, "add default gw %s",gate_way);

        if( 0 > configure_route(str_route))
        {
                debug(CFG_DEBUG, "Unable to configure the gateway.\n");
                return -1;
        }

	strcpy(wfps_database.nw_cfg_t.gateway,gate_way);

        return 0;
}


int val_set_netmask(char *net_mask) {

	char pri_if_name[16];
        in_addr_t    ip_subnet_mask;
        unsigned char *token;
        int get_octet,check_mask,count =0, ip_mask_gw_add[4]={0};
        get_managment_if_name(pri_if_name);
	ip_subnet_mask = inet_addr((const char*)(net_mask));
        if((0 == ip_subnet_mask) || (-1 == ip_subnet_mask))
        {
                debug(CFG_ERROR, "Invalid IP Address.\n");
                return -1;
        }

        if( 0 > set_netmask(pri_if_name, ip_subnet_mask))
        {
            debug(CFG_DEBUG, "Unable to configure the IP Address.\n");
            return -1;
        }

        strcpy(wfps_database.nw_cfg_t.netmask,net_mask);

        return 0;
}


int val_set_dns(char *dns1,char *dns2) {
	char *str;
        str = (malloc(sizeof(char)*100));
        if (str == NULL) {
            debug(CFG_ERROR,"dns malloc error\r\n");
            return -1;
        }
        sprintf(str,"echo nameserver %s > /etc/resolv.conf",dns1);
        system (str);
        sprintf(str,"echo nameserver %s >> /etc/resolv.conf",dns2);
        system (str);
        free(str);

	strcpy(wfps_database.nw_cfg_t.dns_pri,dns1);
	strcpy(wfps_database.nw_cfg_t.dns_sec,dns2);
        
        return 0;
}
	
int dhcp_ip_get()
{
        struct ifaddrs *ifaddr, *ifa;
        int family, s, n;
        char host[NI_MAXHOST],*token,netmask[NI_MAXHOST];

        if (getifaddrs(&ifaddr) == -1) {
        	debug(CFG_ERROR,"getifaddrs error\n\n");
         	return -1;
	}

        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
             family = ifa->ifa_addr->sa_family;

                if (family == AF_INET) {
                        s = getnameinfo(ifa->ifa_addr,
                                        (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                        sizeof(struct sockaddr_in6),
                                        host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                        if (s != 0) {
	                        debug(CFG_ERROR,"getnameinfo ip_addr error\n\n");
        		        return -1;
                         }

                        n = getnameinfo(ifa->ifa_netmask, sizeof(struct sockaddr_in),
                                                       netmask, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                        if (n != 0) {
                                debug(CFG_ERROR,"getnameinfo netmask error\n\n");
                                return -1;
                        }

                        if(strncmp(ifa->ifa_name ,"wlan0",5) == 0) {
				memcpy(&(wfps_database.nw_cfg_t.ip_addr),&host,sizeof(host));
				memcpy(&(wfps_database.nw_cfg_t.netmask),&netmask,sizeof(netmask));
                        }
		}
        }
        return 0;
}


int get_dns_address() {

	FILE *file = fopen ("/etc/resolv.conf", "r" );
        char *token,dns_pri[16],dns_sec[16];
        int count = 0;
        if ( file != NULL ) {
        	char line [ 128 ]; /* or other suitable maximum line size */
                while ( fgets ( line, sizeof line, file ) != NULL ) {
                	if((strstr(line,"nameserver")) != NULL) {
                        	token = strtok(line," ");
                                if (token != NULL) {
                                   	token = strtok(NULL,"\n");
                                        if(count == 0) {
                                	        memcpy(dns_pri,token,16);
                                        } else {
                                                memcpy(dns_sec,token,16);
                                        }
                                        count++;
                                }
                         }
                }
                fclose ( file );
	} else {
                debug(CFG_ERROR,"file open failure\n\n");
                return -1;      
	}
	memcpy(&(wfps_database.nw_cfg_t.dns_pri),&dns_pri,sizeof(dns_pri));
        memcpy(&(wfps_database.nw_cfg_t.dns_sec),&dns_sec,sizeof(dns_sec));

	return 0;
}


int server_alive()
{
	char err[100];
	int sockfd,desc;
	struct sockaddr_in servaddr;
	int ret,i;
	char **pptr;
	char *hname = malloc(256*sizeof(char));
	char str[120];

        strcpy(str,wfps_database.server_cfg_t.server_url);

	printf("server url is %s\n",str);

	char *token;
	char str1[20],str2[20],str3[20],str4[20];

	/* get the first token */
	token = strtok(str,"/");
	int count=1;

	/* walk through other tokens */
	while( token != NULL )
	{

		if(count == 1){
			printf( " str1 is %s\n", token );
			strcpy(str1,token);                                                               
		}
		else if(count ==2){
			strcpy(str2,token);
			printf("str2 is %s\n",token);
		}
		else if(count ==3){
			strcpy(str3,token);
			printf("str3 is %s\n",token);
		}
		else if(count ==4){
			strcpy(str4,token);
			printf("str4 is %s\n",token);
		}

		token = strtok(NULL, "/");
		count++;
	}

	sprintf(hname,"%s",str2);

	struct hostent *hptr;
	if ((hptr = gethostbyname(hname)) == NULL) {
		fprintf(stderr, " gethostbyname error for host: %s: %s",
				hname, hstrerror(h_errno));

	}
	if (hptr->h_addrtype == AF_INET
			&& (pptr = hptr->h_addr_list) != NULL) {
		printf("address: %s\n",
				inet_ntop(hptr->h_addrtype, *pptr, str,
					sizeof(str)));
	} else {
		fprintf(stderr, "Error call inet_ntop \n");
	}
	
	printf("cfg_mgr - before socket creation\n");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("cfg_mgr - after socket creation\n");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(2000);
	inet_pton(AF_INET, str, &servaddr.sin_addr);

	printf("cfg_mgr - before connect\n");
	desc=connect(sockfd, (SA *) & servaddr, sizeof(servaddr));
	printf("cfg_mgr -after connect\n");
	if(desc < 0){
		printf("Unable to connect to server\n");
		return 0;
	}
	else
	{
		printf("server alive\n");
		return 1;
	}
}



/* Network Configuration */

void Set_Nw_Cfg_page(NW_CFG_T *nw_msg)
{
	NW_CFG_T *info_nw;
	CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
	cmgr_ecode_t        cmgr_error;
        unsigned int buff_size;
	char status = 0;
	char err_str[100];
	        
	buff_size = sizeof(CMD_INTERFACE_T);
        info_nw = (NW_CFG_T*)malloc(sizeof(NW_CFG_T));
/*  malloc check */
	if((NULL == info_nw) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
		return;
        }
//	printf("inside the app functon definition \n");
	memcpy(info_nw,nw_msg,sizeof(NW_CFG_T));

	if (info_nw->dhcp == 0) {
		if (wfps_database.nw_cfg_t.dhcp == 1) {
			dhcp_stop();
		}

		if ( 0 > val_set_ip(info_nw->ip_addr)) {
            		debug(CFG_ERROR,"ip set failure\n\n");
			strcpy(err_str,"ip set failure");
			status = 1;
                        goto ret;
        	}
        
        	if ( 0 > val_set_gateway(info_nw->gateway)) {
			debug(CFG_ERROR,"gateway set failure\n\n");
			strcpy(err_str,"gateway set failure");
			status = 1;
                        goto ret;
        	}
	
		if ( 0 > val_set_netmask(info_nw->netmask)) {
			debug(CFG_ERROR,"netmask set failure\n\n");
			strcpy(err_str,"netmask set failure");
			status = 1;
                        goto ret;
        	}

                if (0 > val_set_dns(info_nw->dns_pri,info_nw->dns_sec)) {
			debug(CFG_ERROR,"dns set failure\n\n");
			strcpy(err_str,"dns set failure");
			status = 1;
                        goto ret;
		}
	} else {
		char pri_if_name[16];

        	get_managment_if_name(pri_if_name);
		
		if (0 > dhcp_start(pri_if_name)) {
			debug(CFG_ERROR,"dhcp ip get failure\n\n");
			strcpy(err_str,"dns set failure");
			status = 1;
                        goto ret;
		}
		memcpy(&(wfps_database.nw_cfg_t.dhcp),&(info_nw->dhcp),sizeof(info_nw->dhcp));
	}


	wfps_db_write_cfg_2_file(); // write to flash
ret:
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}
	
	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
	}

	p_tx_api_hdr->buff_length      = buff_size;
        p_tx_api_hdr->cmd_opcode       = CFGM_WEB_NW_CFG_SET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;

        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
                free(info_nw);                 
                return ;

        }

        //Free the API buffer
        free(p_tx_api_hdr);

free_ret:
        free(info_nw);
//	printf("end of the app functon definition \n");
        return ;
} /* end of Set_Nw_Cfg */

	
void Get_Nw_Cfg_page()
{
        NW_CFG_T *info_nw;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        unsigned int buff_size;
	char status = 0;
	char err_str[100];
	

        buff_size = sizeof(CMD_INTERFACE_T) + sizeof(NW_CFG_T);
        info_nw = (NW_CFG_T*)malloc(sizeof(NW_CFG_T));
/* check the malloc operaton */
	if((NULL == info_nw) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }
//	printf("inside the app functon definition \n");

	if (wfps_database.nw_cfg_t.dhcp == 1) {
		if (0 > dhcp_ip_get()) {
			debug(CFG_ERROR,"dhcp ip get failure\n\n");
			strcpy(err_str,"dhcp ip get failur");
			status = 1;
			goto ret;
		}
		if (0 > get_dns_address()) {
			debug(CFG_ERROR,"dns get failure\n\n");
			strcpy(err_str,"dns get failure");
			status = 1;
			goto ret;
		}
/*TODO adding the get_gateway function
	if (0 > get_gateway()) {
			debug(CFG_ERROR,"dns get failure\n\n");
                        return;
                }     */
	}
	memcpy(info_nw,&wfps_database.nw_cfg_t,sizeof(NW_CFG_T));

ret:	
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}

	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
        	memcpy(p_tx_api_hdr->buff,info_nw,sizeof(NW_CFG_T));
	}

	p_tx_api_hdr->buff_length      = buff_size;
	p_tx_api_hdr->cmd_opcode       = CFGM_WEB_NW_CFG_GET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;
        //Send message on the network
        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
		goto free_ret;
        }

        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_nw);
//	printf("end of the app functon definition \n");
        return ;
}
 /* end of Get_Nw_Sys */


/* Wireless Configuration */

void Set_Wlan_Cfg_page(WLAN_CFG_T *wlan_msg)
{
        WLAN_CFG_T *info_wlan;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        unsigned int buff_size;
	char status = 0;
	char err_str[100];
	char *str;

        buff_size = sizeof(CMD_INTERFACE_T);
        info_wlan = (WLAN_CFG_T*)malloc(sizeof(WLAN_CFG_T));
/* check the malloc operaton */
	if((NULL == info_wlan) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }
//	printf("inside the app functon definition \n");

	memcpy(info_wlan,wlan_msg,sizeof(WLAN_CFG_T));
	memcpy(&wfps_database.wlan_cfg_t,info_wlan,sizeof(WLAN_CFG_T));
	wfps_db_write_cfg_2_file(); // write to flash

if(info_wlan->sta_sec == 1)

{
        sprintf(str,"/bin/sh /board_client.sh %s on %s", info_wlan->sta_ssid,info_wlan->sta_wpa_secret);
        system(str);
}

else

{
        sprintf(str,"/bin/sh /board_client.sh %s off", info_wlan->sta_ssid);
        system(str);
}


ret:
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}
	
	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
	}

	p_tx_api_hdr->buff_length      = buff_size;
        p_tx_api_hdr->cmd_opcode       = CFGM_WEB_WLAN_CFG_SET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;

        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
                free(info_wlan);                 
                return ;

        }

        //Free the API buffer
        free(p_tx_api_hdr);

free_ret:
        free(info_wlan);
//	printf("end of the app functon definition \n");
        return ;
}
 /* end of Set_Nw_Sys */

void Get_Wlan_Cfg_page()
{
        WLAN_CFG_T *info_wlan;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        unsigned int buff_size;
	FILE *fd;
        char *freq;
        unsigned int channel;
	char status = 0;
	char err_str[100];

        buff_size = sizeof(CMD_INTERFACE_T) + sizeof(WLAN_CFG_T);
        info_wlan = (WLAN_CFG_T*)malloc(sizeof(WLAN_CFG_T));
/* check the malloc operaton */
	if((NULL == info_wlan) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }
//	printf("inside the app functon definition \n");

	memcpy(info_wlan,&(wfps_database.wlan_cfg_t),sizeof(WLAN_CFG_T));
#if 0
#define CMD "iwconfig wlan0 > channel.txt"
#define RM_CMD "rm channel.txt"
	system(CMD);

        if ((fd = fopen("channel.txt","r")) != NULL) {
                char line [ 128 ]; /* or other suitable maximum line size */
                fgets(line,sizeof line, fd);
                if ( fgets ( line, sizeof line, fd ) != NULL ) /* read a line 2*/
                {
                        if((strstr(line,"Access Point: Not-Associated")) == NULL) {
                                strtok(line," ");
                                strtok(NULL,":");
                                freq = strtok(NULL," ");
                                if( ! (strncmp(freq,"2.412",5))) {
                                        channel = 1;
                                }else if( ! (strncmp(freq,"2.417",5))) {
                                        channel = 2;
                                }else if( ! (strncmp(freq,"2.422",5))) {
                                        channel = 3;
                                }else if( ! (strncmp(freq,"2.427",5))) {
                                        channel = 4;
                                }else if( ! (strncmp(freq,"2.432",5))) {
                                        channel = 5;
                                }else if( ! (strncmp(freq,"2.437",5))) {
                                        channel = 6;
                                }else if( ! (strncmp(freq,"2.442",5))) {
                                        channel = 7;
                                }else if( ! (strncmp(freq,"2.447",5))) {
                                        channel = 8;
                                }else if( ! (strncmp(freq,"2.452",5))) {
                                        channel = 9;
                                }else if( ! (strncmp(freq,"2.457",5))) {
                                        channel = 10;
                                }else if( ! (strncmp(freq,"2.462",5))) {
                                        channel = 11;
				}else if( ! (strncmp(freq,"2.467",5))) {
                                        channel = 12;
				}else {
                                        channel = 13;
				}
                        } else {
				channel = 0;
			}
                }
                fclose(fd);
		system(RM_CMD);
        } else {
		debug(CFG_ERROR,"file open failure\n\n");
		fclose(fd);
		strcpy(err_str,"dns get failure");
		status = 1;
		goto ret;
        }

	memcpy(&(info_wlan->sta_chnl),&channel,sizeof(channel));
#endif
ret:	
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}

	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
        	memcpy(p_tx_api_hdr->buff,info_wlan,sizeof(WLAN_CFG_T));
	}

	p_tx_api_hdr->buff_length      = buff_size;
	p_tx_api_hdr->cmd_opcode       = CFGM_WEB_WLAN_CFG_GET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;
        //Send message on the network
        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
		goto free_ret;
        }

        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_wlan);
//	printf("end of the app functon definition \n");
        return ;
}
 /* end of Get_Nw_Cfg */

/* Server Configuration */

void Set_Srv_Cfg_page(SERVER_CFG_T *server_msg)
{
        SERVER_CFG_T *info_server;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        unsigned int buff_size;
	char status = 0;
	char err_str[100];

        buff_size = sizeof(CMD_INTERFACE_T);
        info_server = (SERVER_CFG_T*)malloc(sizeof(SERVER_CFG_T));
/* check the malloc operaton */
	if((NULL == info_server) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }
//	printf("inside the app functon definition \n");
	memcpy(info_server,server_msg,sizeof(SERVER_CFG_T));
	memcpy(&(wfps_database.server_cfg_t),info_server,sizeof(SERVER_CFG_T));
	
//	wfps_db_write_cfg_2_file(); // write to flash
ret:
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}
	
	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
	}

	p_tx_api_hdr->buff_length      = buff_size;
        p_tx_api_hdr->cmd_opcode       = CFGM_WEB_SRV_CFG_SET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;

        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
                free(info_server);                 
                return ;

        }

        //Free the API buffer
        free(p_tx_api_hdr);

free_ret:
        free(info_server);
//	printf("inside the app functon definition \n");
        return ;
}
     /* end of Set_Srv_Cfg */

void Get_Srv_Cfg_page(int module)
{
        SERVER_CFG_T *info_server;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        unsigned int buff_size;
	char status = 0;
	char err_str[100];

        buff_size = sizeof(CMD_INTERFACE_T) + sizeof(SERVER_CFG_T);
        info_server = (SERVER_CFG_T*)malloc(sizeof(SERVER_CFG_T));
/* check the malloc operaton */
	if((NULL == info_server) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }
//	printf("inside the app functon definition \n");
	memcpy(info_server,&(wfps_database.server_cfg_t),sizeof(SERVER_CFG_T));
ret:	
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}

	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
        	memcpy(p_tx_api_hdr->buff,info_server,sizeof(SERVER_CFG_T));
	}
	
	if(module == MOD_ID_MAC_UPLD_MGR ){
		p_tx_api_hdr->buff_length      = buff_size;
		p_tx_api_hdr->cmd_opcode       = CFGM_MAC_UPLD_SRV_CFG_GET;
		p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;
		//Send message on the network
		if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
					(cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_MAC_UPLD_MGR,
					&cmgr_error))
		{
			debug(CFG_ERROR,"Send message to MOD_ID_MAC_UPLD_MGR failed\n\n");
			free(p_tx_api_hdr);
			goto free_ret;
		}
	}else {
		p_tx_api_hdr->buff_length      = buff_size;
		p_tx_api_hdr->cmd_opcode       = CFGM_WEB_SRV_CFG_GET;
		p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;
		//Send message on the network
		if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
					(cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
					&cmgr_error))
		{
			debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
			free(p_tx_api_hdr);
			goto free_ret;
		}
	}

        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_server);
//	printf("inside the app functon definition \n");
        return ;
}
 /* end of Get_Srv_Cfg */


/* Network Statistics */

void Get_Net_Stats_page()
{
        NW_STATS_T *info_net_stats;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        unsigned int buff_size;
	char status = 0;
	char err_str[100];
	char pri_if_name[16];
	iwstats rssi_str;
        bool ser_reach;
	struct net_device_stats *net_stats;

        buff_size = sizeof(CMD_INTERFACE_T) + sizeof(NW_STATS_T);
        info_net_stats = (NW_STATS_T*)malloc(sizeof(NW_STATS_T));
/* check the malloc operaton */
	if((NULL == info_net_stats) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }

//	memset(info_net_stats,0,sizeof(NW_STATS_T));
	
	printf("inside the app functon definition \n");
        
	get_managment_if_name(pri_if_name);
#if 1
        net_stats = (struct net_device_stats *) malloc(sizeof(NET_DEVICE_STATS_T));
	if((NULL == net_stats) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
		free(info_net_stats);
                return;
        }

        if( 0 != get_stat(pri_if_name,net_stats)) {
                debug(CFG_ERROR,"error in get_stat \n\n");
		strcpy(err_str,"getting status failure");
                status = 1;
                goto ret;
        }

        ser_reach = server_alive();
        if(ser_reach) {
                debug(CFG_ERROR,"server alive \n");
        } else {
                debug(CFG_ERROR,"server NOt alive \n");
        }

	memset(&rssi_str, 0, sizeof(iwstats));
        /* Get stats */
        iw_getstats(pri_if_name, &(rssi_str));

        printf("rssi = %d \n",rssi_str.qual.level - 0x100);
	printf("Rx bytes = %lu \n",net_stats->rx_bytes);
        printf("Rx packets = %lu \n",net_stats->rx_packets);
        printf("Rx error = %lu \n",net_stats->rx_errors);
        printf("Rx dropped = %lu \n",net_stats->rx_dropped);
        printf("ignore \n");
        printf("muticast = %lu \n",net_stats->multicast);
        printf("Tx bytes = %lu \n",net_stats->tx_bytes);
        printf("Tx packets = %lu \n",net_stats->tx_packets);
        printf("Tx error = %lu \n",net_stats->tx_errors);
        printf("Tx dropped = %lu \n",net_stats->tx_dropped);
        printf("collisions = %lu \n",net_stats->collisions);
	
	info_net_stats->tx_pkts = net_stats->tx_packets;
	info_net_stats->rx_pkts = net_stats->rx_packets;
	info_net_stats->err_pkts =net_stats->tx_packets;
	info_net_stats->server_reachable = ser_reach;
	info_net_stats->rssi = rssi_str.qual.level - 0x100;
        printf("rssi = %d \n",info_net_stats->rssi);
#endif	
ret:	
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}

	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
        	memcpy(p_tx_api_hdr->buff,info_net_stats,sizeof(NW_STATS_T));
	}

	p_tx_api_hdr->buff_length      = buff_size;
	p_tx_api_hdr->cmd_opcode       = CFGM_WEB_NW_STATS_GET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;
        //Send message on the network
        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
		debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
		goto free_ret;
        }

        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_net_stats);
        free(net_stats);
//	printf("inside the app functon definition \n");
        return ;
}
 /* end of Get_Net_Stats */

/* Time Profile Configuration */

void Set_TProf_Cfg_page(TPROFILE_CFG_T *tprof_msg)
{
        TPROFILE_CFG_T *info_tprofile;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        unsigned int buff_size;
	char status = 0;
	char err_str[100];

        buff_size = sizeof(CMD_INTERFACE_T);
        info_tprofile = (TPROFILE_CFG_T*)malloc(sizeof(TPROFILE_CFG_T));
/* check the malloc operaton */
	if((NULL == info_tprofile) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }
//	printf("inside the app functon definition \n");
	memcpy(info_tprofile,tprof_msg,sizeof(TPROFILE_CFG_T));
	memcpy(&(wfps_database.tprofile_cfg_t),info_tprofile,sizeof(TPROFILE_CFG_T));
	
//	wfps_db_write_cfg_2_file(); // write to flash
ret:
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}
	
	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
	}

	p_tx_api_hdr->buff_length      = buff_size;
        p_tx_api_hdr->cmd_opcode       = CFGM_WEB_TPROF_CFG_SET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;

        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
                free(info_tprofile);                 
                return ;

        }

        //Free the API buffer
        free(p_tx_api_hdr);

free_ret:
        free(info_tprofile);
//	printf("inside the app functon definition \n");
        return ;
}
 /* end of Set_TProf_Cfg */

void Get_TProf_Cfg_page()
{
        TPROFILE_CFG_T *info_tprofile;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        unsigned int buff_size;
	char status = 0;
	char err_str[100];

        buff_size = sizeof(CMD_INTERFACE_T) + sizeof(TPROFILE_CFG_T);
        info_tprofile = (TPROFILE_CFG_T*)malloc(sizeof(TPROFILE_CFG_T));
/* check the malloc operaton */
	if((NULL == info_tprofile) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }
//	printf("inside the app functon definition \n");
	memcpy(info_tprofile,&(wfps_database.tprofile_cfg_t),sizeof(TPROFILE_CFG_T));
ret:	
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}

	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
        	memcpy(p_tx_api_hdr->buff,info_tprofile,sizeof(TPROFILE_CFG_T));
	}

	p_tx_api_hdr->buff_length      = buff_size;
	p_tx_api_hdr->cmd_opcode       = CFGM_WEB_TPROF_CFG_GET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;
        //Send message on the network
        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
		goto free_ret;
        }

        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_tprofile);
//	printf("inside the app functon definition \n");
        return ;
}
 /* end of Get_TProf_Cfg */


/* Mac List */
void Get_Mac_List_page(MAC_LIST_T *info_mac_msg)
{

	MAC_LIST_T         *info_mac;
	CMD_INTERFACE_T    *p_tx_api_hdr;   //Pointer to API hdr for the transmit buffer
	cmgr_ecode_t        cmgr_error;
	unsigned int from_mac,to_mac;
        unsigned int buff_size;
        char err_str[100],status=0; 
        unsigned int n=0;
	int mac_count=0, i=0;
	probe ps;
	FILE *fs;

	from_mac = info_mac_msg->frm_mac_count;
	to_mac 	 = info_mac_msg->to_mac_count;
	
	buff_size = sizeof(CMD_INTERFACE_T) + sizeof(MAC_LIST_T);
	info_mac = (MAC_LIST_T*)malloc(sizeof(MAC_LIST_T));

	/* check the malloc operaton */
	if((NULL == info_mac) ){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		return;
	}
	printf("inside the app functon definition \n");

	if ( (fs=fopen(MAC_LIST_FILE_PATH,"r")) != NULL )
	{
		while( fread(&ps,sizeof(ps),1,fs)==1)
		{

			if(n >= from_mac && n <= to_mac)
			{

				sprintf(info_mac->mac[mac_count].time_stamp, "%02d:%02d:%02d:%03d",ps.timestamp[0],ps.timestamp[1],ps.timestamp[2],ps.timestamp[3]);

				sprintf(info_mac->mac[mac_count].mac_addr,"%02x:%02x:%02x:%02x:%02x:%02x",ps.src[0],ps.src[1],ps.src[2],ps.src[3],ps.src[4],ps.src[5]);

				info_mac->mac[mac_count].channel=ps.channel;
				info_mac->mac[mac_count].rssi=ps.rssi;
				strcpy(info_mac->mac[mac_count].tag_id,wfps_database.sys_cfg_t.hostname);
				mac_count++;
			}
			n++;
		}
		info_mac->no_of_mac = mac_count;
		fclose(fs);
	} else {
		debug(CFG_ERROR,"file read failure\n\n");
		strcpy(err_str,"file read failure");
		status = 1;
		goto ret;
	}


ret:	
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}

	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
        	memcpy(p_tx_api_hdr->buff,info_mac,sizeof(MAC_LIST_T));
	}
	p_tx_api_hdr->buff_length      = buff_size;
	p_tx_api_hdr->cmd_opcode       = CFGM_WEB_MAC_LIST_GET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;
        
	//Send message on the network
        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
		goto free_ret;
        }

        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_mac);
        return ;
}

void Set_Mgt_Cfg_page(MGMT_CFG_T *mgmt_cfg)
{
        MGMT_CFG_T *info_mgmt;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        unsigned int buff_size;
	char status = 0;
	char err_str[100];

        buff_size = sizeof(CMD_INTERFACE_T) + sizeof(MGMT_CFG_T);
        info_mgmt = (MGMT_CFG_T*)malloc(sizeof(MGMT_CFG_T));
/* check the malloc operaton */
	if((NULL == info_mgmt) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }
//	printf("inside the app functon definition \n");

	memcpy(info_mgmt,mgmt_cfg,sizeof(MGMT_CFG_T));
	
	if (info_mgmt->save == 1) {
		wfps_db_write_cfg_2_file(); // write to flash
		free(info_mgmt);
//		system("reboot");
	} 
	else if (info_mgmt->reboot == 1) {
		free(info_mgmt);
		system("reboot");
	}
	else if (info_mgmt->factory_def == 1) {
		free(info_mgmt);
		char *str;
		str = malloc(100*sizeof(char));
        	sprintf(str,"rm %s -fr",DB_CFG_FILE_NAME);
        	system(str);		
		free(str);
		system("reboot");
	}

//TODO	Password Change Function
ret:	
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}

	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
	}

	p_tx_api_hdr->buff_length      = buff_size;
	p_tx_api_hdr->cmd_opcode       = CFGM_WEB_MGT_CFG_SET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;
        //Send message on the network
        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
		goto free_ret;
        }

        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_mgmt);
        return ;
}
 /* end of Set_Mgt_Cfg */

/*start of Get_scan_Cfg */
void Get_Scan_Cfg_page(SCAN_CFG_T *info_scan_msg)
{
        SCAN_CFG_T *info_scan;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        unsigned int buff_size;
        int scan_count;
       	char err_str[100], status=0;
        FILE *fs;
	int count=0,len,buff_len;
    	char ps[100],*pch;
        buff_size = sizeof(CMD_INTERFACE_T) + sizeof(SCAN_CFG_T);
        info_scan = (SCAN_CFG_T*)malloc(sizeof(SCAN_CFG_T));
        /* check the malloc operaton */
        if((NULL == info_scan) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }

        system(SCAN);
	 if ( (fs = fopen(SCAN_LIST_FILE_PATH,"r")) != NULL) {

		fseek(fs,0,SEEK_END);
		len=ftell(fs);
		fseek(fs,0,SEEK_SET);
		while(fgets(ps,len,fs) != NULL) {
		//	pch=strtok(ps,":");
		//	while(pch != NULL) {
				buff_len=strlen(ps);
				ps[buff_len-1]='\0';
				printf("%s",ps);	//debug print for SSID
				strcpy(info_scan->scan[count].ssid,ps);
		//		pch=strtok(NULL,":");
		//	}
			count++;
		}

		info_scan->no_count=count;
                fclose(fs);

        } else {
                debug(CFG_ERROR,"file read failure\n");
                strcpy(err_str,"file read failure\n");
                status=1;
                goto ret;
        }

ret:
        if(status){
                buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
        }

        if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                goto free_ret;
        }

        if(status){
                p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
                memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
        }else{
                p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
                memcpy(p_tx_api_hdr->buff,info_scan,sizeof(SCAN_CFG_T));
        }
        p_tx_api_hdr->buff_length      = buff_size;
        p_tx_api_hdr->cmd_opcode       = CFGM_WEB_SCAN_LIST_GET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;

        //Send message on the network
        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
                goto free_ret;
        }
        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_scan);
        return ;
}
/*End of Get_scan_Cfg */

/* Start bulb on off cfg */
	int val_set_bulb(int dim) {
	char *str;
	str = (malloc(sizeof(char)*200));
	if (str == NULL) {
	debug(CFG_ERROR,"on-off malloc error\r\n");
	  return -1;
  	}
   	sprintf(str,"echo %d > /sys/devices/platform/pwm-backlight.3/backlight/pwm-backlight.3/brightness",dim);
   	system (str);
   	free(str);

   	return 0;
  	}

/* On/Off Confifuration */
void Set_Bulb_Cfg_page(BULBSTATS_CFG_T *bulb_msg)
	{
	BULBSTATS_CFG_T *info_bulb;
	CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
	cmgr_ecode_t        cmgr_error;
	char err_str[100];
	unsigned int buff_size;
	char *str;
        str = (malloc(sizeof(char)*200));

	char status = 0;

	buff_size = sizeof(CMD_INTERFACE_T) + sizeof(BULBSTATS_CFG_T);
	info_bulb = (BULBSTATS_CFG_T*)malloc(sizeof(BULBSTATS_CFG_T));
	/*  malloc check */
	if((NULL == info_bulb) ){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		return;
	}
	memcpy(info_bulb,bulb_msg,sizeof(BULBSTATS_CFG_T));

	if(info_bulb->stat_on == 100){
		sprintf(str,"echo 100 > /sys/devices/platform/pwm-backlight.3/backlight/pwm-backlight.3/brightness");
		system (str);
	}	
	else if (info_bulb->stat_off == 1){
		sprintf(str,"echo 0 > /sys/devices/platform/pwm-backlight.3/backlight/pwm-backlight.3/brightness");
		system (str);
	}
	else if (info_bulb->stat_dim != 0){

	if (0 > val_set_bulb(info_bulb->stat_dim)) {
	  debug(CFG_ERROR,"bulb set failure\n\n");
	  strcpy(err_str,"bulb set failure");
	  status = 1;
	  goto ret;
	 }
	}

	//	wfps_db_write_cfg_2_file(); // write to flash
ret:
	if(status){
		buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
	}

	if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
		debug(CFG_ERROR,"Memory allocation failure\n\n");
		goto free_ret;
	}

	if(status){
		p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
		memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
	}else{	
		p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
	}

	p_tx_api_hdr->buff_length      = buff_size;
	p_tx_api_hdr->cmd_opcode       = CFGM_WEB_BULBSTATS_CFG_SET;
	p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;

	if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
				(cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
				&cmgr_error))
	{
		debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
		free(p_tx_api_hdr);
		free(info_bulb);                 
		return ;

	}

	//Free the API buffer
	free(p_tx_api_hdr);

free_ret:
	free(info_bulb);
	return ;
} /*end of Set_bulb_Cfg */

/* Start of blink Cfg */
void Set_Blink_Cfg_page(BLINK_CFG_T *blink_msg)
{
        BLINK_CFG_T *info_blink;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        char err_str[100];
        unsigned int buff_size;

	char *str;
        str = (malloc(sizeof(char)*200));
        char status = 0;

        buff_size = sizeof(CMD_INTERFACE_T) + sizeof(BLINK_CFG_T);
        info_blink = (BLINK_CFG_T*)malloc(sizeof(BLINK_CFG_T));
        /*  malloc check */
        if((NULL == info_blink) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }
        memcpy(info_blink,blink_msg,sizeof(BLINK_CFG_T));

        if(info_blink->blink_on == 1){
                sprintf(str,"echo 100 > /sys/devices/platform/pwm-backlight.3/backlight/pwm-backlight.3/brightness");
                system (str);
		sleep(1);
                sprintf(str,"echo 0 > /sys/devices/platform/pwm-backlight.3/backlight/pwm-backlight.3/brightness");
                system (str);
		sleep(1);
                sprintf(str,"echo 100 > /sys/devices/platform/pwm-backlight.3/backlight/pwm-backlight.3/brightness");
                system (str);
		sleep(1);
                sprintf(str,"echo 0 > /sys/devices/platform/pwm-backlight.3/backlight/pwm-backlight.3/brightness");
                system (str);
		sleep(1);
                sprintf(str,"echo 40 > /sys/devices/platform/pwm-backlight.3/backlight/pwm-backlight.3/brightness");
                system (str);
        }  else if(info_blink->blink_on == 0) {
                sprintf(str,"echo 0 > /sys/devices/platform/pwm-backlight.3/backlight/pwm-backlight.3/brightness");
                system (str);
        }  else {
		debug(CFG_ERROR,"file read failure\n");
                strcpy(err_str,"file read failure\n");
                status=1;
                goto ret;
	}

ret:
        if(status){
                buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
        }

        if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                goto free_ret;
        }

        if(status){
                p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
                memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
        }else{
                p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
                memcpy(p_tx_api_hdr->buff,info_blink,sizeof(BLINK_CFG_T));
        }
        p_tx_api_hdr->buff_length      = buff_size;
        p_tx_api_hdr->cmd_opcode       = CFGM_WEB_BLINK_CFG_SET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;

        //Send message on the network
        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
                goto free_ret;
        }
        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_blink);
        return ;
}
/* end of blink cfg */

/*start of bulb power cfg*/
void Get_Power_Cfg_page(POWER_CFG_T *power_msg)
{
        POWER_CFG_T *info_power;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        char err_str[100], status=0;
        unsigned int buff_size;
	FILE *fs;
	int ps;
	int bright_val;
	float power_val;

	buff_size = sizeof(CMD_INTERFACE_T) + sizeof(POWER_CFG_T);
        info_power = (POWER_CFG_T*)malloc(sizeof(POWER_CFG_T));
	/* check the malloc operaton */
        if((NULL == info_power) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }
	
	if ( (fs = fopen(BACKLIGHT_VALUE,"r")) != NULL) {
		while(fscanf(fs, "%d" , &ps)==1) {
	//	printf("######file value=%d\n",ps);
		bright_val=ps;
	//	printf("######bright_val=%d\n",bright_val);
		}
		power_val=(0.1125*bright_val)+1;
	//	printf("##########power=%f\n",power_val);
		info_power->pwr=power_val;
	//	printf("##########power_cfg=%f\n",info_power->pwr);
	fclose(fs);
	
	} else {
                debug(CFG_ERROR,"file read failure\n");
                strcpy(err_str,"file read failure\n");
                status=1;
                goto ret;
        }
ret:
        if(status){
                buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
        }

        if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                goto free_ret;
        }

        if(status){
                p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
                memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
        }else{
                p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
                memcpy(p_tx_api_hdr->buff,info_power,sizeof(POWER_CFG_T));
        }
        p_tx_api_hdr->buff_length      = buff_size;
        p_tx_api_hdr->cmd_opcode       = CFGM_WEB_POWER_CFG_GET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;

        //Send message on the network
        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
                goto free_ret;
        }
        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_power);
        return ;
}
/*end of bulb power cfg*/

/**Polling for backlight value**/
void Get_Poll_Cfg_page(POLL_CFG_T *poll_msg)
{
        POLL_CFG_T *info_poll;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        char err_str[100], status=0;
        unsigned int buff_size;
        FILE *fs;
       int ps;
        int bright_val;

        buff_size = sizeof(CMD_INTERFACE_T) + sizeof(POLL_CFG_T);
        info_poll = (POLL_CFG_T*)malloc(sizeof(POLL_CFG_T));
        /* check the malloc operaton */
        if((NULL == info_poll) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }

        if ( (fs = fopen(BACKLIGHT_VALUE,"r")) != NULL) {
                while(fscanf(fs, "%d" , &ps)==1) {
//		printf("######file value=%d\n",ps);
                bright_val=ps;
//		printf("######bright value=%d\n",bright_val);
                }
		info_poll->poll=bright_val;
        fclose(fs);

        } else {
                debug(CFG_ERROR,"file read failure\n");
                strcpy(err_str,"file read failure\n");
                status=1;
                goto ret;
        }
ret:
        if(status){
                buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
        }

        if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                goto free_ret;
	}
        if(status){
                p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
                memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
        }else{
                p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
                memcpy(p_tx_api_hdr->buff,info_poll,sizeof(POLL_CFG_T));
        }
        p_tx_api_hdr->buff_length      = buff_size;
        p_tx_api_hdr->cmd_opcode       = CFGM_WEB_POLL_CFG_GET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;

        //Send message on the network
        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
                goto free_ret;
        }
        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_poll);
   	return;
}

/*start of bulb blackscholes cfg*/
void Get_Black_Cfg_page(BLACK_CFG_T *black_msg)
{
        BLACK_CFG_T *info_black;
        CMD_INTERFACE_T     *p_tx_api_hdr; //Pointer to API hdr for the transmit buffer
        cmgr_ecode_t        cmgr_error;
        char err_str[100], status=0;
        unsigned int buff_size;
        FILE *fs;
        char *ps;
        float black_val;
	char line[100];

        buff_size = sizeof(CMD_INTERFACE_T) + sizeof(BLACK_CFG_T);
        info_black = (BLACK_CFG_T*)malloc(sizeof(BLACK_CFG_T));
        /* check the malloc operaton */
        if((NULL == info_black) ){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                return;
        }

	char *str;
        str = malloc(100*sizeof(char));
	sprintf(str,"./blacscholes 4 /in_4.txt /tmp/out.txt");
	system (str);
        free(str);
        if ( (fs = fopen("/tmp/out.txt","r")) != NULL) {
		fgets(line, sizeof(line), fs);
		fgets(line, sizeof(line), fs);
		
		ps = malloc(100);
	
		if(!ps) {
			perror("malloc");
			return -1;
		}
		
		sscanf(line,"%s",ps);

	//	info_black->blk = malloc(100);
	//	if(!info_black->blk) {
	//		perror("malloc");
	//		return -1;
	//	}
		
                info_black->blk=atof(ps);
        //      printf("##########black_cfg=%f\n",info_power->blk);
        fclose(fs);
        } else {
                debug(CFG_ERROR,"file read failure\n");
                strcpy(err_str,"file read failure\n");
                status=1;
                goto ret;
        }
ret:
        if(status){
                buff_size = sizeof(CMD_INTERFACE_T) + strlen(err_str);
        }

        if((p_tx_api_hdr = (CMD_INTERFACE_T*) malloc(buff_size)) == NULL){
                debug(CFG_ERROR,"Memory allocation failure\n\n");
                goto free_ret;
        }
        if(status){
                p_tx_api_hdr->cmd_exec_status = API_CMD_FAILED;
                memcpy(p_tx_api_hdr->buff,err_str,strlen(err_str));
        }else{
                p_tx_api_hdr->cmd_exec_status  = API_CMD_PROCESSED;
                memcpy(p_tx_api_hdr->buff,info_black,sizeof(BLACK_CFG_T));
        }
        p_tx_api_hdr->buff_length      = buff_size;
        p_tx_api_hdr->cmd_opcode       = CFGM_WEB_BLACK_CFG_GET;
        p_tx_api_hdr->module_id        = MOD_ID_CFG_MGR;

        //Send message on the network
        if(CFGM_SUCCESS != cmgr_send_buff_to_module(MOD_ID_CFG_MGR, g_connection_id,
                                (cmgr_s8bit*)p_tx_api_hdr, buff_size, MOD_ID_WEB_SRV,
                                &cmgr_error))
        {
                debug(CFG_ERROR,"Send message to MOD_ID_WEB_SRV failed\n\n");
                free(p_tx_api_hdr);
                goto free_ret;
        }
        //Free the API buffer
        free(p_tx_api_hdr);
free_ret:
        free(info_black);
        return ;
}
/*end of blackscholes API*/ 
