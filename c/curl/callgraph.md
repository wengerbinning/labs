#### global_init

global_init -> Curl_trc_init
global_init -> Curl_ssl_init
global_init -> Curl_win32_init
global_init -> Curl_amiga_init
global_init -> Curl_macos_init
global_init -> Curl_resolver_global_init
global_init -> Curl_ssh_init

#### curl_global_init

```c
CURLcode curl_global_init (long flags);
```

curl_global_init -> curl_global_init
curl_global_init -> global_init
curl_global_init -> global_init_unlock


#### curl_global_cleanup

```c
void curl_global_cleanup (void);
```

curl_global_cleanup -> global_init_lock
curl_global_cleanup -> Curl_ssl_cleanup
curl_global_cleanup -> Curl_resolver_global_cleanup
curl_global_cleanup -> Curl_win32_cleanup
curl_global_cleanup -> Curl_amiga_cleanup
curl_global_cleanup -> Curl_ssh_cleanup
curl_global_cleanup -> global_init_unlock


#### curl_easy_perform

```c
CURLcode curl_easy_perform (CURL *curl);
```

curl_slist_append

curl_easy_setopt

curl_easy_perform

curl_easy_cleanup


curl_easy_init
curl_easy_setopt
curl_easy_perform
curl_easy_cleanup
