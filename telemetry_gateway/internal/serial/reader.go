package serial

import (
	"log"

	"go.bug.st/serial"
)

func ReadSerial(portName string, baud int, out chan<- byte) {
	mode := &serial.Mode{
		BaudRate: baud,
	}
	port, err := serial.Open(portName, mode)
	if err != nil {
		log.Fatalf("failed to open serial port %s: %v", portName, err)
	}
	defer port.Close()

	buf := make([]byte, 256)
	for {
		n, err := port.Read(buf)
		if err != nil {
			log.Fatalf("serial read error: %v", err)
		}
		for i := 0; i < n; i++ {
			out <- buf[i]
		}
	}
}
