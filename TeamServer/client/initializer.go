package client

import (
	"TeamServer/debug"
	"TeamServer/macro"
	"TeamServer/support"
	"bytes"
	"encoding/json"
	"log"

	"github.com/gorilla/websocket"
)

func SendClientAllTargetInfo(Client *macro.ClientInfo) {

	for _, TargetInfo := range macro.TargetMap {
		SendClientNewTarget(&TargetInfo.TargetData, macro.Single, Client)

	}
	for _, Listener := range macro.ListenerMap {
		SendClientListenerInfo(Listener, macro.Single, Client)

	}
	SendClientC2Configration(macro.Single, Client)
	return
}

func SendAll(Client *macro.ClientInfo) {
	SendClientAllTargetInfo(Client)
}

func DeleteClient(Client *macro.ClientInfo) {
	debug.Info("Client [%s] Logout", Client.UserName)
	Client.Conn.Close()
	delete(macro.ClientMap, Client.SessionID)

}

func AddNewClient(SessionID string, UserName string, Conn *websocket.Conn) *macro.ClientInfo {
	var NewClient *macro.ClientInfo = new(macro.ClientInfo)
	NewClient.Conn = Conn
	NewClient.UserName = UserName
	NewClient.SessionID = SessionID
	macro.ClientMap[SessionID] = NewClient
	return NewClient
}

func Login(Conn *websocket.Conn) {
	var buffer bytes.Buffer
	var pk *Packager = NewPackager()
	var Package Package

	//response := make(map[string]map[string]any)
	_, JSONData, err := Conn.ReadMessage()
	if err != nil {
		debug.Error("read:", err)
		Conn.Close()
		return
	}
	Pack := pk.CreatePackage(JSONData)
	log.Println(Pack.Head.Event)
	if Pack.Head.Event == LoginCT {
		UserPassword := Pack.Body.Info["Password"]
		Package.Head.Event = LoginTC
		Package.Head.Time = support.GetTime()
		Package.Body.SubEvent = LoginTC
		Package.Body.Info = make(map[string]interface{})

		if err != nil {
			debug.Error("json.NewEncoder:", err)
			return
		}
		if UserPassword == macro.ServicePass {
			SessionID := support.GenerateRandomID()
			Package.Body.Info["SessionID"] = SessionID
			NewClient := AddNewClient(SessionID, Pack.Head.User, Conn)
			defer HandleClient(NewClient, Pack)
			debug.Good("User %s Login Successfuly", NewClient.UserName)
		} else {
			Package.Body.Info["SessionID"] = ""
			defer Conn.Close()
			debug.Warn("Login %s failed")

		}
		err = json.NewEncoder(&buffer).Encode(Package)
		if err != nil {
			debug.Error("encode error:", err)
			return
		}
		err = Conn.WriteMessage(websocket.BinaryMessage, buffer.Bytes())
		if err != nil {
			debug.Error("write:", err)
			return
		}
	} else {
		debug.Error("Registration Code Error")
	}

	return

}
