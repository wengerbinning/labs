
package libs

import (
	"fmt"
	"errors"
	"database/sql"
)

type User struct {
	Uidx     int
	Name  string
	Type     int
	Attr  string
}

func Search (srv DbServer, usr string) ([]User, error) {
	var users []User

	if ( srv.Context == nil) {
		return nil, errors.New("invalid user id: must be positive")
	}

	db := *srv.Context.(**sql.DB)

	fmt.Println("RUNNING IN HERE")

	// res, err := db.Query("SHOW TABLES")
	res, err := db.Query("SELECT id,name,type FROM users")
	if (err != nil) {
		return nil, err
	}
	defer res.Close()

	for res.Next() {
		var u User
		err := res.Scan(&u.Uidx, &u.Name, &u.Type);
		if (err != nil) {
			break;
		}
		users = append(users, u)
	}

	return users, nil
}


func Find (srv DbServer, usr string) int {
	var users []User

	if (srv.Context== nil) {
		return -1
	}

	db  := *srv.Context.(**sql.DB)
	cmd := "SELECT id,name,type FROM users WHERE name = ?"
	res, err := db.Query(cmd, usr)
	if (err != nil) {
		fmt.Println(err)
		return -1
	}
	defer res.Close()

	for res.Next() {
		var u User
		err := res.Scan(&u.Uidx, &u.Name, &u.Type);
		if (err != nil) {
			fmt.Println(err)
			break;
		}
		users = append(users, u)
	}

	for _, u := range users {
		fmt.Printf("ID: %d, Name: %s, Email: %d\n", u.Uidx, u.Name, u.Type)
	}

	return 0
}
