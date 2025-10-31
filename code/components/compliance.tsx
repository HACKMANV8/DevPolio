"use client"

import { useEffect, useRef, useState } from "react"
import { Shield, CheckCircle2 } from "lucide-react"

export function Compliance() {
  const [isVisible, setIsVisible] = useState(false)
  const sectionRef = useRef<HTMLDivElement>(null)

  useEffect(() => {
    const observer = new IntersectionObserver(
      ([entry]) => {
        if (entry.isIntersecting) {
          setIsVisible(true)
        }
      },
      { threshold: 0.3 },
    )

    if (sectionRef.current) {
      observer.observe(sectionRef.current)
    }

    return () => observer.disconnect()
  }, [])

  return (
    <section ref={sectionRef} className="relative py-20 px-4 overflow-hidden">
      <div className="absolute inset-0 bg-gradient-to-t from-purple-950/30 via-slate-950 to-slate-950" />

      <div className="relative z-10 max-w-4xl mx-auto">
        <div
          className={`rounded-xl border border-purple-500/30 bg-gradient-to-br from-purple-900/20 to-slate-900/20 backdrop-blur-sm p-12 neon-border transition-all duration-700 ${
            isVisible ? "opacity-100 scale-100" : "opacity-0 scale-95"
          }`}
        >
          <div className="flex flex-col md:flex-row items-start gap-8">
            {/* Left: Icon & Title */}
            <div className="flex-shrink-0">
              <div className="p-4 rounded-lg bg-purple-500/10 mb-6">
                <Shield className="w-12 h-12 text-purple-400" />
                <h2 className="text-3xl font-bold text-white glow-text mb-4">Security & Compliance</h2>
              </div>
              
            </div>

            {/* Right: Compliance Info */}
            <div className="flex-1 space-y-4">
              <div className="flex items-start gap-4">
                <CheckCircle2 className="w-6 h-6 text-purple-400 flex-shrink-0 mt-1" />
                <div>
                  <p className="text-white font-semibold mb-1">NIST SP 800-88 Rev.1 Compliant</p>
                  <p className="text-slate-300">
                    Meets federal standards for secure data erasure and digital forensics
                  </p>
                </div>
              </div>

              <div className="flex items-start gap-4">
                <CheckCircle2 className="w-6 h-6 text-purple-400 flex-shrink-0 mt-1" />
                <div>
                  <p className="text-white font-semibold mb-1">Tamper-Proof Certificates</p>
                  <p className="text-slate-300">Generates verifiable wipe certificates for compliance audits</p>
                </div>
              </div>

              <div className="flex items-start gap-4">
                <CheckCircle2 className="w-6 h-6 text-purple-400 flex-shrink-0 mt-1" />
                <div>
                  <p className="text-white font-semibold mb-1">Circular Economy Support</p>
                  <p className="text-slate-300">
                    Enables sustainable device reuse aligned with India's environmental goals
                  </p>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </section>
  )
}
