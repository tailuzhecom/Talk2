package main

import (
	"net"
	"fmt"
	"encoding/json"
	"database/sql"
	_ "github.com/go-sql-driver/mysql"
)

type MessageBody struct {
	From string	`json:"From"`
	To string	`json:"To"`
	Content string	`json:"Content"`

}

type LoginMessage struct {
	UserName string `json:"UserName""`
	Passwd string `json:"Passwd"`
}


var g_clientConns = make(map[string]net.Conn)
var g_db *sql.DB

func main()  {
	var err error
	g_db, err = sql.Open("mysql", "root:134679asd@/chat")
	if err != nil {
		fmt.Println("Open db error")
		return
	}
	rows, err := g_db.Query("select * from login where username = ?", "kd")
	var username_db, passw_db string
	for rows.Next() {
		rows.Scan(&username_db, &passw_db)
		fmt.Println(username_db, passw_db)
	}
	listenServer, err := net.Listen("tcp", "127.0.0.1:9999")
	if err != nil {
		fmt.Println("Listen error")
		return
	}
	fmt.Println("ChatServer start...")
	for {
		conn, err := listenServer.Accept()
		if err != nil {
			fmt.Println(err)
			continue
		}
		go handleLoginConn(conn)
	}
}

func handleLoginConn(conn net.Conn)  {
	buffer := make([]byte, 2048)
	var clientName string
	for {
		n, err := conn.Read(buffer)
		if n == 0 || err != nil {
			fmt.Println("Client quit")
			return
		}
		loginMessages := []LoginMessage{}
		json.Unmarshal(buffer[0:n], &loginMessages)
		fmt.Println("loginMessages", loginMessages)
		rows, err := g_db.Query("select * from login where username = ?", loginMessages[0].UserName)
		var username_db, passw_db string
		if rows.Next() {
			rows.Scan(&username_db, &passw_db)
			if username_db == loginMessages[0].UserName && passw_db == loginMessages[0].Passwd {
				conn.Write([]byte("201"))
				g_clientConns[loginMessages[0].UserName] = conn
				clientName = loginMessages[0].UserName
				break
			} else {
				conn.Write([]byte("404\0"))
			}
		} else {
			conn.Write([]byte("404\0"))
		}
	}
	handleReadConn(conn, clientName)
}

func handleReadConn(conn net.Conn, clientName string)  {
	buffer := make([]byte, 2048)
	for {
		n, err := conn.Read(buffer)
		if n == 0 || err != nil {
			fmt.Println("Client quit")
			delete(g_clientConns, clientName)
			return
		}
		messageFromClient := []MessageBody{}
		json.Unmarshal(buffer[0:n], &messageFromClient)
		fmt.Println(string(buffer[0:n]))
		fmt.Println("Json from client", messageFromClient)
		if g_clientConns[messageFromClient[0].To] != nil {
			g_clientConns[messageFromClient[0].To].Write([]byte(messageFromClient[0].From+ ": " +
							messageFromClient[0].Content))
		} else {
			fmt.Println(messageFromClient[0].To, "该用户不在线")
		}
	}
}

func handleListenConn()  {

}
