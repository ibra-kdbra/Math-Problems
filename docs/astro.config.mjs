import react from "@astrojs/react";
import starlight from "@astrojs/starlight";
import tailwindcss from "@tailwindcss/vite";
import { defineConfig } from "astro/config";
import mdx from "@astrojs/mdx";

export default defineConfig({
  site: "https://docs-math.vercel.app",
  integrations: [
    starlight({
      title: "starmint",
      logo: {
        light: "./src/assets/dark-logo.svg",
        dark: "./src/assets/light-logo.svg",
      },
      customCss: ["./src/styles/tailwind.css"],
      components: {
        Header: "./src/components/Header.astro",
      },
      sidebar: [
        {
          label: "Stage 1: The Foundations",
          items: [
            { label: "Getting Started", autogenerate: { directory: "getting-started" } },
            { label: "Number Theory", autogenerate: { directory: "number-theory" } },
            { label: "Equations Field", autogenerate: { directory: "equations-field" } },
          ],
        },
        {
          label: "Stage 2: Algorithms & Logic",
          items: [
            { label: "Sorting Algorithms", autogenerate: { directory: "sorting-algorithms" } },
            { label: "Unique Algorithms", autogenerate: { directory: "unique-algorithms" } },
            { label: "Sparse Matrix", autogenerate: { directory: "sparse-matrix" } },
            { label: "Encoder Decoder", autogenerate: { directory: "encoder-decoder" } },
            { label: "Security Algorithms", autogenerate: { directory: "security-algorithms" } },
          ],
        },
        {
          label: "Stage 3: The Edge of Chaos",
          items: [
            { label: "Mandlebrot", autogenerate: { directory: "mandlebrot" } },
            { label: "Math Chaos", autogenerate: { directory: "math-chaos" } },
          ],
        },
        {
          label: "Stage 4: Simulating Life",
          items: [
            { label: "PySpace", autogenerate: { directory: "pyspace" } },
          ],
        },
      ],
    }),
    mdx(),
    react(),
  ],
  vite: {
    plugins: [tailwindcss()],
  },
});
