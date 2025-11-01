"use client"

import { useEffect, useState } from "react"
import { Download } from "lucide-react"

export function Hero() {
  const [isLoaded, setIsLoaded] = useState(false)

  useEffect(() => {
    setIsLoaded(true)
  }, [])

  return (
    <section className="relative min-h-screen flex items-center justify-center overflow-hidden px-4 py-20">
      <div className="absolute inset-0 bg-gradient-to-br from-purple-950 via-slate-950 to-slate-900" />

      {/* Animated background elements */}
      <div className="absolute top-20 left-10 w-72 h-72 bg-purple-600/10 rounded-full blur-3xl animate-pulse" />
      <div className="absolute bottom-20 right-10 w-96 h-96 bg-purple-500/5 rounded-full blur-3xl animate-pulse delay-1000" />

      <div className="relative z-10 max-w-3xl text-center">
        {/* Label */}
        <div
          className={`mb-8 inline-block px-4 py-2 rounded-full border border-purple-500/30 bg-purple-900/20 text-sm font-medium text-purple-300 transition-all duration-700 ${
            isLoaded ? "opacity-100 scale-100" : "opacity-0 scale-95"
          }`}
          style={{
            animation: isLoaded ? "fade-in-up 0.6s ease-out 0.1s forwards" : "none",
          }}
        >
          Data Erasure
        </div>

        {/* Title */}
        <h1
          className="text-5xl md:text-7xl font-bold mb-6 text-balance text-white glow-text leading-tight"
          style={{
            animation: isLoaded ? "fade-in-up 0.6s ease-out 0.2s forwards" : "none",
            opacity: isLoaded ? 1 : 0,
          }}
        >
          Void
        </h1>

        {/* Tagline */}
        <p
          className="text-xl md:text-2xl font-semibold mb-4 text-purple-300 text-balance"
          style={{
            animation: isLoaded ? "fade-in-up 0.6s ease-out 0.3s forwards" : "none",
            opacity: isLoaded ? 1 : 0,
          }}
        >
          Secure. Compliant. Irreversible Data Erasure.
        </p>

        {/* Description */}
        <p
          className="text-lg text-slate-300 mb-10 text-balance leading-relaxed max-w-2xl mx-auto"
          style={{
            animation: isLoaded ? "fade-in-up 0.6s ease-out 0.4s forwards" : "none",
            opacity: isLoaded ? 1 : 0,
          }}
        >
          A cross-platform data wiping solution for Windows, Linux, and Android ensuring NIST SP 800-88
          compliance and supporting India's circular economy.
        </p>

        {/* CTA Button */}
        <div className="flex flex-col sm:flex-row justify-center gap-4"
          style={{
            animation: isLoaded ? "fade-in-up 0.6s ease-out 0.5s forwards" : "none",
            opacity: isLoaded ? 1 : 0,
          }}
        >
          <a
            href="https://drive.google.com/uc?export=download&id=1tjpmqRFmL5yHBPS9IxegDvbm3RQBDBXs"
            className="inline-flex items-center gap-2 px-8 py-4 bg-gradient-to-r from-purple-600 to-purple-700 text-white font-semibold rounded-lg hover:from-purple-500 hover:to-purple-600 transition-all duration-300 glow-pulse hover:shadow-2xl hover:shadow-purple-500/50"
          >
            <Download className="w-5 h-5" />
            Download ISO
          </a>
          <a
            href="https://drive.google.com/uc?export=download&id=1mHMUd8OD9wMa-LwUIyE3s9FpevuHfC0s"
            className="inline-flex items-center gap-2 px-8 py-4 bg-gradient-to-r from-purple-600 to-purple-700 text-white font-semibold rounded-lg hover:from-purple-500 hover:to-purple-600 transition-all duration-300 glow-pulse hover:shadow-2xl hover:shadow-purple-500/50"
          >
            <Download className="w-5 h-5" />
            Download Debian Package
          </a>
        </div>
      </div>
    </section>
  )
}
