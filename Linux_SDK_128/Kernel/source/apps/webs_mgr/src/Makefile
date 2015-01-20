BASE_DIR = $(PWD)
SRC_DIRS = .
CGI_DIR	 = ../cgi-bin

CFLAGS  += -I../../../common -I../inc 
LDFLAGS += #-L../ fix me

all: CGI
	$(CC) $(CFLAGS) $(LDFLAGS) -s uploadfile.c -o $(CGI_DIR)/uploadfile.cgi -lpthread -lconn
	$(CC) $(CFLAGS) $(LDFLAGS) -s image_upgrade.c -o $(CGI_DIR)/image_upgrade.cgi -lpthread -lconn
	$(CC) $(CFLAGS) $(LDFLAGS) -s get_scan.c -o $(CGI_DIR)/Web_Get_Scan_Cfg.cgi -lpthread -lconn 
	$(CC) $(CFLAGS) $(LDFLAGS) -s set_net.c -o $(CGI_DIR)/Web_Set_Nw_Cfg.cgi -lpthread -lconn
	$(CC) $(CFLAGS) $(LDFLAGS) -s get_net.c -o $(CGI_DIR)/Web_Get_Nw_Cfg.cgi -lpthread -lconn
	$(CC) $(CFLAGS) $(LDFLAGS) -s set_on_off.c -o $(CGI_DIR)/Web_Set_Bulb_Cfg.cgi -lpthread -lconn
	$(CC) $(CFLAGS) $(LDFLAGS) -s set_blink.c -o $(CGI_DIR)/Web_Set_Blink_Cfg.cgi -lpthread -lconn
	$(CC) $(CFLAGS) $(LDFLAGS) -s get_power.c -o $(CGI_DIR)/Web_Get_Power_Cfg.cgi -lpthread -lconn
	$(CC) $(CFLAGS) $(LDFLAGS) -s set_wlan.c -o $(CGI_DIR)/Web_Set_Wlan_Cfg.cgi -lpthread -lconn
	$(CC) $(CFLAGS) $(LDFLAGS) -s get_wlan.c -o $(CGI_DIR)/Web_Get_Wlan_Cfg.cgi -lpthread -lconn
	$(CC) $(CFLAGS) $(LDFLAGS) -s get_poll.c -o $(CGI_DIR)/Web_Get_Poll_Cfg.cgi -lpthread -lconn
	$(CC) $(CFLAGS) $(LDFLAGS) -s get_black.c -o $(CGI_DIR)/Web_Get_Black_Cfg.cgi -lpthread -lconn
    	
	$(CC) $(CFLAGS) $(LDFLAGS) -s set_mgt.c -o $(CGI_DIR)/Web_Set_Mgt_Cfg.cgi -lpthread -lconn
CGI:
	mkdir -p $(CGI_DIR)

clean:
	rm -rf $(CGI_DIR)/*
