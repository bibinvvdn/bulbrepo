#ifndef __NETWORK_H__
#define __NETWORK_H__
//#include <net/if.h>
//#include <linux/wireless.h>

typedef struct iw_statistics    iwstats;

typedef struct net_device_stats {
	unsigned long rx_packets;	/* total packets received       */
	unsigned long tx_packets;	/* total packets transmitted    */
	unsigned long rx_bytes;	/* total bytes received         */
	unsigned long tx_bytes;	/* total bytes transmitted      */
	unsigned long rx_errors;	/* bad packets received         */
	unsigned long tx_errors;	/* packet transmit problems     */
	unsigned long rx_dropped;	/* no space in linux buffers    */
	unsigned long tx_dropped;	/* no space available in linux  */
	unsigned long multicast;	/* multicast packets received   */
	unsigned long collisions;
} NET_DEVICE_STATS_T;

int set_mac_address(const char * ifname, unsigned char *hwaddr);
int get_mac_address(const char *interface, unsigned char *mac);
int get_ipaddress(const char *interface, unsigned int *ip_addr);
int set_ipaddress(const char *interface, int addr);
int get_brdcst_address(const char *interface, unsigned int *brd_addr);
int set_brdcst_address(const char *interface, int addr);
int get_netmask(const char *interface, unsigned int *netmask);
int set_netmask(const char *interface, int netmask);
int get_stat(const char *ifname, struct net_device_stats *stats);
int get_interface_flags(const char *ifname);
int set_interface_flags(const char *ifname, short flag);
int INET_setroute(int, int, char**);
int configure_route(char*);
int check_interfaces();
int iw_getstats(char *ifname,iwstats *rssi_str);
/* DHCP APIs */
int dhcp_start(char *interface);
int dhcp_stop(void );
#endif /* __NETWORK_H__ */
