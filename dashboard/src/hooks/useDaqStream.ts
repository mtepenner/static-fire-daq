import { useEffect, useRef, useState, useCallback } from 'react'

export interface TelemetryPacket {
  timestamp_ms: number
  pressure_psi: number
  thrust_n: number
  valve_state: number
}

const MAX_BUFFER = 1000

export function useDaqStream() {
  const [data, setData] = useState<TelemetryPacket[]>([])
  const [connected, setConnected] = useState(false)
  const wsRef = useRef<WebSocket | null>(null)

  useEffect(() => {
    function connect() {
      const ws = new WebSocket('ws://localhost:8080/ws')
      wsRef.current = ws

      ws.onopen = () => setConnected(true)
      ws.onclose = () => {
        setConnected(false)
        setTimeout(connect, 2000)
      }
      ws.onerror = () => ws.close()
      ws.onmessage = (event) => {
        try {
          const pkt: TelemetryPacket = JSON.parse(event.data as string)
          setData((prev) => {
            const next = [...prev, pkt]
            return next.length > MAX_BUFFER ? next.slice(next.length - MAX_BUFFER) : next
          })
        } catch {
          // ignore malformed packets
        }
      }
    }
    connect()
    return () => {
      wsRef.current?.close()
    }
  }, [])

  const sendCommand = useCallback((cmd: string) => {
    if (wsRef.current?.readyState === WebSocket.OPEN) {
      wsRef.current.send(cmd)
    }
  }, [])

  return { data, connected, sendCommand }
}
