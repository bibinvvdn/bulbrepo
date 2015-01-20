#ifdef  NETHRA_IPCAM
 // For  imx27 target build 
 #define HTACCESS_DOCROOT        "/usr/htdocs/.htaccess"
 #define HTACCESS_CGI            "/usr/cgi-bin/.htaccess"
 #define SYSCONFIG_HTACCESS_DOCROOT        "/etc/SysConfig/htaccess_htdocs"
 #define SYSCONFIG_HTACCESS_CGI            "/etc/SysConfig/htaccess_cgi"
 #define PASSWORD_FILE           "/etc/SysConfig/passwords"
 #define PASSWORD_FILE_ADMIN     "/etc/SysConfig/passwords_admin"
 #define PASSWORD_FILE_USER     "/etc/SysConfig/passwords_user"
 #define HTPASSWD_BINARY        "/usr/bin/htpasswd"
 #define SET_CMD_LOCKFILE       "/usr/cgi-bin/set_cmd_lockfile"

#else
 // For  x86 test build 
 #define HTACCESS_DOCROOT        "/var/www/html/.htaccess"
 #define HTACCESS_CGI            "/var/www/cgi-bin/.htaccess"
 #define SYSCONFIG_HTACCESS_DOCROOT        "/tmp/htaccess_html"
 #define SYSCONFIG_HTACCESS_CGI            "/tmp/htaccess_cgi"
 #define PASSWORD_FILE           "/var/www/ip_cam_passwd/passwords"
 #define PASSWORD_FILE_ADMIN     "/var/www/ip_cam_passwd/passwords_admin"
 #define PASSWORD_FILE_USER     "/var/www/ip_cam_passwd/passwords_user"
 #define HTPASSWD_BINARY        "/usr/bin/htpasswd"
 #define SET_CMD_LOCKFILE       "/var/www/cgi-bin/set_cmd_lockfile"

#endif

#define HTDIGEST_PASSWORD_LENGTH	9

#define FILE_UPLOAD_BOUNDARY_LINE_LENGTH 29

#define QQVGA                           0
#define QCIF                            1
#define QVGA                            2
#define CIF                             3
#define VGA                             4
#define SVGA                            5
#define XGA                             6
#define SXGA                            7
#define UXGA                            8
#define QXGA                            9
//#define QSXGA                           10
//#define QUXGA                           11
#define D1_PAL                          16
#define D1_NTSC                         17

#define ISO_100                         0
#define ISO_200                         1
#define ISO_400                         2

#define VIDEO_FORMAT_PAL                1
#define VIDEO_FORMAT_NTSC               2

#define IMAGE_TYPE_MPEG4                0
#define IMAGE_TYPE_H264                 2
#define IMAGE_TYPE_MJPEG                3

#define IP_ADDRESS                      1
#define NETMASK                         2
#define NAMESERVER                      3
#define GATEWAY                         4

#define STREAM_IP_TYPE_UNICAST          0
#define STREAM_IP_TYPE_MULTICAST        1

#define MIRRORING_NORMAL		0
#define MIRRORING_HORZ			1
#define MIRRORING_VERT			2
#define MIRRORING_HORZ_VERT		3

#define MOTDET_IN_LOW_SENSE    		0
#define MOTDET_IN_HIG_SENSE    		2
#define MOTDET_IN_MED_SENSE    		1   
#define MOTDET_IN_VLOW_SENSE   		3

#define MOTDET_OUT_VLOW_SENSE   	4
#define MOTDET_OUT_LOW_SENSE    	5
#define MOTDET_OUT_MED_SENSE    	6
#define MOTDET_OUT_HIG_SENSE    	7

#define MOTION_SENS_LOW			50
#define MOTION_SENS_OPTIMUM		150
#define MOTION_SENS_HIGH		250

#define DC_OUT_OFF			0
#define DC_OUT_ON			1
#define DC_OUT_ALARM			2

#define ZONE_0				0x0001
#define ZONE_1				0x0002
#define ZONE_2				0x0004
#define ZONE_3				0x0008
#define ZONE_4				0x0010
#define ZONE_5				0x0020
#define ZONE_6				0x0040
#define ZONE_7				0x0080
#define ZONE_8				0x0100
#define ZONE_9				0x0200
#define ZONE_10				0x0400
#define ZONE_11				0x0800
#define ZONE_12				0x1000
#define ZONE_13				0x2000
#define ZONE_14				0x4000
#define ZONE_15				0x8000

#define NO_COLOR			0
#define COLOR_MONOCHROME                1
#define COLOR_RED                       2
#define COLOR_BLUE                      3
#define COLOR_GREEN                     4
#define COLOR_NEGATIVE                  5
#define COLOR_MONOCHROME_NEGATIVE       6
#define COLOR_SEPIA                     7
