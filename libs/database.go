package libs

import (
	"fmt"
	"database/sql"
	_ "github.com/go-sql-driver/mysql"
)

type SrvOps interface {
	Init(c DbConfig, d string) error
	Exit() int
}

type DbsOps interface {
	Create(name string) error
}

type DtsOps interface {
	Create (name string) error
	Search (t string, q string, argv ...interface{})
}

type DesOps interface {
	Add (e string)
}

type DbConfig struct {
	Host     string
	Port       uint
	Username string
	Password string
	Database string
}

type DbServer struct {
	Engine int
	Context interface{}
}

func (srv *DbServer) Init(c DbConfig, d string) error {
	dsn := fmt.Sprintf("%s:%s@tcp(%s:%v)/%s",
		c.Username, c.Password, c.Host, c.Port, d)
	ctx, err := sql.Open("mysql", dsn)
	if (err != nil) {
		return err
	}
	err = ctx.Ping()
	if (err != nil) {
		return err
	}

	fmt.Printf("Connect %s\n", dsn)
	srv.Context = &ctx
	return nil
}

func (srv *DbServer) Exit() int {
	if (srv == nil && srv.Context == nil) {
		return -1
	}

	ctx := *srv.Context.(**sql.DB)
	if (ctx != nil) {
		ctx.Close()
	}

	return 0
}



func (srv *DbServer) Search (q string, arg ...interface{}) {
	if (srv.Context == nil) {
		return -1
	}

	db := *srv.Context.(**sql.DB)

	res, err := db.Query(q, arg...)
	if (err != nil) {
		return nil, err
	}
	defer res.Close()

	for res.Next() {

	}

}
