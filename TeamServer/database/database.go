package database

import (
	"TeamServer/debug"
	"fmt"

	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
)

type DatabaseManager struct {
	db                   *gorm.DB
	ListenerInfoHandler  IListenerInfoHandler
	C2ConfigHandler      IC2Config
	TargetSessionHandler ITargetSession
}

var DB *DatabaseManager

func NewDB(dbPath string) (*DatabaseManager, error) {
	db, err := gorm.Open(sqlite.Open(dbPath), &gorm.Config{})
	if err != nil {
		return nil, fmt.Errorf("failed to open database: %v", err)
	}
	DB = &DatabaseManager{
		db:                   db,
		ListenerInfoHandler:  &ListenerInfoHandler{db: db},
		C2ConfigHandler:      &C2ConfigHandler{db: db},
		TargetSessionHandler: &TargetSessionHandler{db: db},
	}

	return DB, nil
}

func (manager *DatabaseManager) Initialize() error {
	debug.Info("Initialize Database..")
	err := manager.db.AutoMigrate(
		&ListenerInfo{},
		&C2Config{},
		&TargetSession{},
	)

	if err != nil {
		return fmt.Errorf("failed to auto-migrate tables: %v", err)
	}

	return nil
}

func (manager *DatabaseManager) Drop() error {
	debug.Info("Drop Database..")
	err := manager.db.Migrator().DropTable(

		&ListenerInfo{},
		&C2Config{},
		&TargetSession{},
	)
	if err != nil {
		return fmt.Errorf("failed to drop existing tables: %v", err)
	}
	//fmt.Println("Existing tables dropped successfully!")

	return nil
}

func (manager *DatabaseManager) DropConfig() error {
	debug.Info("Drop Database..")
	err := manager.db.Migrator().DropTable(
		&C2Config{},
	)
	if err != nil {
		return fmt.Errorf("failed to drop existing tables: %v", err)
	}

	return nil
}

func (manager *DatabaseManager) ReCreate() error {

	err := manager.Drop()

	if err != nil {
		return fmt.Errorf("failed to drop existing tables: %v", err)
	}

	manager.Initialize()
	if err != nil {
		return fmt.Errorf("failed to Initialize  tables: %v", err)
	}
	//	fmt.Println("tables Initialize successfully!")
	debug.Good("database recreated successfully")
	return nil
}
