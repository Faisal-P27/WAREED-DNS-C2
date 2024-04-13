package support

import (
	"TeamServer/macro"
	"math/rand"
	"time"

	"github.com/google/uuid"
)

func init() {
	rand.Seed(time.Now().UnixNano())
}

var letterRunes = []byte("abcdefghijklmnopqrstuvwxyz123456789")
var ipv6letterSave = []byte("abcde2345678")
var ipv6letterRunes = []byte("abcdef1234567890")

func RandStringRunes(n int) string {
	b := make([]byte, n)
	for i := range b {
		b[i] = letterRunes[rand.Intn(len(letterRunes))]
	}
	return string(b)
}
func Randipv6Runes(n int) (string, error) {
	n = n * 2
	b := make([]byte, n)
	i := 2
	for i = range b {
		b[i] = ipv6letterRunes[rand.Intn(len(ipv6letterRunes))]
	}
	b[0] = ipv6letterSave[rand.Intn(len(ipv6letterSave))]
	b[1] = ipv6letterSave[rand.Intn(len(ipv6letterSave))]

	return string(b), nil
}

func GenerateIPv6(IPv6 string) string {
	p := 4
	for i := 0; i < 7; i++ {
		IPv6 = IPv6[:p] + ":" + IPv6[p:]
		p = p + 5
	}

	return IPv6
}

func RemoveTask(s []*macro.TasksPacket, i int) []*macro.TasksPacket {
	s[i] = s[len(s)-1]
	return s[:len(s)-1]
}

// Check if a string is contained within the array
func Contains(slice []string, item string) bool {

	for _, a := range slice {

		if a == item {
			return true
		}
	}
	return false
}

func AddNullByte(str string) string {
	return str + "\x00"
}

// GenerateRandomGUID generates a random GUID (UUID) and returns it as a string.
func GenerateRandomID() string {
	randomGUID, err := uuid.NewRandom()
	if err != nil {
		panic(err)
	}
	return randomGUID.String()
}

func GetTime() string {
	return time.Now().Format("02/01/2006 15:04:05")
}
