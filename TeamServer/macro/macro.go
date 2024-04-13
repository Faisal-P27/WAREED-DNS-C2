package macro

import (
	"TeamServer/database"
	"sync"

	"github.com/gorilla/websocket"
)

var TeamServerMagicValue string = "cdn"
var Config HamsConfig
var ServicePass string

//Client:
//PrintConsole Type
var PrintConsoleShell string = "1"
var PrintConsoleSendTask string = "2"
var PrintConsoleWarning string = "3"
var PrintConsoleSuccessful string = "4"

//Message
var Brodcast int = 1
var Single int = 2

type MessagePacket struct {
	Payload     string
	Size        int
	MessageType string
	IsLastPart  bool
}

type TasksPacket struct {
	Task        []string
	Size        int
	Remaing     int
	TaskType    string
	TaskName    string
	IsLastPart  bool
	IsFirstPart bool
	TaskID      string
}

type MessageCache struct {
	LastLockup   string
	LastResponse string
}

type TargetInfo struct {
	TargetData       database.TargetSession
	ActiveMessage    bool
	LastMessage      MessageCache
	MassageQueue     int
	MessageContainer MessagePacket
	TaskContainer    []*TasksPacket
	sync.RWMutex
}

type HamsConfig struct {
	MagicValue       string   `json:"magic_value"`
	Domains          []string `json:"domains"`
	ListenerPort     string   `json:"listener_port"`
	Sleep            int      `json:"sleep"`
	Jitter           int      `json:"jitter"`
	LookupDelay      int      `json:"lookup_delay"`
	DefaultDNSServer string   `json:"default_dns_server"`
}

var TargetMap = map[string]*TargetInfo{}

var TargetMapBackUp = map[string]TargetInfo{}

var ListenerMap = map[string]*database.ListenerInfo{}

var MacroRegisterCode string = "01"
var MacroRegisterRemaingCode string = "02"
var MacroResultCode string = "03"
var MacroAskTaskCode string = "04"

var MacroShellResultCode string = "07"
var MacroShellResultRemaingCode string = "08"
var MacroInternetTest string = "09"
var MacroInternetTestResult string = "10"
var MacroInternetTestResultRemaing string = "11"

var MacroStartProxyResult string = "13"

var MacroStopProxy string = "14"
var MacroStopProxyResult string = "15"

var MacroTaskBufferDoneCode string = "f0f0:f0f0:f0f0:f0f0:f0f0:f0f0:f0f0:f0f0"
var MacroTaskNotFoundCode string = "f1f1:f1f1:f1f1:f1f1:f1f1:f1f1:f1f1:f1f1"
var MacroDomainValidCode string = "2001:0db8:85a3:0000:0000:8a2e:0370:7334"

var MacroDataEnd string = "99"

var MacroNotiNewPing int = 6

var MacroSizeOfJunkData int = 4

var MacroNoTaskCodeTA string = "2000"
var MacroChangeSleepCodeTA string = "0500"
var MacroShellCodeTA string = "0600"
var MacroShellResultCodeTA string = "0700"

var MacroInjectCodeTA string = "1100"
var MacroInjectCodeAT string = "17"

var MacroKillAT string = "16"
var MacroKillTA string = "1000"

var MacroSelfDelAT string = "15"
var MacroSelfDelTA string = "0F00"

var MacroNewCallBackDomainCodeTA string = "1200"
var MacroNewCallBackDomainCodeAT string = "18"

var MacroValidateDomainCodeAT string = "19"

var MacroDeleteBackDomainCodeTA string = "1400"
var MacroDeleteBackDomainCodeAT string = "20"

var MacroValidateDeletedDomainCodeTA string = "1500"
var MacroValidateDeletedDomainCodeAT string = "21"

var MacroListAllActiveDomainCodeTA string = "1600"
var MacroListAllActiveDomainCodeAT string = "22"

var MacroStartProxyCodeTA string = "0c00"
var MacroStartProxyCodeAT string = "12"

var MacroStopProxyCodeTA string = "0e00"
var MacroStopProxyCodeAT string = "14"

var MacroChangeSleepCodeAT string = "05"
var MacroShellCodeAT string = "06"
var MacroShellResultCodeAT string = "07"
var MacroShellResultRemaingCodeAT string = "08"

type ClientInfo struct {
	SessionID string
	UserName  string
	Conn      *websocket.Conn
	Mutex     sync.Mutex
}

var ClientMap = map[string]*ClientInfo{}
var Macro_CT_AddChangeSleep int = 3
