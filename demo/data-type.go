


type MyInt = int   // MyInt 完全等同于 int



type UserID int64   // UserID 与 int64 是不同类型，需要显式转换
type Handler func(http.ResponseWriter, *http.Request) // 函数类型
