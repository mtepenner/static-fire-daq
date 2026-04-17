interface Props {
  sendCommand: (cmd: string) => void
}

export default function AbortPanel({ sendCommand }: Props) {
  function handleAbort() {
    if (window.confirm('CONFIRM ABORT: This will close all valves immediately. Proceed?')) {
      sendCommand('ABORT')
    }
  }

  return (
    <div style={{ marginTop: 16 }}>
      <button
        onClick={handleAbort}
        style={{
          background: '#c00',
          color: '#fff',
          border: '3px solid #f00',
          borderRadius: 8,
          padding: '20px 60px',
          fontSize: 28,
          fontWeight: 'bold',
          cursor: 'pointer',
          letterSpacing: 4,
        }}
      >
        ⚠ ABORT
      </button>
    </div>
  )
}
