package database

import "gorm.io/gorm"

type IListenerInfoHandler interface {
	AddListenerInfo(listenertInfo *ListenerInfo) error
	UpdateListenerInfo(listenertInfo *ListenerInfo) error
	DelListenerInfoByName(name string) error
	GetListenerInfoByName(name string) (*ListenerInfo, error)
	GetAllListenerInfos() ([]ListenerInfo, error)
	CleanListenerInfos() error
}

type ListenerInfo struct {
	DomainName string `gorm:"primaryKey"`
	Protocol   string
	BindIP     string
	BindPort   string
	Status     string
	Hits       int
}

type ListenerInfoHandler struct {
	db *gorm.DB
}

func (m *ListenerInfoHandler) AddListenerInfo(listenerInfo *ListenerInfo) error {
	result := m.db.Create(listenerInfo)
	return result.Error
}

func (m *ListenerInfoHandler) UpdateListenerInfo(listenerInfo *ListenerInfo) error {
	result := m.db.Save(listenerInfo)
	return result.Error
}

func (m *ListenerInfoHandler) DelListenerInfoByName(name string) error {
	result := m.db.Delete(&ListenerInfo{}, name)
	return result.Error
}

func (m *ListenerInfoHandler) GetListenerInfoByName(name string) (*ListenerInfo, error) {
	var listenerInfo ListenerInfo
	result := m.db.First(&listenerInfo, "domain_name = ?", name)
	return &listenerInfo, result.Error
}

func (m *ListenerInfoHandler) GetAllListenerInfos() ([]ListenerInfo, error) {
	var listenerInfos []ListenerInfo
	result := m.db.Find(&listenerInfos)
	return listenerInfos, result.Error
}

func (m *ListenerInfoHandler) CleanListenerInfos() error {
	result := m.db.Where("1 = 1").Delete(&ListenerInfo{})
	return result.Error
}
