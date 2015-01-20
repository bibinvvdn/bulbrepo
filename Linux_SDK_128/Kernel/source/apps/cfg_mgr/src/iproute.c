#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <asm/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/route.h>
#include <netinet/in.h>
#include <linux/sockios.h>
#include <arpa/inet.h>
#include "net-support.h"
#include "network.h"

int INET_setroute(int action, int options, char **args);

#define HAVE_NEW_ADDRT 1
#if HAVE_NEW_ADDRT
#define mask_in_addr(x) (((struct sockaddr_in *)&((x).rt_genmask))->sin_addr.s_addr)
#define full_mask(x) (x)
#else
#define mask_in_addr(x) ((x).rt_genmask)
#define full_mask(x) (((struct sockaddr_in *)&(x))->sin_addr.s_addr)
#endif

#define fprintf(x, arg...) do{}while(0);
#define usage() -EINVAL
#define _(str) str
struct addr {
	struct sockaddr_in addr;
	char *name;
	int host;
	struct addr *next;
};

struct service {
	int number;
	char *name;
	struct service *next;
};

/* Like strncpy but make sure the resulting string is always 0 terminated. */
char *safe_strncpy(char *dst, const char *src, size_t size)
{
	dst[size - 1] = '\0';
	return strncpy(dst, src, size - 1);
}

int dhcp_start(char *interface)
{
        char sys_cmd[100];;

        system("killall udhcpc");

        snprintf(sys_cmd, 100, "/sbin/udhcpc -i %s ", interface);
        return system(sys_cmd);
}
int dhcp_stop(void )
{
            system("killall udhcpc");
                return 0;
}

static void INET_reserror(char *text)
{
	herror(text);
}

int INET_rinput(int action, int options, char **args)
{
	if (action == RTACTION_FLUSH) {
		fprintf(stderr,
			_("Flushing `inet' routing table not supported\n"));
		return (usage());
	}
	if (options & FLAG_CACHE) {
		fprintf(stderr,
			_("Modifying `inet' routing cache not supported\n"));
		return (usage());
	}
	if ((*args == NULL) || (action == RTACTION_HELP))
		return (usage());

	return (INET_setroute(action, options, args));
}
static int INET_resolve(char *name, struct sockaddr_in *sin, int hostfirst)
{
	struct hostent *hp;
	struct netent *np;

	/* Grmpf. -FvK */
	sin->sin_family = AF_INET;
	sin->sin_port = 0;

	/* Default is special, meaning 0.0.0.0. */
	if (!strcmp(name, "default")) {
		sin->sin_addr.s_addr = INADDR_ANY;
		return (1);
	}
	/* Look to see if it's a dotted quad. */
	if (inet_aton(name, &sin->sin_addr)) {
		return 0;
	}
	/* If we expect this to be a hostname, try hostname database first */
#ifdef DEBUG
	if (hostfirst)
		fprintf(stderr, "gethostbyname (%s)\n", name);
#endif
	if (hostfirst && (hp = gethostbyname(name)) != (struct hostent *)NULL) {
		memcpy((char *)&sin->sin_addr, (char *)hp->h_addr_list[0],
		       sizeof(struct in_addr));
		return 0;
	}
	/* Try the NETWORKS database to see if this is a known network. */
#ifdef DEBUG
	fprintf(stderr, "getnetbyname (%s)\n", name);
#endif
	if ((np = getnetbyname(name)) != (struct netent *)NULL) {
		sin->sin_addr.s_addr = htonl(np->n_net);
		return 1;
	}
	if (hostfirst) {
		/* Don't try again */
		errno = h_errno;
		return -1;
	}
#ifdef DEBUG
	res_init();
	_res.options |= RES_DEBUG;
#endif

#ifdef DEBUG
	fprintf(stderr, "gethostbyname (%s)\n", name);
#endif
	if ((hp = gethostbyname(name)) == (struct hostent *)NULL) {
		errno = h_errno;
		return -1;
	}
	memcpy((char *)&sin->sin_addr, (char *)hp->h_addr_list[0],
	       sizeof(struct in_addr));

	return 0;
}

static int INET_getsock(char *bufp, struct sockaddr *sap)
{
	char *sp = bufp, *bp;
	unsigned int i;
	unsigned val;
	struct sockaddr_in *sin;

	sin = (struct sockaddr_in *)sap;
	sin->sin_family = AF_INET;
	sin->sin_port = 0;

	val = 0;
	bp = (char *)&val;
	for (i = 0; i < sizeof(sin->sin_addr.s_addr); i++) {
		*sp = toupper(*sp);

		if ((*sp >= 'A') && (*sp <= 'F'))
			bp[i] |= (int)(*sp - 'A') + 10;
		else if ((*sp >= '0') && (*sp <= '9'))
			bp[i] |= (int)(*sp - '0');
		else
			return (-1);

		bp[i] <<= 4;
		sp++;
		*sp = toupper(*sp);

		if ((*sp >= 'A') && (*sp <= 'F'))
			bp[i] |= (int)(*sp - 'A') + 10;
		else if ((*sp >= '0') && (*sp <= '9'))
			bp[i] |= (int)(*sp - '0');
		else
			return (-1);

		sp++;
	}
	sin->sin_addr.s_addr = htonl(val);

	return (sp - bufp);
}

static int INET_input(int type, char *bufp, struct sockaddr *sap)
{
	switch (type) {
	case 1:
		return (INET_getsock(bufp, sap));
	case 256:
		return (INET_resolve(bufp, (struct sockaddr_in *)sap, 1));
	default:
		return (INET_resolve(bufp, (struct sockaddr_in *)sap, 0));
	}
}

static int INET_getnetmask(char *adr, struct sockaddr *m, char *name)
{
	struct sockaddr_in *mask = (struct sockaddr_in *)m;
	char *slash, *end;
	int prefix;

	if ((slash = strchr(adr, '/')) == NULL)
		return 0;

	*slash++ = '\0';
	prefix = strtoul(slash, &end, 0);
	if (*end != '\0')
		return -1;

	if (name) {
		sprintf(name, "/%d", prefix);
	}
	mask->sin_family = AF_INET;
	mask->sin_addr.s_addr = htonl(~(0xffffffffU >> prefix));
	return 1;
}
int INET_setroute(int action, int options, char **args)
{
	struct rtentry rt;
	char target[128], gateway[128] = "NONE", netmask[128] = "default";
	int xflag, isnet, skfd = -1;

	xflag = 0;

	if (!strcmp(*args, "#net")) {
		xflag = 1;
		args++;
	} else if (!strcmp(*args, "#host")) {
		xflag = 2;
		args++;
	}
	if (*args == NULL)
		return -EINVAL;

	safe_strncpy(target, *args++, (sizeof target));

	/* Clean out the RTREQ structure. */
	memset((char *)&rt, 0, sizeof(struct rtentry));

	/* Special hack for /prefix syntax */
	{
		union {
			struct sockaddr_in m;
			struct sockaddr d;
		} mask;
		int n;

		n = INET_getnetmask(target, &mask.d, netmask);
		if (n < 0)
			return -EINVAL;
		else if (n)
			rt.rt_genmask = full_mask(mask.d);
	}

	/* Prefer hostname lookup is -host flag was given */
	if ((isnet =
	     INET_input((xflag != 2 ? 0 : 256), target, &rt.rt_dst)) < 0) {
		INET_reserror(target);
		return (1);
	}
	switch (xflag) {
	case 1:
		isnet = 1;
		break;
	case 2:
		isnet = 0;
		break;
	default:
		break;
	}

	/* Fill in the other fields. */
	rt.rt_flags = (RTF_UP | RTF_HOST);
	if (isnet)
		rt.rt_flags &= ~RTF_HOST;

	while (*args) {
		if (!strcmp(*args, "metric")) {
			int metric;

			args++;
			if (!*args || !isdigit(**args))
				return -EINVAL;
			metric = atoi(*args);
#if HAVE_NEW_ADDRT
			rt.rt_metric = metric + 1;
#else
			ENOSUPP("inet_setroute", "NEW_ADDRT (metric)");
#endif
			args++;
			continue;
		}
		if (!strcmp(*args, "netmask")) {
			struct sockaddr mask;

			args++;
			if (!*args || mask_in_addr(rt))
				return -EINVAL;
			safe_strncpy(netmask, *args, (sizeof netmask));
			if ((isnet = INET_input(0, netmask, &mask)) < 0) {
				INET_reserror(netmask);
				return (E_LOOKUP);
			}
			rt.rt_genmask = full_mask(mask);
			args++;
			continue;
		}
		if (!strcmp(*args, "gw") || !strcmp(*args, "gateway")) {
			args++;
			if (!*args)
				return -EINVAL;
			if (rt.rt_flags & RTF_GATEWAY)
				return (usage());
			safe_strncpy(gateway, *args, (sizeof gateway));
			if ((isnet =
			     INET_input(256, gateway, &rt.rt_gateway)) < 0) {
				INET_reserror(gateway);
				return (E_LOOKUP);
			}
			if (isnet) {
				fprintf(stderr,
					_
					("route: %s: cannot use a NETWORK as gateway!\n"),
					gateway);
				return (E_OPTERR);
			}
			rt.rt_flags |= RTF_GATEWAY;
			args++;
			continue;
		}
		if (!strcmp(*args, "mss") || !strcmp(*args, "mtu")) {
			args++;
			rt.rt_flags |= RTF_MSS;
			if (!*args)
				return (usage());
			rt.rt_mss = atoi(*args);
			args++;
			if (rt.rt_mss < 64 || rt.rt_mss > 65536) {
				fprintf(stderr, _("route: Invalid MSS/MTU.\n"));
				return (E_OPTERR);
			}
			continue;
		}
		if (!strcmp(*args, "window")) {
			args++;
			if (!*args)
				return (usage());
			rt.rt_flags |= RTF_WINDOW;
			rt.rt_window = atoi(*args);
			args++;
			if (rt.rt_window < 128) {
				/* fprintf(stderr, _("route: Invalid window.\n")); */
				return (E_OPTERR);
			}
			continue;
		}
		if (!strcmp(*args, "irtt")) {
			args++;
			if (!*args)
				return -EINVAL;
			args++;
#if HAVE_RTF_IRTT
			rt.rt_flags |= RTF_IRTT;
			rt.rt_irtt = atoi(*(args - 1));
			rt.rt_irtt *= (HZ / 100);	/* FIXME */
#if 0				/* FIXME: do we need to check anything of this? */
			if (rt.rt_irtt < 1 || rt.rt_irtt > (120 * HZ)) {
				fprintf(stderr,
					_("route: Invalid initial rtt.\n"));
				return (E_OPTERR);
			}
#endif
#else
			ENOSUPP("inet_setroute", "RTF_IRTT");
#endif
			continue;
		}
		if (!strcmp(*args, "reject")) {
			args++;
#if HAVE_RTF_REJECT
			rt.rt_flags |= RTF_REJECT;
#else
			ENOSUPP("inet_setroute", "RTF_REJECT");
#endif
			continue;
		}
		if (!strcmp(*args, "mod")) {
			args++;
			rt.rt_flags |= RTF_MODIFIED;
			continue;
		}
		if (!strcmp(*args, "dyn")) {
			args++;
			rt.rt_flags |= RTF_DYNAMIC;
			continue;
		}
		if (!strcmp(*args, "reinstate")) {
			args++;
			rt.rt_flags |= RTF_REINSTATE;
			continue;
		}
		if (!strcmp(*args, "device") || !strcmp(*args, "dev")) {
			args++;
			if (rt.rt_dev || *args == NULL)
				return -EINVAL;
			rt.rt_dev = *args++;
			continue;
		}
		/* nothing matches */
		if (!rt.rt_dev) {
			rt.rt_dev = *args++;
			if (*args)
				return -EINVAL;	/* must be last to catch typos */
		} else
			return -EINVAL;
	}

#if HAVE_RTF_REJECT
	if ((rt.rt_flags & RTF_REJECT) && !rt.rt_dev)
		rt.rt_dev = "lo";
#endif

	/* sanity checks.. */
	if (mask_in_addr(rt)) {
		__u32 mask = ~ntohl(mask_in_addr(rt));
		if ((rt.rt_flags & RTF_HOST) && mask != 0xffffffff) {
			fprintf(stderr,
				_
				("route: netmask %.8x doesn't make sense with host route\n"),
				mask);
			return (E_OPTERR);
		}
		if (mask & (mask + 1)) {
			fprintf(stderr, _("route: bogus netmask %s\n"),
				netmask);
			return (E_OPTERR);
		}
		mask = ((struct sockaddr_in *)&rt.rt_dst)->sin_addr.s_addr;
		if (mask & ~mask_in_addr(rt)) {
			fprintf(stderr,
				_
				("route: netmask doesn't match route address\n"));
			return (E_OPTERR);
		}
	}
	/* Fill out netmask if still unset */
	if ((action == RTACTION_ADD) && rt.rt_flags & RTF_HOST)
		mask_in_addr(rt) = 0xffffffff;

	/* Create a socket to the INET kernel. */
	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return (E_SOCK);
	}
	/* Tell the kernel to accept this route. */
	if (action == RTACTION_DEL) {
		if (ioctl(skfd, SIOCDELRT, &rt) < 0) {
			perror("SIOCDELRT");
			close(skfd);
			return (E_SOCK);
		}
	} else {
		if (ioctl(skfd, SIOCADDRT, &rt) < 0) {
			perror("SIOCADDRT");
			close(skfd);
			return (E_SOCK);
		}
	}

	/* Close the socket. */
	(void)close(skfd);
	return (0);
}

int str_to_argv(char *str, char **argv, int maxargs)
{
	int i;

	i = 0;
	argv[i] = str;
	//puts(str);
	while(argv[i] && i < maxargs) {
		i++;
		argv[i] = strchr(argv[i-1], ' ');
		if(!argv[i])
			break;
		*argv[i] = '\0';
		argv[i]++;
		if(!strlen(argv[i-1])) {
			argv[i-1] = argv[i];
			i--;
		}
	}

	return i;
}

#define MAX_ARGS 10
int configure_route(char *str)
{
	char *argv[MAX_ARGS];

	str_to_argv(str, argv, MAX_ARGS);
	if(!strncmp("add", argv[0], 3))
		return INET_setroute(RTACTION_ADD, 0, &argv[1]);
	else if(!strncmp("del", argv[0], 3))
		return INET_setroute(RTACTION_DEL, 0, &argv[1]);

	return 0;
}
