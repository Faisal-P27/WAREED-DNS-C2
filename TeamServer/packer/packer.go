package packer

import (
	"encoding/binary"
)

type Packer struct {
	data []byte
	size int
}

func NewPacker() *Packer {
	var packer = new(Packer)

	return packer
}

func (p *Packer) AddInt64(data int64) {
	var buffer = make([]byte, 4)
	binary.LittleEndian.PutUint32(buffer, uint32(data))
	p.data = append(p.data, buffer...)
}

func (p *Packer) AddInt32(data int32) {
	var buffer = make([]byte, 4)
	binary.LittleEndian.PutUint32(buffer, uint32(data))
	p.data = append(p.data, buffer...)
}

func (p *Packer) AddInt(data int) {
	var buffer = make([]byte, 4)
	binary.LittleEndian.PutUint32(buffer, uint32(data))
	p.data = append(p.data, buffer...)
}

// AddUInt32 use a much as possible this function
func (p *Packer) AddUInt32(data uint32) {
	var buffer = make([]byte, 4)
	binary.LittleEndian.PutUint32(buffer, data)
	p.data = append(p.data, buffer...)
}

func (p *Packer) AddString(data string) {
	var buffer = make([]byte, 4)
	binary.LittleEndian.PutUint32(buffer, uint32(len(data)))
	p.data = append(p.data, buffer...)
	p.data = append(p.data, []byte(data)...)
}

func (p *Packer) AddBytes(data []byte) {
	var buffer = make([]byte, 4)
	binary.LittleEndian.PutUint32(buffer, uint32(len(data)))
	p.data = append(p.data, buffer...)
	p.data = append(p.data, data...)
}

func (p *Packer) AddLastBytes(data []byte) {
	p.data = append(p.data, data...)
}

func (p *Packer) Buffer() []byte {
	return p.data
}

func (p *Packer) Size() int {
	return p.size
}

func (p *Packer) AddOwnSizeFirst() {
	var oldData = p.data
	p.data = nil
	p.AddInt(len(oldData))
	p.AddLastBytes(oldData)
}
