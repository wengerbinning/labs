package main

import (
	"fmt"
	"labs/libs"
)


var AdminConf libs.DbConfig = {
    Host: "127.0.0.1",
    Port: 3306,
    Username: "root",
    Password: "root",
    Database: "wenger",
}

var userConf libs.DbConfig = {
    Host: "127.0.0.1",
    Port: 3306,
    Username: "root",
    Password: "root",
    Database: "wenger",
}

func main() {

	var ctx libs.DbServer

	conf := libs.DbConfig {
		Host: "127.0.0.1",
		Port: 3306,
		Username: "root",
		Password: "root",
		Database: "wenger",
	}

	err := ctx.Init(conf, "wenger")
	if (nil != err) {
		fmt.Println(err)
	}
	defer ctx.Exit()


	fmt.Println("Database Connect Finished ...")

	libs.Find(srv, "wenger")




	// // 5. 检查数据库
	// var exists bool
	// query := "SELECT EXISTS(SELECT 1 FROM information_schema.schemata WHERE schema_name = ?)"
	// err = srv.QueryRow(query, database).Scan(&exists)
	// if err != nil {
	// 	fmt.Println("Not Found database")
	// }





	// 5. 执行数据库操作（CRUD示例）

    // --- 插入示例 ---
    // result, err := db.Exec("INSERT INTO users (username, email) VALUES (?, ?)", "alice", "alice@example.com")
    // if err != nil {
    //     log.Fatal("Insert failed: ", err)
    // }
    // lastId, _ := result.LastInsertId()
    // fmt.Printf("Inserted new user, ID: %d\n", lastId)

    // --- 查询单条记录示例 ---
    // var user User
    // row := db.QueryRow("SELECT id, username, email, created_at FROM users WHERE id = ?", 1)
    // if err = row.Scan(&user.ID, &user.Username, &user.Email, &user.CreatedAt); err != nil {
    //     if err == sql.ErrNoRows {
    //         log.Println("No user found with ID 1")
    //     } else {
    //         log.Fatal("Query failed: ", err)
    //     }
    // } else {
    //     fmt.Printf("Found user: %+v\n", user)
    // }

    // --- 查询多条记录示例 ---
    // rows, err := db.Query("SELECT id, username, email, created_at FROM users")
    // if err != nil {
    //     log.Fatal("Query failed: ", err)
    // }
    // defer rows.Close()

    // var users []User
    // for rows.Next() {
    //     var u User
    //     if err := rows.Scan(&u.ID, &u.Username, &u.Email, &u.CreatedAt); err != nil {
    //         log.Fatal("Scan failed: ", err)
    //     }
    //     users = append(users, u)
    // }
    // if err = rows.Err(); err != nil {
    //     log.Fatal("Rows iteration error: ", err)
    // }
    // fmt.Printf("All users: %+v\n", users)
}
