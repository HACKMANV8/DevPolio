import { Hero } from "@/components/hero"
import { Features } from "@/components/features"
import { Compliance } from "@/components/compliance"
import { Footer } from "@/components/footer"

export default function Home() {
  return (
    <main className="w-full overflow-x-hidden">
      <Hero />
      <Features />
      <Compliance />
      <Footer />
    </main>
  )
}
