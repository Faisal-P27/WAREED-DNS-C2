package main

import (
	"TeamServer/client"
	"TeamServer/database"
	"TeamServer/debug"
	"TeamServer/support"
	"TeamServer/transport"
	"fmt"
	"os"

	"github.com/common-nighthawk/go-figure"
)

func TestTeamServer() {

	client.InitializeNewTarget("bbb")
	client.InitializeNewTarget("ccc")
	client.InitializeNewTarget("aaa")

}

var dbPath string = "teamserver.db"
var argInfo string = "Usege:\n\n./WareedServer [ClientPassword] [ClientPort] [ConfigFilePath]\n\n for db cleaning [WareedServer -c] "

func LoadConfigrationFile(ConfigPath string, ClientPassword string) error {

	database.DB.DropConfig()
	database.DB.Initialize()
	err := support.LoadC2FileConfigration(ConfigPath, ClientPassword)
	if err != nil {
		return err
	}
	support.LoadHistory()
	return nil
}
func LoadConfigrationDatabase(ClientPassword string) error {

	database.DB.Initialize()
	err := support.LoadConfigrationDatabase(ClientPassword)
	if err != nil {
		debug.Error("no configration found in database please add configration file")
		os.Exit(1)
		return err
	}
	support.LoadHistory()
	return nil
}

func printLogo() {
	fmt.Println()
	fmt.Println()
	myFigureW := figure.NewColorFigure(" WAREED", "epic", "red", true)
	myFigureW.Print()
	myFigureD := figure.NewColorFigure("   DNS C2", "epic", "red", true)
	myFigureD.Print()
	fmt.Println()
	fmt.Println()
}
func main() {

	debug.InitLogger()
	debug.Info("STARTING..")
	debug.Info("connect to database..")
	database.NewDB(dbPath)
	if len(os.Args) < 2 {
		debug.Error(argInfo)
		return
	}
	if os.Args[1] == "-c" {
		database.DB.ReCreate()

		return
	}
	ClientPassword := os.Args[1]
	ClientPort := os.Args[2]
	if len(os.Args) == 4 {
		debug.Info("load configration from file")
		ConfigPath := os.Args[3]
		err := LoadConfigrationFile(ConfigPath, ClientPassword)
		if err != nil {
			debug.Error("fail to run c2")
			return
		}

	} else if len(os.Args) == 3 {
		debug.Info("load configration from database")
		err := LoadConfigrationDatabase(ClientPassword)
		if err != nil {
			debug.Error("failed to run c2")
			return
		}
	} else {
		debug.Error(argInfo)
		return
	}
	go client.StartClient("0.0.0.0", ClientPort)

	transport.StartDNSServer()

}
