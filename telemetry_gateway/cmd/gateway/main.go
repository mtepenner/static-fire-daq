package main

import (
	"encoding/binary"
	"encoding/json"
	"log"
	"math"
	"net/http"
	"os"

	"github.com/mtepenner/static-fire-daq/telemetry_gateway/internal/decoder"
	serialreader "github.com/mtepenner/static-fire-daq/telemetry_gateway/internal/serial"
	wsHub "github.com/mtepenner/static-fire-daq/telemetry_gateway/internal/websocket"
)

type TelemetryPacket struct {
	TimestampMs uint32  `json:"timestamp_ms"`
	PressurePsi float32 `json:"pressure_psi"`
	ThrustN     float32 `json:"thrust_n"`
	ValveState  uint8   `json:"valve_state"`
}

func decodePacket(data []byte) (*TelemetryPacket, error) {
	// TelemetryPacket: uint32(4) + float32(4) + float32(4) + uint8(1) = 13 bytes
	if len(data) < 13 {
		return nil, nil
	}
	pkt := &TelemetryPacket{
		TimestampMs: binary.LittleEndian.Uint32(data[0:4]),
		PressurePsi: math.Float32frombits(binary.LittleEndian.Uint32(data[4:8])),
		ThrustN:     math.Float32frombits(binary.LittleEndian.Uint32(data[8:12])),
		ValveState:  data[12],
	}
	return pkt, nil
}

func main() {
	serialPort := os.Getenv("SERIAL_PORT")
	if serialPort == "" {
		serialPort = "/dev/ttyUSB0"
	}

	hub := wsHub.NewHub()
	go hub.Run()

	byteChan := make(chan byte, 4096)
	go serialreader.ReadSerial(serialPort, 2000000, byteChan)

	// COBS frame processor
	go func() {
		var frame []byte
		for b := range byteChan {
			if b == 0x00 {
				if len(frame) > 0 {
					// append the zero terminator for the decoder
					decoded, err := decoder.Decode(append(frame, 0x00))
					if err != nil {
						log.Printf("cobs decode error: %v", err)
					} else {
						pkt, err := decodePacket(decoded)
						if err != nil || pkt == nil {
							log.Printf("packet decode error or short packet")
						} else {
							jsonData, err := json.Marshal(pkt)
							if err == nil {
								hub.Broadcast(jsonData)
							}
						}
					}
					frame = frame[:0]
				}
			} else {
				frame = append(frame, b)
			}
		}
	}()

	http.HandleFunc("/ws", func(w http.ResponseWriter, r *http.Request) {
		wsHub.ServeWs(hub, w, r)
	})
	http.HandleFunc("/health", func(w http.ResponseWriter, r *http.Request) {
		w.WriteHeader(http.StatusOK)
		w.Write([]byte("ok"))
	})

	log.Println("gateway listening on :8080")
	log.Fatal(http.ListenAndServe(":8080", nil))
}
