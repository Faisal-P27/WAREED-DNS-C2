package core

import (
	"TeamServer/cryptography"
	"TeamServer/debug"
	"TeamServer/parser"
	"strconv"
)

func (tm *TargetManager) setMessageSize() error {
	MassgeSize, err := strconv.Atoi(tm.Data[8:])
	if err != nil {
		tm.ClearMessageContainer()
		debug.Error("Failed Registering Goust Session Error: ", err)
		return err
	}
	debug.Info("Set Message Size [%d]", MassgeSize)
	tm.Target.MessageContainer.Size = MassgeSize

	return nil
}

//only for debug
func (tm *TargetManager) displayEncodedData() {
	debug.Info("Encoded Data \n\n[%s]\n", tm.Target.MessageContainer.Payload)
}

func (tm *TargetManager) ParsAndDecryptData() string {
	DecodeBuffer := cryptography.DecryptData(tm.Target.MessageContainer.Payload)
	ParseBuffer := parser.NewParser(DecodeBuffer)
	Result := string(ParseBuffer.ParseBytes())
	return Result
}

func (tm *TargetManager) logDataDownloadProgress() {
	downloadedSize := len(tm.Target.MessageContainer.Payload)
	sizePercentage := float64(downloadedSize) / float64(tm.Target.MessageContainer.Size) * 100
	debug.Info("RID [%d] Received data for session [%s | %s]: [%.2f%%] [%d] out of [%d]", tm.RID, tm.Target.TargetData.SessionID, tm.Target.TargetData.HostName, sizePercentage, downloadedSize, tm.Target.MessageContainer.Size)
}
func (tm *TargetManager) PrintSendProgress() {

	var SizePercentage float64 = float64(float64(tm.Target.TaskContainer[0].Remaing) / float64(tm.Target.TaskContainer[0].Size) * 100)
	debug.Info("RID [%d] Sending [%d] packect out of [%d] Waiting [%.2f%%]", tm.RID, tm.Target.TaskContainer[0].Remaing, tm.Target.TaskContainer[0].Size, SizePercentage)

}
