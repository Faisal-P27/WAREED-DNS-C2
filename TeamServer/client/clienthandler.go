package client

import (
	"TeamServer/database"
	"TeamServer/debug"
	"TeamServer/macro"
	"bytes"
	"encoding/json"

	"strconv"
	"time"
)

func SingleNewMessge(Client *macro.ClientInfo, Package Package) {
	var buffer bytes.Buffer
	err := json.NewEncoder(&buffer).Encode(Package)

	if err != nil {
		debug.Error("write:", err)
		return
	}

	err = SendMessge(Client, buffer.Bytes())
	if err != nil {
		debug.Error("Error BrodcastNewMessge data to :", Client.SessionID)

	}

}

func BrodcastNewMessge(Package Package) {
	var buffer bytes.Buffer
	err := json.NewEncoder(&buffer).Encode(Package)

	if err != nil {
		debug.Error("write:", err)
		return
	}

	for _, Client := range macro.ClientMap {
		err = SendMessge(Client, buffer.Bytes())
		if err != nil {
			debug.Error("Error BrodcastNewMessge data to :", Client.SessionID)

		}
	}
}
func SendClientListenerInfo(NewListener *database.ListenerInfo, SendType int, Client *macro.ClientInfo) {
	Package := InitializePackge(NewListenerTC, NewListenerTC)
	Package.Body.Info["DomainName"] = NewListener.DomainName
	Package.Body.Info["Protocol"] = NewListener.Protocol
	Package.Body.Info["BindIP"] = NewListener.BindIP
	Package.Body.Info["BindPort"] = NewListener.BindPort
	Package.Body.Info["Hits"] = strconv.Itoa(NewListener.Hits)
	Package.Body.Info["Status"] = NewListener.Status
	if SendType == macro.Brodcast {
		BrodcastNewMessge(Package)
	} else if SendType == macro.Single {
		SingleNewMessge(Client, Package)
	}

}

func SendClientDelListenerInfo(NewListener *database.ListenerInfo, SendType int, Client *macro.ClientInfo) {
	Package := InitializePackge(DelListenerTC, DelListenerTC)
	Package.Body.Info["DomainName"] = NewListener.DomainName
	Package.Body.Info["Protocol"] = NewListener.Protocol
	Package.Body.Info["BindIP"] = NewListener.BindIP
	Package.Body.Info["BindPort"] = NewListener.BindPort
	Package.Body.Info["Hits"] = strconv.Itoa(NewListener.Hits)
	Package.Body.Info["Status"] = NewListener.Status
	if SendType == macro.Brodcast {
		BrodcastNewMessge(Package)
	} else if SendType == macro.Single {
		SingleNewMessge(Client, Package)
	}

}

func SendClientC2Configration(SendType int, Client *macro.ClientInfo) {
	Package := InitializePackge(C2CongigTC, C2CongigTC)
	Package.Body.Info["MagicValue"] = macro.Config.MagicValue
	Package.Body.Info["Domains"] = macro.Config.Domains
	Package.Body.Info["DefaultDNSServer"] = macro.Config.DefaultDNSServer
	Package.Body.Info["ListenerPort"] = macro.Config.ListenerPort
	Package.Body.Info["Sleep"] = macro.Config.Sleep
	Package.Body.Info["Jitter"] = macro.Config.Jitter
	Package.Body.Info["LookupDelay"] = macro.Config.LookupDelay
	if SendType == macro.Brodcast {
		BrodcastNewMessge(Package)
	} else if SendType == macro.Single {
		SingleNewMessge(Client, Package)
	}

}

func SendClientNewTarget(NewTargert *database.TargetSession, SendType int, Client *macro.ClientInfo) {

	Package := InitializePackge(NewTargetSessionTC, NewTargetSessionTC)
	sessionJSON, err := json.Marshal(NewTargert)
	if err != nil {
		debug.Error("Error marshalling TargetSession: %v", err)
		return
	}
	var sessionMap map[string]interface{}
	err = json.Unmarshal(sessionJSON, &sessionMap)
	if err != nil {
		debug.Error("Error marshalling TargetSession: %v", err)
		return
	}
	Package.Body.Info = sessionMap
	if SendType == macro.Brodcast {
		BrodcastNewMessge(Package)
	} else if SendType == macro.Single {
		SingleNewMessge(Client, Package)
	}

}

func SendPrintConsole(Target database.TargetSession, Data string, Type string) {

	Package := InitializePackge(PrintConsoleTC, PrintConsoleTC)
	Package.Body.Info["SessionID"] = Target.SessionID
	Package.Body.Info["ConsoleData"] = Data
	Package.Body.Info["Type"] = Type
	BrodcastNewMessge(Package)

}

func SendUpdateLastSeen(Target database.TargetSession) {

	Package := InitializePackge(UpdateLastSeenTC, UpdateLastSeenTC)
	Package.Body.Info["SessionID"] = Target.SessionID
	Package.Body.Info["LastSeen"] = Target.LastPing
	BrodcastNewMessge(Package)

}
func SendNewTaskSending(Target database.TargetSession, PacketSize int, TaskID string) {

	Console := "Sending Task ID [" + TaskID + "]" + " Task Size [" + strconv.Itoa(PacketSize) + "] Message"
	SendPrintConsole(Target, Console, macro.PrintConsoleSendTask)

}

func SendConsoleWarning(Target database.TargetSession, Data string) {

	SendPrintConsole(Target, Data, macro.PrintConsoleWarning)

}
func SendConsoleSuccessful(Target database.TargetSession, Data string) {

	SendPrintConsole(Target, Data, macro.PrintConsoleSuccessful)

}

func SendUpdateListenerHits() {

	for _, Listener := range macro.ListenerMap {
		Package := InitializePackge(UpdateListenerHitsTC, UpdateListenerHitsTC)
		Package.Body.Info["DomainName"] = Listener.DomainName
		Package.Body.Info["Hits"] = strconv.Itoa(Listener.Hits)
		BrodcastNewMessge(Package)
	}

}

func SnedNewMessageBox(Client *macro.ClientInfo, Title string, Text string, TextInfo string, OK bool) {
	Package := InitializePackge(MessageBoxTC, MessageBoxTC)
	Package.Body.Info["Title"] = Title
	Package.Body.Info["TextInfo"] = TextInfo
	Package.Body.Info["Text"] = Text
	if OK {
		Package.Body.Info["Type"] = "Good"
	} else {
		Package.Body.Info["Type"] = "Error"
	}
	go SingleNewMessge(Client, Package)
}

func RunUpdateListenerHits() {

	for {

		SendUpdateListenerHits()
		time.Sleep(10 * time.Second)

	}

}

func SendNewPayload(Client *macro.ClientInfo, payload string, ptype string, os string, path string, name string, err string) {

	Package := InitializePackge(NewPayloadTC, NewPayloadTC)
	Package.Body.Info["Payload"] = payload
	Package.Body.Info["Type"] = ptype
	Package.Body.Info["OS"] = os
	Package.Body.Info["Path"] = path
	Package.Body.Info["Name"] = name
	Package.Body.Info["Error"] = err
	go SingleNewMessge(Client, Package)

}
