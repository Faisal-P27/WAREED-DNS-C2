package database

import (
	"gorm.io/gorm"
)

// TargetSession represents the database model for a target session.
type TargetSession struct {
	HostName     string `json:"HostName"`
	OS           string `json:"OS"`
	Privilege    string `json:"Privilege"`
	UserName     string `json:"UserName"`
	ProcessID    string `json:"ProcessID"`
	LastPing     string `json:"LastPing"`
	LastPing1970 string `json:"LastPing1970"`
	SessionID    string `json:"SessionID" gorm:"primaryKey"` // Corrected struct tag
	Status       string
}

// ITargetSession outlines the operations available for managing TargetSession entities.
type ITargetSession interface {
	AddTargetSession(sessionInfo *TargetSession) error
	UpdateTargetSession(sessionInfo *TargetSession) error
	DelTargetSessionByID(id string) error
	GetTargetSessionByID(id string) (*TargetSession, error)
	GetAllTargetSessions() ([]TargetSession, error)
	CleanTargetSessions() error
}

// TargetSessionHandler implements the ITargetSession interface for TargetSession entity management.
type TargetSessionHandler struct {
	db *gorm.DB
}

// NewTargetSessionHandler creates a new handler with a GORM database connection.
func NewTargetSessionHandler(db *gorm.DB) *TargetSessionHandler {
	return &TargetSessionHandler{db: db}
}

func (handler *TargetSessionHandler) AddTargetSession(sessionInfo *TargetSession) error {
	result := handler.db.Create(sessionInfo)
	return result.Error
}

func (handler *TargetSessionHandler) UpdateTargetSession(sessionInfo *TargetSession) error {
	result := handler.db.Save(sessionInfo)
	return result.Error
}

func (handler *TargetSessionHandler) DelTargetSessionByID(id string) error {
	result := handler.db.Delete(&TargetSession{}, "session_id = ?", id)
	return result.Error
}

func (handler *TargetSessionHandler) GetTargetSessionByID(id string) (*TargetSession, error) {
	var session TargetSession
	result := handler.db.Where("session_id = ?", id).First(&session)
	if result.Error != nil {
		return nil, result.Error
	}
	return &session, nil
}

func (handler *TargetSessionHandler) GetAllTargetSessions() ([]TargetSession, error) {
	var sessions []TargetSession
	result := handler.db.Find(&sessions)
	if result.Error != nil {
		return nil, result.Error
	}
	return sessions, nil
}

func (handler *TargetSessionHandler) CleanTargetSessions() error {
	result := handler.db.Delete(&TargetSession{})
	return result.Error
}
