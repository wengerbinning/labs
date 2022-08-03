网络处理

* 基础
* 路由
* 请求
* 响应
* 服务端：处理请求数据并返回响应
* 中间件：在服务端处理之前， 进行鉴权、认证、日志、限流、安检等行为
* 客户端：请求服务资源



基础核心

*  http.Handler

服务端

* http.Server
* http.ListenAndServe
* http.ListenAndServeTLS
* srv.Shutdown(ctx)

客户端

* http.Get
* http.NewRequest
* http.Client

中间件




路由

* http.HandleFunc
* http.NewServeMux
* mux.HandleFunc
* mux.Handle

请求

* r.URL.Query().Get("name")
* r.ParseForm()
* r.FormValue("email")
* r.Header.Get("User-Agent")
* r.Body


响应


* http.ListenAndServe



http.HandleFunc - 将路由与处理函数注册到默认ServeMux
http.Handle -  将实现http.Handler接口的数据注册到默认ServeMux

func HandleFunc (pattern string, handler func(ResponseWriter, *Request))
func HandleFunc (pattern string, handler http.Handler)





/api/devel/


login
logout
register
setting
