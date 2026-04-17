package writer

import (
	"encoding/csv"
	"fmt"
	"os"
	"time"
)

type CSVWriter struct {
	file   *os.File
	writer *csv.Writer
}

func NewCSVWriter(dir string) (*CSVWriter, error) {
	ts := time.Now().Format("20060102_150405")
	filename := fmt.Sprintf("%s/telemetry_%s.csv", dir, ts)
	f, err := os.Create(filename)
	if err != nil {
		return nil, err
	}
	w := csv.NewWriter(f)
	w.Write([]string{"timestamp_ms", "pressure_psi", "thrust_n", "valve_state"})
	w.Flush()
	return &CSVWriter{file: f, writer: w}, nil
}

func (c *CSVWriter) WriteRow(timestampMs uint32, pressurePsi float32, thrustN float32, valveState uint8) error {
	row := []string{
		fmt.Sprintf("%d", timestampMs),
		fmt.Sprintf("%.4f", pressurePsi),
		fmt.Sprintf("%.4f", thrustN),
		fmt.Sprintf("%d", valveState),
	}
	if err := c.writer.Write(row); err != nil {
		return err
	}
	c.writer.Flush()
	return c.writer.Error()
}

func (c *CSVWriter) Close() error {
	c.writer.Flush()
	return c.file.Close()
}
