package decoder

import "errors"

// Decode decodes a COBS-encoded packet. The input should NOT include the
// trailing 0x00 terminator, or it should be stripped before calling.
func Decode(input []byte) ([]byte, error) {
	// Strip trailing zero terminator if present
	if len(input) > 0 && input[len(input)-1] == 0x00 {
		input = input[:len(input)-1]
	}

	if len(input) == 0 {
		return []byte{}, nil
	}

	output := make([]byte, 0, len(input))
	idx := 0

	for idx < len(input) {
		code := int(input[idx])
		idx++

		if idx+code-1 > len(input) {
			return nil, errors.New("cobs: malformed packet")
		}

		for i := 1; i < code; i++ {
			output = append(output, input[idx])
			idx++
		}

		if code < 0xFF && idx < len(input) {
			output = append(output, 0x00)
		}
	}

	return output, nil
}
