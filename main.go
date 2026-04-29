package main

import (
	"fmt"
	"net/http"
	"encoding/json"
)

type WebRes struct {
	HandleCommand string
	HandleCode int
	HandleMessage string
}

type DevelHandler struct{}
func (h DevelHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(map[string]string{"message": "Hello, World!"})
}






type LoginHandler struct{
	res WebRes
}
func (h LoginHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")

	// // 请求头 - Header
	// userAgent := r.Header.Get("User-Agent")

	// // 参数：/path?name=john
	// name := r.URL.Query().Get("name")

	// 表单 - Form
	r.ParseForm()
	username := r.FormValue("username")
	password := r.FormValue("password")

	resVal := 0
	fmt.Printf("%s:%s\n", username, password)
	if (username != "" && password != "") {
		resVal =  0
	} else {
		resVal = -1
	}

	// // 请求体 - Body
	// var data map[string]interface{}
	// json.NewDecoder(r.Body).Decode(&data)
	// defer r.Body.Close()


	h.res.HandleCode = resVal
	h.res.HandleMessage = "Nothing"
	json.NewEncoder(w).Encode(h.res)
}


func devel(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		fmt.Printf("%s %s\n", r.Method, r.URL.Path)
		next.ServeHTTP(w, r)
	})
}

func main() {
	mux := http.NewServeMux()

	mux.Handle("/", DevelHandler{})
	mux.Handle("/login", LoginHandler{})

	http.ListenAndServe(":8080", devel(mux))
}
