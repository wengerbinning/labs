
go version go1.22.0 linux/amd64
go install github.com/go-sql-driver/mysql@latest




go mod init
go get -u github.com/go-sql-driver/mysql





* 服务/连接///


数据库(database)
数据表(datatable)
数据项(dataentry)


DBConfig




Server
session

DbCtx
DbOps
DtCtx
DtOps
DeCtx
DeOps


type DeCtx struct {

}
