package client

import (
	"TeamServer/debug"
	"TeamServer/macro"
	"log"
	"net/http"

	"github.com/gorilla/websocket"
)

var upgrader = websocket.Upgrader{}

func SendMessge(Client *macro.ClientInfo, Buffer []byte) error {
	Client.Mutex.Lock()
	err := Client.Conn.WriteMessage(websocket.BinaryMessage, Buffer)
	if err != nil {
		debug.Error("Sending data to :", Client.SessionID)

	}
	Client.Mutex.Unlock()
	return err
}

func HandleClient(Client *macro.ClientInfo, Pack Package) error {
	SendAll(Client)
	Conn := Client.Conn
	for true {
		var Pack Package
		var err error
		err, Pack = ReadClient(Conn)
		if err != nil {
			debug.Error("read:", err)
			DeleteClient(Client)
			return err
		}
		ds := NewClientDispatch(Pack, Client)
		ds.ClientDispatch()
	}
	return nil
}

func ReadClient(Conn *websocket.Conn) (error, Package) {
	var Pack Package
	var pk *Packager = NewPackager()
	_, JSONData, err := Conn.ReadMessage()
	if err != nil {
		debug.Error("read:", err)
		Conn.Close()
		return err, Pack
	}
	Pack = pk.CreatePackage(JSONData)
	return nil, Pack

}

func HandleConn(w http.ResponseWriter, r *http.Request) {
	Conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		debug.Error("upgrade:", err)
		return
	}
	Login(Conn)

}

func StartClient(ip string, port string) {

	Listener := ip + ":" + port
	log.SetFlags(0)
	http.HandleFunc("/AtherAPI", HandleConn)
	debug.Good("Client Listener " + Listener + " Runing!")
	go RunUpdateListenerHits()
	log.Fatal(http.ListenAndServe(Listener, nil))

}
