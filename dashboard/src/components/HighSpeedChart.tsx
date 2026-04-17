import { useEffect, useRef } from 'react'
import uPlot from 'uplot'
import 'uplot/dist/uPlot.min.css'
import type { TelemetryPacket } from '../hooks/useDaqStream'

interface Props {
  data: TelemetryPacket[]
}

export default function HighSpeedChart({ data }: Props) {
  const containerRef = useRef<HTMLDivElement>(null)
  const plotRef = useRef<uPlot | null>(null)

  useEffect(() => {
    if (!containerRef.current) return

    const opts: uPlot.Options = {
      title: 'Telemetry',
      width: 800,
      height: 300,
      series: [
        {},
        { label: 'Pressure (PSI)', stroke: '#0af', scale: 'psi' },
        { label: 'Thrust (N)', stroke: '#f80', scale: 'n' },
      ],
      axes: [
        {},
        { scale: 'psi', side: 1 },
        { scale: 'n', side: 3 },
      ],
    }

    plotRef.current = new uPlot(opts, [[], [], []], containerRef.current)
    return () => {
      plotRef.current?.destroy()
      plotRef.current = null
    }
  }, [])

  useEffect(() => {
    if (!plotRef.current || data.length === 0) return
    const times = data.map((d) => d.timestamp_ms / 1000)
    const pressures = data.map((d) => d.pressure_psi)
    const thrusts = data.map((d) => d.thrust_n)
    plotRef.current.setData([times, pressures, thrusts])
  }, [data])

  return <div ref={containerRef} style={{ marginBottom: 16 }} />
}
