package client

import (
	"TeamServer/debug"
	"encoding/json"
	"time"
)

//RecivCode
var (
	LoginCT int = 500
)

//SendCode
var (
	LoginTC              int = 100
	NewTargetSessionTC   int = 101
	PrintConsoleTC       int = 102
	UpdateLastSeenTC     int = 103
	NewTaskSending       int = 104
	NewListenerTC        int = 105
	UpdateListenerHitsTC int = 106
	MessageBoxTC         int = 107
	NewPayloadTC         int = 108
	DelListenerTC        int = 109
	C2CongigTC           int = 110
)

type Packager struct{}

type Package struct {
	Head struct {
		Event int `json:"Event"`

		User    string `json:"User"`
		Time    string `json:"Time"`
		OneTime string `json:"OneTime"`
	}

	Body struct {
		SubEvent int `json:"SubEvent"`

		Info map[string]interface{} `json:"Info"`
	}
}

func NewPackager() *Packager {
	return new(Packager)
}

func (p Packager) CreatePackage(jsonObject []byte) Package {
	var pk Package

	if err := json.Unmarshal(jsonObject, &pk); err != nil {
		debug.Error("Error while creating Package struct :: " + err.Error())
	}

	return pk
}

func InitializePackge(Event int, SubEvent int) Package {
	var Package Package
	Package.Head.Event = Event
	Package.Head.Time = time.Now().Format("02/01/2006 15:04:05")
	Package.Body.SubEvent = SubEvent
	Package.Body.Info = make(map[string]interface{})

	return Package
}
