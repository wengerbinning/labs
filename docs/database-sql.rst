

* MySQL, MariaDB, SQLLite
* 连接管理
* 安全并发

db, err = sql.Open()
db.Ping()
db.Close()



db.SetMaxOpenConns(25)
db.SetMaxIdleConns(5)
db.SetConnMaxLifetime(5 * time.Minute)

db.Exec
db.Query
db.QueryRow
row.Scan
