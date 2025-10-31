"use client"

import { useEffect, useRef, useState } from "react"
import { HardDrive, Zap, Smartphone, Trash2 } from "lucide-react"

const features = [
  {
    icon: HardDrive,
    title: "ATA Secure Erase for HDD/SATA",
    description: "Military-grade secure deletion for traditional hard drives",
  },
  {
    icon: Zap,
    title: "NVMe Cryptographic Key Erase",
    description: "Instant erase for modern NVMe SSDs via key destruction",
  },
  {
    icon: Smartphone,
    title: "ADB + Fastboot Android Wipe",
    description: "Complete device reset for Android devices",
  },
  {
    icon: Trash2,
    title: "Filesystem Signature Erase for USB",
    description: "Secure deletion of USB storage and external drives",
  },
]

export function Features() {
  const [visibleItems, setVisibleItems] = useState<number[]>([])
  const containerRef = useRef<HTMLDivElement>(null)

  useEffect(() => {
    const observer = new IntersectionObserver(
      (entries) => {
        entries.forEach((entry) => {
          if (entry.isIntersecting) {
            const index = Array.from(containerRef.current?.children || []).indexOf(entry.target as Element)
            setVisibleItems((prev) => [...new Set([...prev, index])])
          }
        })
      },
      { threshold: 0.1 },
    )

    containerRef.current?.children &&
      Array.from(containerRef.current.children).forEach((child) => observer.observe(child))

    return () => observer.disconnect()
  }, [])

  return (
    <section className="relative py-20 px-4 overflow-hidden">
      <div className="absolute inset-0 bg-gradient-to-b from-slate-950 via-purple-950/20 to-slate-950" />

      <div className="relative z-10 max-w-6xl mx-auto">
        {/* Section Title */}
        <div className="text-center mb-16">
          <h2 className="text-4xl md:text-5xl font-bold text-white mb-4 glow-text">Powerful Features</h2>
          <p className="text-lg text-slate-300">Enterprise-grade data erasure across all platforms</p>
        </div>

        {/* Features Grid */}
        <div ref={containerRef} className="grid grid-cols-1 md:grid-cols-2 gap-6">
          {features.map((feature, index) => {
            const Icon = feature.icon
            const isVisible = visibleItems.includes(index)

            return (
              <div
                key={index}
                className={`group p-8 rounded-xl border border-purple-500/20 bg-gradient-to-br from-purple-900/10 to-slate-900/10 backdrop-blur-sm hover:border-purple-500/50 hover:bg-gradient-to-br hover:from-purple-900/20 hover:to-slate-900/20 transition-all duration-500 cursor-pointer neon-border ${
                  isVisible ? "fade-in-up" : "opacity-0"
                }`}
                style={{
                  animation: isVisible ? `fade-in-up 0.6s ease-out ${index * 0.1}s forwards` : "none",
                }}
              >
                <div className="mb-4 inline-block p-3 rounded-lg bg-purple-500/10 group-hover:bg-purple-500/20 transition-colors">
                  <Icon className="w-6 h-6 text-purple-400" />
                </div>
                <h3 className="text-xl font-semibold text-white mb-2">{feature.title}</h3>
                <p className="text-slate-300 leading-relaxed">{feature.description}</p>
              </div>
            )
          })}
        </div>
      </div>
    </section>
  )
}
