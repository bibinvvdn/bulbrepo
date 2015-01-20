//void print_option_on_off(uchar opt_name);

void print_html_msg(char msg_str[]);

void html_header(void);

void html_tail(void);

void html_line_br(void);

char* get_post_request(void);

long gen_msg_id(void);

int print_file(char fname[]);

//int decode_post_data(char *data, char *param_name);

uchar* decode_post_data(char *data, char *param_name);

int htaccess_cgi_admin(void);

int htaccess_cgi_admin_user(void);

int htaccess_html_docroot(void);

int htaccess_cgi_admin_basic(void);

int htaccess_cgi_admin_user_basic(void);

int htaccess_html_docroot_basic(void);

char* decode_file_upload_text(char *upload_data, char *file_name);

int decode_file_upload(char *uploaded_files, char *file_name, char *file_to_save);

//int gen_md5_password_key(uchar *name, uchar *realm, uchar *password, uchar *md5_key);

//int write_md5_password_key_to_file(uchar *name, uchar *realm, uchar *md5_key, char *file_path);

int copy_file(char *source_file_path, char *dest_file_path);

int append_to_file(char *source_file_path, char *dest_file_path);

int write_blank_file(char *file_path);

int write_to_file(char *mem_block, int size, char *file_path);

int check_set_command_status();

int set_command_status(int cmd_status);

void html_msg_page_header();

void html_msg_page_reboot_header();

void html_msg_page_tailer();

void command_locking_msg(char* fwd_req, char* page_name);

void error_msg(char* fwd_req, char* page_name);
