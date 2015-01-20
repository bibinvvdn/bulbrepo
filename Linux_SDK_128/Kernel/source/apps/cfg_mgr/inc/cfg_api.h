#ifndef __CFG_API_H_
#define __CFG_API_H_

int val_set_ip(char *);
int val_set_gateway(char *gate_way);
int val_set_netmask(char* net_mask);
int val_set_dns(char *dns1,char *dns2);
int dhcp_ip_get();
int get_dns_address();

int val_set_bulb(int);
int mac_read(char *mac);
int hw_vrn_read(char *hd_vr);
int sw_vrn_read(char *sw_vr);
int server_alive();
#endif /*  __CFG_API_H_ */
