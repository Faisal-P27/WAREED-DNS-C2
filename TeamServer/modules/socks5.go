package modules

import (
	"TeamServer/debug"
	"net"
)

const defaultBufSize = 65536

var totalBytesIn = 0
var totalBytesOut = 0
var connectedClients = 0
var ProxyStatus = false
var GlobalAgentListener net.Listener
var GlobalProxyListener net.Listener

func tunnelProxy2Agent(proxyConn, agentConn net.Conn) {
	defer proxyConn.Close()
	defer agentConn.Close()
	buf := make([]byte, defaultBufSize)
	bufSize := 0
	for {
		bytesRead, err := proxyConn.Read(buf[bufSize:])
		if err != nil {
			debug.Warn("proxyConn.Read %v", err)
			break
		}
		totalBytesIn += bytesRead
		debug.Info("[R]  %v Bytes Read From Proxy Client %v", bytesRead, proxyConn.RemoteAddr())
		if bytesRead == 0 {
			debug.Warn("1 bytesRead == 0")
			break
		}

		bytesWritten, err := agentConn.Write(buf[:bytesRead+bufSize])
		if err != nil {
			debug.Warn("agentConn Write %v", err)
			break
		}
		debug.Info("[W] %v Bytes Written To Agent %v", bytesWritten, agentConn.RemoteAddr())
	}
	connectedClients -= 1
}

func tunnelAgent2Proxy(proxyConn, agentConn net.Conn) {
	defer proxyConn.Close()
	defer agentConn.Close()
	buf := make([]byte, defaultBufSize)
	bufSize := 0
	for {
		bytesRead, err := agentConn.Read(buf[bufSize:])
		if err != nil {
			debug.Warn("agentConn Read: %v", err)
			return
		}
		debug.Info("[R]  %v Bytes Read From Agent %v", bytesRead, agentConn.RemoteAddr())
		if bytesRead == 0 {
			debug.Warn("2 bytesRead == 0")
			return
		}

		bytesWritten, err := proxyConn.Write(buf[:bytesRead+bufSize])
		if err != nil {
			debug.Error("proxyConn Write: %v", err)
			return
		}

		totalBytesOut += bytesWritten
		debug.Info("[W] %v Bytes Written To Proxy Client %v", bytesWritten, proxyConn.RemoteAddr())
	}
}

func CloseProxy() bool {
	debug.Info("Closing Socks Proxy")
	if ProxyStatus {
		GlobalAgentListener.Close()
		GlobalProxyListener.Close()
		debug.Good("Socks Proxy Closed!")
		ProxyStatus = false
		return true
	}
	debug.Warn("Socks Proxy Not Runing!!")
	return false

}

func StartProxyServer(agentAddr string, proxyAddr string, done chan string) bool {
	var err error
	if ProxyStatus {
		debug.Error("Proxy Already Runing!\n")
		done <- "Proxy Already Runing"
		return false
	}

	GlobalAgentListener, err = net.Listen("tcp", agentAddr)
	if err != nil {

		debug.Error("Listen agentAddr: %v", err)
		done <- "Listen agentAddr Error"
		return false
	}
	debug.Good("[R]  Listening Proxy Interface [%s]", proxyAddr)
	GlobalProxyListener, err = net.Listen("tcp", proxyAddr)
	if err != nil {
		debug.Error("Listen proxyAddr: %v", err)
		done <- "Listen proxyAddr Error"
		return false
	}

	done <- "success"
	ProxyStatus = true

	for {
		buf := make([]byte, defaultBufSize)
		agentConn, err := GlobalAgentListener.Accept()
		if err != nil {
			debug.Error("Agent Listener: %v", err)
			return false
		}
		debug.Info("[R]  Agent Connected-<><>-%v", agentConn.RemoteAddr().String())
		bytesRead, err := agentConn.Read(buf[0:])
		if err != nil {
			debug.Warn("agentConn Read: %v", err)
			continue
		}
		debug.Info("[I] Agent %v Initiated Connection With %v Bytes", agentConn.RemoteAddr(), bytesRead)

		proxyConn, err := GlobalProxyListener.Accept()
		if err != nil {
			debug.Error("Proxy Listener: %v", err)
			return false
		}
		debug.Info("[R]  Proxy Client Connected-<><>-%v", proxyConn.RemoteAddr().String())
		connectedClients += 1
		go tunnelProxy2Agent(proxyConn, agentConn)
		go tunnelAgent2Proxy(proxyConn, agentConn)

	}

}
