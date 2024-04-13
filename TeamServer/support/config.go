package support

import (
	"TeamServer/database"
	"TeamServer/debug"
	"TeamServer/macro"
	"encoding/hex"
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"os"
	"strings"

	"github.com/asaskevich/govalidator"
	"golang.org/x/crypto/sha3"
)

func MapAndAddHamsConfigToDB(hamsConfig macro.HamsConfig) error {
	debug.Info("Save new config to database")
	c2Config := database.C2Config{
		MagicValue:       hamsConfig.MagicValue,
		Domains:          strings.Join(hamsConfig.Domains, ","), // Join slice into a single string
		ListenerPort:     hamsConfig.ListenerPort,
		Sleep:            hamsConfig.Sleep,
		Jitter:           hamsConfig.Jitter,
		LookupDelay:      hamsConfig.LookupDelay,
		DefaultDNSServer: hamsConfig.DefaultDNSServer,
	}

	// Add mapped C2Config to database
	return database.DB.C2ConfigHandler.AddC2Config(&c2Config)
}

func MapC2ConfigToHamsConfig(c2Config *database.C2Config) macro.HamsConfig {
	// Assuming domains are separated by commas, split the string into a slice.
	domainSlice := strings.Split(c2Config.Domains, ",")

	// Map each field from C2Config to HamsConfig.
	return macro.HamsConfig{
		MagicValue:       c2Config.MagicValue,
		Domains:          domainSlice,
		ListenerPort:     c2Config.ListenerPort,
		Sleep:            c2Config.Sleep,
		Jitter:           c2Config.Jitter,
		LookupDelay:      c2Config.LookupDelay,
		DefaultDNSServer: c2Config.DefaultDNSServer,
	}
}

func LoadConfigrationDatabase(TeamServerPassword string) error {
	SetClientPass(TeamServerPassword)
	config, err := database.DB.C2ConfigHandler.GetC2ConfigByID(1) // only one config can be save in db
	if err != nil {
		debug.Error(err.Error())
		return err
	}
	hamsConfig := MapC2ConfigToHamsConfig(config)
	macro.Config = hamsConfig
	SetUpDomainsListener(macro.Config.Domains)
	printConfig()
	return nil
}

func verifyHamsConfig(config *macro.HamsConfig) error {
	// Validate MagicValue for a specific length of 3 characters.
	if len(config.MagicValue) != 3 {
		return errors.New("invalid 'magic_value': must have a length of 3")
	}

	// Validate each domain in Domains.
	for _, domain := range config.Domains {
		if !govalidator.IsDNSName(domain) {
			return fmt.Errorf("invalid domain in 'domains': %s", domain)
		}
	}

	// Validate DefaultDNSServer as a valid IP address.
	if !govalidator.IsIP(config.DefaultDNSServer) {
		return fmt.Errorf("invalid 'default_dns_server' value: %s", config.DefaultDNSServer)
	}

	// Validate ListenerPort as a valid port.
	if !govalidator.IsPort(config.ListenerPort) {
		return fmt.Errorf("invalid 'listener_port' value: %s", config.ListenerPort)
	}

	// Validate Sleep as a positive integer.
	if config.Sleep <= 0 {
		return errors.New("invalid 'sleep': must be a positive integer")
	}

	// Validate Jitter as a non-negative integer.
	if config.Jitter < 0 {
		return errors.New("invalid 'jitter': must be a non-negative integer")
	}

	// Validate LookupDelay as a positive integer.
	if config.LookupDelay <= 0 {
		return errors.New("invalid 'lookup_delay': must be a positive integer")
	}

	// If all checks pass, return nil indicating the configuration is valid.
	return nil
}

func LoadC2FileConfigration(configPath string, TeamServerPassword string) error {
	var Config macro.HamsConfig
	file, err := os.Open(configPath)
	if err != nil {
		debug.Error(" opening file:", err)

		return err
	}
	defer file.Close()

	data, err := ioutil.ReadAll(file)
	if err != nil {
		debug.Error(" reading file:", err)
		return err
	}

	err = json.Unmarshal(data, &Config)
	if err != nil {
		debug.Error(" unmarshalling JSON:", err)
		return err
	}
	err = SetConfig(&Config, TeamServerPassword)
	if err != nil {
		return err
	}
	return nil
}

func SetClientPass(TeamServerPassword string) {

	Hasher := sha3.New256()
	Hasher.Write([]byte(TeamServerPassword))
	macro.ServicePass = hex.EncodeToString(Hasher.Sum(nil))

}

func SetUpDomainsListener(Domains []string) {

	for _, DomainName := range Domains {

		AddNewListener(DomainName)

	}
}
func printConfig() {
	debug.Good("configration Loaded:")
	debug.Info("Magic Value   :%s", macro.Config.MagicValue)
	debug.Info("Domains      :%s", macro.Config.Domains)
	debug.Info("Listener Port :%s", macro.Config.ListenerPort)
	debug.Info("Sleep        :%d", macro.Config.Sleep)
	debug.Info("Jitter       :%d", macro.Config.Jitter)
	debug.Info("Lookup Delay  :%d", macro.Config.LookupDelay)
	debug.Info("Default DNS Server  :%s", macro.Config.DefaultDNSServer)

}
func SetConfig(Config *macro.HamsConfig, TeamServerPassword string) error {
	SetClientPass(TeamServerPassword)
	err := verifyHamsConfig(Config)
	if err != nil {
		debug.Error(err.Error())
		return err
	}
	macro.Config = *Config
	printConfig()
	SetUpDomainsListener(macro.Config.Domains)

	err = MapAndAddHamsConfigToDB(macro.Config)
	if err != nil {
		debug.Error(err.Error())
		return err
	}
	debug.Good("Configration Loaded Successfully")
	return nil
}
func LoadOldSessions() {
	sessions, err := database.DB.TargetSessionHandler.GetAllTargetSessions()
	if err != nil {
		debug.Error("retrieving sessions: ", err.Error())
		return
	}

	for _, session := range sessions {
		if session.HostName != "<Goust>" {
			macro.TargetMap[session.SessionID] = &macro.TargetInfo{
				TargetData:    session,
				ActiveMessage: false,
				MassageQueue:  1,
			}
		}
	}
	debug.Good("Successfully loaded and initialized sessions.")
}

func LoadHistory() {
	LoadOldDomains()
	LoadOldSessions()
}

func LoadOldDomains() {
	listeners, err := database.DB.ListenerInfoHandler.GetAllListenerInfos()
	if err != nil {
		debug.Error("retrieving listener information: %v", err)
		return
	}

	for _, listener := range listeners {
		debug.Good("Loading Domain [%s]", listener.DomainName)
		_, ok := macro.ListenerMap[listener.DomainName]
		if !ok {
			macro.ListenerMap[listener.DomainName] = &listener
		} else {
			debug.Info("Domain [%s] already loaded to memory", listener.DomainName)

		}

	}

	debug.Good("Successfully loaded listener information into ListenerMap.")
}
func AddNewListener(DomainName string) (bool, *database.ListenerInfo) {
	Listener, ok := macro.ListenerMap[DomainName]
	if !ok {
		Listener := new(database.ListenerInfo)
		Listener.DomainName = DomainName
		Listener.Protocol = "DNS\\IPV6"
		Listener.BindIP = "0.0.0.0"
		Listener.BindPort = macro.Config.ListenerPort
		Listener.Status = "Active"
		Listener.Hits = 0
		macro.ListenerMap[Listener.DomainName] = Listener
		debug.Good("New Listener [%s] Added Successfully", Listener.DomainName)
		_, err := database.DB.ListenerInfoHandler.GetListenerInfoByName(Listener.DomainName)
		if err != nil {
			err := database.DB.ListenerInfoHandler.AddListenerInfo(Listener)
			if err != nil {
				debug.Error("add new domain listener: %v", err)

			}
		} else {
			debug.Info("Domain [%s] already added to database", Listener.DomainName)
		}

		return true, Listener

	}
	debug.Warn("Listener [%s] Already Added!", Listener.DomainName)

	return false, Listener
}

func DeleteListener(DomainName string) (bool, *database.ListenerInfo) {
	// Check if the listener exists in the map
	Listener, ok := macro.ListenerMap[DomainName]
	if !ok {
		// If not found, log an error and return false and nil
		debug.Error(fmt.Sprintf("Delete Listener [%s] Not Found!", DomainName))
		return false, nil
	}

	// If found, delete the listener from the map
	delete(macro.ListenerMap, DomainName)

	// Return true and the deleted listener information
	return true, Listener
}

func GetDomainListeners(Listeners []string) (string, int, error) {
	if len(Listeners) == 0 {
		return "", 0, errors.New("no listeners provided")
	}

	var verifiedDomains []string
	for _, domain := range Listeners {
		listener, exists := macro.ListenerMap[domain]
		if exists {
			verifiedDomains = append(verifiedDomains, "\""+listener.DomainName+"\"")
		}
	}

	if len(verifiedDomains) == 0 {
		return "", 0, errors.New("no valid domains found")
	}

	domainsString := strings.Join(verifiedDomains, ",")
	return domainsString, len(verifiedDomains), nil
}
