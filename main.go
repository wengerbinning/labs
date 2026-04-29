package main

import (
	"net/http"
)



type DevelHandler struct{}
func (h DevelHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("Hello, World!"))
}





func main() {
	mux := http.NewServeMux()

	mux.Handle("/", DevelHandler{})

	http.ListenAndServe(":8080", mux)
}
