


SSL_library_init
SSL_load_error_strings
SSL_CTX_new
SSL_CTX_free
SSL_CTX_set_timeout
SSL_set_mode



ERR_print_errors_fp




BIO_new_ssl_connect
BIO_get_ssl
BIO_get_fd
BIO_set_conn_hostname
BIO_set_nbio
BIO_do_connect
BIO_do_handshake
BIO_write
BIO_read
BIO_free_all



BIO_ADDRINFO
BIO_socket
BIO_connect
BIO_ADDRINFO_free



openlog
syslog
closelog


* 连接超时
* 写超时(10次重传, 缓存100K数据)
* 读超时()



fastcgi_request_buffering off; 处理最小数据单位由接收buffer决定.



* 子路由返回502的结果
* 子路有没有返回结果

