package debug

import (
	"io"
	"log"
	"os"
)

var Log *Logger

func InitLogger() {
	Log = NewLogger(os.Stdout)
}

func NewLogger(StdOut io.Writer) *Logger {
	var logger = new(Logger)

	logger.STDOUT = os.Stdout
	logger.STDERR = os.Stderr
	logger.showTime = true
	logger.debug = false
	logger.log = log.New(StdOut, "", 0)

	return logger
}

func Info(str string, args ...interface{}) {
	Log.Info(str, args...)
}

func Good(str string, args ...interface{}) {
	Log.Good(str, args...)
}

func Debug(str string, args ...interface{}) {
	Log.Debug(str, args...)
}

func DebugError(str string, args ...interface{}) {
	Log.DebugError(str, args...)
}

func Warn(str string, args ...interface{}) {
	Log.Warn(str, args...)
}

func Error(str string, args ...interface{}) {
	Log.Error(str, args...)
}

func Fatal(args ...interface{}) {
	Log.Fatal(args...)
}

func Panic(args ...interface{}) {
	Log.Panic(args...)
}

func SetDebug(enable bool) {
	Log.SetDebug(enable)
}

func ShowTime(time bool) {
	Log.ShowTime(time)
}

func SetStdOut(w io.Writer) {
	Log.log.SetOutput(w)
}
