package debug

import (
	"fmt"
	"log"
	"os"
	"runtime"
	"strconv"
	"strings"
	"time"
)

func FunctionTrace() (string, int) {
	var (
		frame    runtime.Frame
		frames   *runtime.Frames
		caller   = make([]uintptr, 15)
		callNums int
	)

	callNums = runtime.Callers(2, caller)

	frames = runtime.CallersFrames(caller[:callNums])

	frame, _ = frames.Next()
	frame, _ = frames.Next()
	frame, _ = frames.Next()

	return frame.Function, frame.Line
}

type Logger struct {
	STDOUT *os.File
	STDERR *os.File

	log      *log.Logger
	showTime bool
	debug    bool
}

func (logger *Logger) Info(str string, args ...interface{}) {
	if logger.showTime {
		logger.log.SetPrefix("[" + Green(time.Now().Format("15:04:05")) + "] [" + Blue("INFO") + "] ")
	} else {
		logger.log.SetPrefix("[" + Blue("INFO") + "] ")
	}
	logger.log.Printf(str+"\n", args...)
}

func (logger *Logger) Good(str string, args ...interface{}) {
	if logger.showTime {
		logger.log.SetPrefix("[" + Green(time.Now().Format("15:04:05")) + "] [" + Green("GOOD") + "] ")
	} else {
		logger.log.SetPrefix("[" + Green("GOOD") + "] ")
	}
	logger.log.Printf(str+"\n", args...)
}

func (logger *Logger) Debug(str string, args ...interface{}) {
	var Trace, Line = FunctionTrace()
	var Functions = strings.Split(Trace, "/")
	if logger.debug {
		if logger.showTime {
			logger.log.SetPrefix("[" + Green(time.Now().Format("15:04:05")) + "] [" + Yellow("DBUG") + "] [" + BlueUnderline(Functions[len(Functions)-1]+":"+strconv.Itoa(Line)) + "]: ")
		} else {
			logger.log.SetPrefix("[" + Yellow("DBUG") + "] [" + Functions[len(Functions)-1] + ":" + fmt.Sprintf("%03d", Line) + "]: ")
		}
		logger.log.Printf(str+"\n", args...)
	}
}

func (logger *Logger) DebugError(str string, args ...interface{}) {
	var Trace, Line = FunctionTrace()
	var Functions = strings.Split(Trace, "/")
	if logger.debug {
		if logger.showTime {
			logger.log.SetPrefix("[" + Green(time.Now().Format("15:04:05")) + "] [" + BoldRed("DBER") + "] [" + BlueUnderline(Functions[len(Functions)-1]+":"+strconv.Itoa(Line)) + "]: ")
		} else {
			logger.log.SetPrefix("[" + BoldRed("DBER") + "] [" + Functions[len(Functions)-1] + ":" + fmt.Sprintf("%03d", Line) + "]: ")
		}
		logger.log.Printf(str+"\n", args...)
	}
}

func (logger *Logger) Warn(str string, args ...interface{}) {
	if logger.showTime {
		logger.log.SetPrefix("[" + Green(time.Now().Format("15:04:05")) + "] [" + Yellow("WARN") + "] ")
	} else {
		logger.log.SetPrefix("[" + Yellow("WARN") + "] ")
	}
	logger.log.Printf(str+"\n", args...)
}

func (logger *Logger) Error(str string, args ...interface{}) {
	if logger.showTime {
		logger.log.SetPrefix("[" + Green(time.Now().Format("15:04:05")) + "] [" + Red("Error") + "] ")
	} else {
		logger.log.SetPrefix("[" + Red("Error") + "] ")
	}
	logger.log.Printf(str+"\n", args...)
}

func (logger *Logger) Fatal(args ...interface{}) {
	if logger.showTime {
		logger.log.SetPrefix("[" + Green(time.Now().Format("15:04:05")) + "] [" + BoldRed("FATA") + "] ")
	} else {
		logger.log.SetPrefix("[" + BoldRed("FATA") + "] ")
	}
	logger.log.Println(args...)
	os.Exit(1)
}

func (logger *Logger) Panic(args ...interface{}) {
	if logger.showTime {
		logger.log.SetPrefix("[" + Green(time.Now().Format("15:04:05")) + "] [" + BoldRed("PANIC") + "] ")
	} else {
		logger.log.SetPrefix("[" + BoldRed("PANIC") + "] ")
	}
	logger.log.Println(args...)
	panic(args)
}

func (logger *Logger) SetDebug(enable bool) {
	logger.debug = enable
}

func (logger *Logger) ShowTime(time bool) {
	logger.showTime = time
}
