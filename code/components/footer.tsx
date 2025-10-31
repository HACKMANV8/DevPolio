import { Github } from "lucide-react"

export function Footer() {
  return (
    <footer className="relative py-12 px-4 border-t border-purple-500/20 bg-gradient-to-r from-slate-950 via-purple-950/10 to-slate-950">
      <div className="max-w-6xl mx-auto">
        <div className="flex flex-col md:flex-row items-center justify-between gap-8">
          {/* Copyright */}
          <div className="text-center md:text-left">
            <p className="text-slate-400 text-sm">© 2025 Void. All rights reserved.</p>
          </div>

          {/* Links & Social */}
          <div className="flex items-center gap-6">
            <a href="#" className="text-slate-400 hover:text-purple-400 transition-colors flex items-center gap-2">
              <Github className="w-5 h-5" />
              <span className="text-sm">GitHub</span>
            </a>
          </div>
        </div>

        {/* Divider */}
        <div className="mt-8 pt-8 border-t border-purple-500/10">
          <p className="text-xs text-slate-500 text-center">
            Built with security at the core. Open-source and transparent.
          </p>
        </div>
      </div>
    </footer>
  )
}
