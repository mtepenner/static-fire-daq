import { useDaqStream } from './hooks/useDaqStream'
import HighSpeedChart from './components/HighSpeedChart'
import ValveStatusHUD from './components/ValveStatusHUD'
import AbortPanel from './components/AbortPanel'

export default function App() {
  const { data, connected, sendCommand } = useDaqStream()
  const latest = data[data.length - 1]

  return (
    <div style={{ fontFamily: 'monospace', padding: 16, background: '#111', color: '#eee', minHeight: '100vh' }}>
      <h1 style={{ margin: '0 0 8px' }}>Static Fire DAQ</h1>
      <div style={{ marginBottom: 8 }}>
        Status:{' '}
        <span style={{ color: connected ? '#0f0' : '#f00' }}>
          {connected ? 'CONNECTED' : 'DISCONNECTED'}
        </span>
      </div>
      {latest && (
        <div style={{ marginBottom: 8 }}>
          T+{(latest.timestamp_ms / 1000).toFixed(3)}s &nbsp;
          P: {latest.pressure_psi.toFixed(1)} PSI &nbsp;
          F: {latest.thrust_n.toFixed(1)} N
        </div>
      )}
      <HighSpeedChart data={data} />
      <ValveStatusHUD valveState={latest?.valve_state ?? 0} />
      <AbortPanel sendCommand={sendCommand} />
    </div>
  )
}
