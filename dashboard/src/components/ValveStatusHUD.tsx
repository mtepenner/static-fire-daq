interface Props {
  valveState: number
}

function ValveIndicator({ label, open }: { label: string; open: boolean }) {
  return (
    <div style={{ display: 'flex', alignItems: 'center', gap: 8, marginRight: 24 }}>
      <div
        style={{
          width: 20,
          height: 20,
          borderRadius: '50%',
          background: open ? '#0f0' : '#f00',
          border: '2px solid #555',
        }}
      />
      <span>{label}</span>
    </div>
  )
}

export default function ValveStatusHUD({ valveState }: Props) {
  const ignitionOpen = !!(valveState & 0x01)
  const purgeOpen = !!(valveState & 0x02)
  const movOpen = !!(valveState & 0x04)

  return (
    <div style={{ display: 'flex', marginBottom: 16 }}>
      <ValveIndicator label="Ignition" open={ignitionOpen} />
      <ValveIndicator label="Purge" open={purgeOpen} />
      <ValveIndicator label="MOV" open={movOpen} />
    </div>
  )
}
