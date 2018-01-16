package main

import (
	"net"
	"fmt"
	"encoding/json"
	"database/sql"
	_ "github.com/go-sql-driver/mysql"
	"log"
	"os"
)

type MessageBody struct {
	Type string `json:"Type"`
	From string	`json:"From"`
	To string	`json:"To"`
	Content string	`json:"Content"`

}

type ResponseMessage struct {
	Type string `json:"Type"`
	Content string `json:"Content"`
	From string    `json:"From"`
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

//登录验证
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
				responseMessages := ResponseMessage{}
				responseMessages.Type = "201"   //登录成功
				responseMessages.Content = ""
				jsonStr, _ := json.Marshal(&responseMessages)
				conn.Write([]byte(jsonStr))
				g_clientConns[loginMessages[0].UserName] = conn
				clientName = loginMessages[0].UserName
				break
			} else {
				responseMessages := ResponseMessage{}
				responseMessages.Type = "404"   //密码不正确，登录失败
				responseMessages.Content = ""
				jsonStr, _ := json.Marshal(&responseMessages)
				conn.Write([]byte(jsonStr))
			}
		} else {
			responseMessages := ResponseMessage{}
			responseMessages.Type = "404"   //不存在该用户，登录失败
			responseMessages.Content = ""
			jsonStr, _ := json.Marshal(&responseMessages)
			conn.Write([]byte(jsonStr))
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
		//解析用户json
		messageFromClient := []MessageBody{}
		json.Unmarshal(buffer[0:n], &messageFromClient)
		fmt.Println(string(buffer[0:n]))
		fmt.Println("Json from client", messageFromClient)
		switch messageFromClient[0].Type {
		case "SendMessage":
			log.Println(clientName + " SendMessage")
			if g_clientConns[messageFromClient[0].To] != nil {    //如果用户在线
				responseMessages := ResponseMessage{}
				responseMessages.Type = "203"   //发送消息
				responseMessages.Content = clientName + ": " + messageFromClient[0].Content
				responseMessages.From = clientName
				jsonStr, _ := json.Marshal(&responseMessages)
				g_clientConns[messageFromClient[0].To].Write([]byte(jsonStr))
			} else {
				responseMessages := ResponseMessage{}
				responseMessages.Type = "203"   //发送消息
				responseMessages.Content = "系统： 该用户暂时不在线"
				responseMessages.From = messageFromClient[0].To
				jsonStr, _ := json.Marshal(&responseMessages)
				g_clientConns[clientName].Write([]byte(jsonStr))
				//将离线消息存储到数据库
				stmt, _ := g_db.Prepare(`insert into offline_message(from_, to_, content) values(?, ?, ?)`)
				stmt.Exec(clientName, messageFromClient[0].To, clientName + ": " + messageFromClient[0].Content)
				stmt.Close()
			}

		case "GetChatRecord":   //获取聊天记录
			log.Println(clientName + " GetChatRecord")
			var chatRecord string
			rows, _ := g_db.Query("select content_ from chatrecord where from_ = ? and to_ = ?", messageFromClient[0].From, messageFromClient[0].To)
			if rows.Next() {
				rows.Scan(&chatRecord)
			}
			responseMessages := ResponseMessage{}
			responseMessages.Type = "202"  //聊天记录
			responseMessages.Content = chatRecord
			responseMessages.From = messageFromClient[0].To
			jsonStr, _ := json.Marshal(&responseMessages)
			g_clientConns[clientName].Write([]byte(jsonStr))

		case "GetOfflineMessage":   //拉取离线消息
			//从数据库中获取对方发送给client的消息
			log.Println(clientName + " :GetOfflineMessage")
			rows, _ := g_db.Query("select content from offline_message where from_ = ? and to_ = ?",  messageFromClient[0].To, clientName)
			var content string
			for rows.Next() {
				rows.Scan(&content)
				responseMessages := ResponseMessage{}
				responseMessages.Type = "203"  //聊天记录
				responseMessages.Content = content
				responseMessages.From = messageFromClient[0].To
				jsonStr, _ := json.Marshal(&responseMessages)
				g_clientConns[clientName].Write([]byte(jsonStr))
			}
			//删除已拉取的消息
			stmt, _ := g_db.Prepare("DELETE FROM offline_message WHERE from_ = ? and to_ = ?")
			stmt.Exec(messageFromClient[0].To, messageFromClient[0].From)

		case "SendFile":
			log.Println(clientName + " :SendFile")
			//发送的文件先保存在服务端
			 fileBuffer := make([]byte, 2048)
			 conn.Read(fileBuffer)
			 fileName := string(fileBuffer)
			 file, err := os.Create(fileName)
			 defer file.Close()
			 if err != nil {
			 	for {
					n, _ = conn.Read(fileBuffer)
					if n == 0 {
						break
					}
					file.Read(fileBuffer)
				}
			 }
			 
		case "GetFile":
			//用户从服务端拉取文件
		}


	}
}

func handleListenConn()  {

}
