package cryptography

import (
	"TeamServer/debug"
	"encoding/base32"
	"strings"
)

// encryption xor key between agent <-> teamserver
var Key = []byte("ABCD")

func EncryptData(b []byte) string {
	KeyLen := len(Key)
	k := 0
	encryptedBytes := make([]byte, len(b))
	for i := 0; i < len(b); i++ {
		if k >= KeyLen {

			k = 0
		}
		encryptedBytes[i] = b[i] ^ Key[k]
		k++

	}
	encodedBytes := base32.StdEncoding.EncodeToString(encryptedBytes)
	return encodedBytes
}

func DecryptData(b string) []byte {
	KeyLen := len(Key)
	k := 0
	b = strings.ToUpper(b)
	Base32 := strings.Replace(b, "-", "=", -1)
	debug.Info("Decode Data [%s]", Base32)
	DecodeBytes, err := base32.StdEncoding.DecodeString(Base32)

	if err != nil {
		debug.Error("Decoding error:", err)
		return nil
	}
	decryptedBytes := make([]byte, len(DecodeBytes))

	for i := 0; i < len(decryptedBytes); i++ {
		if k >= KeyLen {
			k = 0
		}
		decryptedBytes[i] = DecodeBytes[i] ^ Key[k]
		k++

	}
	return decryptedBytes
}
