#include <stdio.h>
#include <string.h>
//#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include <linux/ioctl.h>
#include <math.h>
#include <fcntl.h>
#include <netdb.h>
//#include <linux/wireless.h>

#include "network.h"

#define debug(x...) fprintf(stderr,x)
#define PRIMARY_IF "br0"
#define _PATH_PROCNET_DEV "/proc/net/dev"

int get_managment_ip(unsigned int *ip)
{
	struct ifconf ifc;
	struct ifreq *ifr;
	struct sockaddr_in *broad_addr;
	struct sockaddr_in dest_addr;
	char buf[512], if_name[IF_NAMESIZE];
	int sentbytes;
	int i, s, ret = 0;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s == -1) {
		perror("socket");
		return -1;
	}

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;

	if(ioctl(s, SIOCGIFCONF, (char*) &ifc) < 0) {
		perror("ioctl SIOCGIFCONF");
		exit(1);
	}

	ifr = ifc.ifc_req;
	/* For each interface */
	for(i = 0;  i < ifc.ifc_len/sizeof(struct ifreq); i++, ifr++) {
		strncpy(if_name, ifr->ifr_name, sizeof(if_name));
		if(ifr->ifr_addr.sa_family != AF_INET) {
			printf("%s: Not AF_INET\n", if_name);
			continue;
		}
		if(ioctl(s, SIOCGIFFLAGS, (char*) ifr) < 0) {
			perror("ioctl SIOCGIFFLAGS");
			continue;
		}
		if(ifr->ifr_flags & IFF_LOOPBACK) {
			continue;
		}
		if(ioctl(s, SIOCGIFADDR, (char*) ifr) < 0) {
			perror("ioctl SIOCGIFBRDADDR");
			continue;
		}
		broad_addr = (struct sockaddr_in*)&ifr->ifr_broadaddr;
		//printf("ipaddr %s %s\n", if_name, inet_ntoa(broad_addr->sin_addr));
		close(s);
		*ip = broad_addr->sin_addr.s_addr;  
		return 0;
	}

	close(s);
	return -1;
}

int get_ipaddress(const char *interface, unsigned int *ip_addr)
{
	int skfd;
	int err;
	struct ifreq ifr;
	struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;

	strncpy(ifr.ifr_name, interface, sizeof(ifr.ifr_name));
	ifr.ifr_addr.sa_family = AF_INET;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		err = skfd * errno;
		debug("unable to open socket: %d\n", err);
		return err;
	}

	err = ioctl(skfd, SIOCGIFADDR, &ifr);
	err *= errno;
	close(skfd);
	*ip_addr = sin->sin_addr.s_addr;

	if(err) {
		debug("ioctl SIOCGIFADDR failed: %d\n", err);
	}
	return err;
}

int set_ipaddress(const char *interface, int addr)
{
	int skfd;
	struct ifreq ifr;
	int err;
	struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;

	strncpy(ifr.ifr_name, interface, sizeof(ifr.ifr_name));
	ifr.ifr_addr.sa_family = AF_INET;
	sin->sin_addr.s_addr = addr;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0){
		err = skfd * errno;
		debug("unable to open socket: %d", err);
		return err;
	}

	err = ioctl(skfd, SIOCSIFADDR, &ifr);
	err *= errno;
	close(skfd);
	if(err) {
		debug("ioctl SIOCSIFADDR failed: %d\n", err);
	}
	return err;
}

int get_brdcst_address(const char *interface, unsigned int *brd_addr)
{
	int skfd;
	int err;
	struct ifreq ifr;
	struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_broadaddr;

	strncpy(ifr.ifr_name, interface, sizeof(ifr.ifr_name));

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		debug("unable to open socket: %d", errno);
		return -1 * errno;
	}
	err = ioctl(skfd, SIOCGIFBRDADDR, &ifr);
	err *= errno;
	close(skfd);
	*brd_addr = sin->sin_addr.s_addr;

	if(err) {
		debug("ioctl SIOCGIFBRDADDR failed: %d\n", err);
	}
	return err;
}

int set_brdcst_address(const char *interface, int addr)
{
	int skfd;
	struct ifreq ifr;
	struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_broadaddr;
	int err;

	strncpy(ifr.ifr_name, interface, sizeof(ifr.ifr_name));
	ifr.ifr_addr.sa_family = AF_INET;
	sin->sin_addr.s_addr = addr;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		err = skfd * errno;
		debug("unable to open socket: %d", err);
		return err;
	}

	err = ioctl(skfd, SIOCSIFBRDADDR, &ifr);
	err *= errno;
	close(skfd);
	if(err) {
		debug("ioctl SIOCSIFBRDADDR failed: %d\n", err);
	}
	return err;
}

int get_netmask(const char *interface, unsigned int *netmask)
{
	int skfd;
	int err;
	struct ifreq ifr;
	struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_netmask;

	strncpy(ifr.ifr_name, interface, sizeof(ifr.ifr_name));

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		err = skfd * errno;
		debug("unable to open socket: %d", err);
		return err;
	}

	err = ioctl(skfd, SIOCGIFNETMASK, &ifr);
	err *= errno;
	close(skfd);
	*netmask = sin->sin_addr.s_addr;

	if(err) {
		debug("ioctl SIOCGIFNETMASK failed: %d\n", err);
	}
	return err;
}

int set_netmask(const char *interface, int netmask)
{
	int skfd;
	struct ifreq ifr;
	struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_netmask;
	int err;

	strncpy(ifr.ifr_name, interface, sizeof(ifr.ifr_name));
	ifr.ifr_addr.sa_family = AF_INET;
	sin->sin_addr.s_addr = netmask;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		err = skfd * errno;
		debug("unable to open socket: %d", err);
		return err;
	}

	err = ioctl(skfd, SIOCSIFNETMASK, &ifr);
	err *= errno;
	close(skfd);
	if(err) {
		debug("ioctl SIOCSIFNETMASK failed: %d\n", err);
	}
	return err;
}

static char *get_name(char *name, char *p)
{
	while (isspace(*p))
		p++;
	while (*p) {
		if (isspace(*p))
			break;
		if (*p == ':') {	/* could be an alias */
			char *dot = p, *dotname = name;
			*name++ = *p++;
			while (isdigit(*p))
				*name++ = *p++;
			if (*p != ':') {	/* it wasn't, backup */
				p = dot;
				name = dotname;
			}
			if (*p == '\0')
				return NULL;
			p++;
			break;
		}
		*name++ = *p++;
	}
	*name++ = '\0';
	return p;
}

static int get_dev_fields(char *bp, struct net_device_stats *stats)
{
	unsigned long ignore;

	sscanf(bp,
	       "%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
	       &stats->rx_bytes,
	       &stats->rx_packets,
	       &stats->rx_errors,
	       &stats->rx_dropped,
	       &ignore,
	       &ignore,
	       &ignore,
	       &stats->multicast,
	       &stats->tx_bytes,
	       &stats->tx_packets,
	       &stats->tx_errors,
	       &stats->tx_dropped,
	       &ignore, &stats->collisions, &ignore, &ignore);

//	printf("Rx bytes = %lu \n",stats->rx_bytes);
//        printf("Rx packets = %lu \n",stats->rx_packets);
//        printf("Rx error = %lu \n",stats->rx_errors);

	return 0;
}

int get_stat(const char *ifname, struct net_device_stats *stats)
{
	FILE *fh;
	char buf[512];

	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh)
		return -ENOTSUP;

	/* Eat first two lines */
	fgets(buf, sizeof buf, fh);
	fgets(buf, sizeof buf, fh);

	while (fgets(buf, sizeof buf, fh)) {
		char *s, name[IFNAMSIZ];
		s = get_name(name, buf);
		if (!strcmp(name, ifname)) {
			get_dev_fields(s, stats);
			fclose(fh);
			return 0;
		}
	}
	fclose(fh);
	return -ENODEV;
}

int get_interface_flags(const char *ifname)
{
	struct ifreq ifr;
	int skfd, err;

	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		err = skfd * errno;
		debug("unable to open socket: %d", err);
		return err;
	}

	err = ioctl(skfd, SIOCGIFFLAGS, &ifr);
	err *= errno;
	close(skfd);
	if(err) {
		debug("ioctl SIOCGIFFLAGS failed: %d\n", err);
		return err;
	}
	return ifr.ifr_flags;
}

int set_interface_flags(const char *ifname, short flag)
{
	struct ifreq ifr;
	int skfd, err;

	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		err = skfd * errno;
		debug("unable to open socket: %d", err);
		return err;
	}

	err = ioctl(skfd, SIOCGIFFLAGS, &ifr);
	err *= errno;
	if (err) {
		return err;
		debug("ioctl SIOCGIFFLAGS failed: %d\n", err);
	}
	ifr.ifr_flags |= flag;
	err = ioctl(skfd, SIOCSIFFLAGS, &ifr);
	err *= errno;
	if (err) {
		debug("ioctl SIOCSIFFLAGS failed: %d\n", err);
		return err;
	}
	return 0;
}

int get_mac_address(const char * ifname, unsigned char *hwaddr)
{
	struct ifreq ifr;
	int skfd, err;

	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		err = skfd * errno;
		debug("unable to open socket: %d", err);
		return err;
	}

	err = ioctl(skfd, SIOCGIFHWADDR, &ifr);
	err *= errno;
	if (err) {
		debug("ioctl SIOCGIFFLAGS failed: %d\n", err);
		close(skfd);
		return err;
	}

	memcpy(hwaddr, ifr.ifr_hwaddr.sa_data, 8);
	close(skfd);
	return 0;
}

int set_mac_address(const char * ifname, unsigned char *hwaddr)
{
	struct ifreq ifr;
	int skfd, err;

	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
	memcpy(ifr.ifr_hwaddr.sa_data, hwaddr, 8);
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		err = skfd * errno;
		debug("unable to open socket: %d", err);
		return err;
	}

	err = ioctl(skfd, SIOCSIFHWADDR, &ifr);
	err *= errno;
	if (err) {
		debug("ioctl SIOCSIFHWADDR failed: %d\n", err);
		close(skfd);
		return err;
	}

	close(skfd);
	return 0;
}
int get_managment_if_name(char *if_name)
{
    FILE *fh;
    char buf[512];
    char pri_if[IFNAMSIZ] = PRIMARY_IF;
    fh = fopen(_PATH_PROCNET_DEV, "r");
    if (!fh)
        return -ENOTSUP;

    /* Eat first two lines */
    fgets(buf, sizeof buf, fh);
    fgets(buf, sizeof buf, fh);

    while (fgets(buf, sizeof buf, fh)) {
        char *s, name[IFNAMSIZ];
        s = get_name(name, buf);
        if(strstr(name,pri_if)){
                strncpy(pri_if,name,IFNAMSIZ);
        }
    }
    strncpy(if_name,pri_if,IFNAMSIZ);
    fclose(fh);
    return 0;
}

int check_interfaces(){
	struct ifconf ifconf;
	struct ifreq ifr[50];
	int s,ifs;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
		return 0;
	ifconf.ifc_buf = (char *) ifr;
	ifconf.ifc_len = sizeof ifr;
	if (ioctl(s, SIOCGIFCONF, &ifconf) == -1)
		return 0;
	ifs = ifconf.ifc_len / sizeof(ifr[0]);
	//      printf("\n\ninterfaces = %d\n", ifs);
	close(s);
	return ifs;
}

#if 0

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

#endif
