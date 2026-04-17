package main

import (
	"encoding/json"
	"log"
	"os"
	"time"

	"github.com/gorilla/websocket"
	"github.com/mtepenner/static-fire-daq/data_logger/internal/writer"
)

type TelemetryPacket struct {
	TimestampMs uint32  `json:"timestamp_ms"`
	PressurePsi float32 `json:"pressure_psi"`
	ThrustN     float32 `json:"thrust_n"`
	ValveState  uint8   `json:"valve_state"`
}

func main() {
	gatewayURL := os.Getenv("GATEWAY_URL")
	if gatewayURL == "" {
		gatewayURL = "ws://localhost:8080/ws"
	}

	dataDir := os.Getenv("DATA_DIR")
	if dataDir == "" {
		dataDir = "/data"
	}

	for {
		if err := run(gatewayURL, dataDir); err != nil {
			log.Printf("logger error: %v, reconnecting in 5s...", err)
		}
		time.Sleep(5 * time.Second)
	}
}

func run(gatewayURL, dataDir string) error {
	log.Printf("connecting to %s", gatewayURL)
	conn, _, err := websocket.DefaultDialer.Dial(gatewayURL, nil)
	if err != nil {
		return err
	}
	defer conn.Close()

	csvWriter, err := writer.NewCSVWriter(dataDir)
	if err != nil {
		return err
	}
	defer csvWriter.Close()

	log.Println("logger connected, writing CSV")
	for {
		_, msg, err := conn.ReadMessage()
		if err != nil {
			return err
		}
		var pkt TelemetryPacket
		if err := json.Unmarshal(msg, &pkt); err != nil {
			log.Printf("unmarshal error: %v", err)
			continue
		}
		if err := csvWriter.WriteRow(pkt.TimestampMs, pkt.PressurePsi, pkt.ThrustN, pkt.ValveState); err != nil {
			log.Printf("csv write error: %v", err)
		}
	}
}
