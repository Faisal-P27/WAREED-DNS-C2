package database

import (
	"gorm.io/gorm"
)

// C2Config represents the database model for C2 configuration.
type C2Config struct {
	ID               int    `gorm:"primaryKey"`
	MagicValue       string `json:"magic_value"`
	Domains          string `json:"domains"`
	ListenerPort     string `json:"listener_port"`
	Sleep            int    `json:"sleep"`
	Jitter           int    `json:"jitter"`
	LookupDelay      int    `json:"lookup_delay"`
	DefaultDNSServer string `json:"default_dns_server"`
}

// IC2Config outlines the operations available for managing C2Config entities.
type IC2Config interface {
	AddC2Config(config *C2Config) error
	UpdateC2Config(config *C2Config) error
	DelC2ConfigByID(id int) error
	GetC2ConfigByID(id int) (*C2Config, error)
	GetAllC2Configs() ([]C2Config, error)
	CleanC2Configs() error
}

// C2ConfigHandler implements the IC2Config interface for C2Config entity management.
type C2ConfigHandler struct {
	db *gorm.DB
}

// AddC2Config adds a new C2Config to the database.
func (handler *C2ConfigHandler) AddC2Config(config *C2Config) error {
	result := handler.db.Create(config)
	return result.Error
}

// UpdateC2Config updates an existing C2Config in the database.
func (handler *C2ConfigHandler) UpdateC2Config(config *C2Config) error {
	result := handler.db.Save(config)
	return result.Error
}

// DelC2ConfigByID deletes a C2Config from the database by ID.
func (handler *C2ConfigHandler) DelC2ConfigByID(id int) error {
	result := handler.db.Delete(&C2Config{}, id)
	return result.Error
}

// GetC2ConfigByID retrieves a C2Config by ID from the database.
func (handler *C2ConfigHandler) GetC2ConfigByID(id int) (*C2Config, error) {
	var config C2Config
	result := handler.db.First(&config, id)
	if result.Error != nil {
		return nil, result.Error
	}
	return &config, nil
}

// GetAllC2Configs retrieves all C2Configs from the database.
func (handler *C2ConfigHandler) GetAllC2Configs() ([]C2Config, error) {
	var configs []C2Config
	result := handler.db.Find(&configs)
	if result.Error != nil {
		return nil, result.Error
	}
	return configs, nil
}

// CleanC2Configs deletes all C2Configs from the database.
func (handler *C2ConfigHandler) CleanC2Configs() error {
	result := handler.db.Where("1 = 1").Delete(&C2Config{})
	return result.Error
}
