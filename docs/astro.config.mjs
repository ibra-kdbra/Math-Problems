import react from '@astrojs/react'
import starlight from '@astrojs/starlight'
import tailwind from '@astrojs/tailwind'
import { defineConfig } from 'astro/config'

export default defineConfig({
  integrations: [
    starlight({
      title: 'starmint',
      logo: {
        light: './src/assets/dark-logo.svg',
        dark: './src/assets/light-logo.svg',
      },
      customCss: ['./src/styles/custom.css', './src/styles/tailwind.css'],
      components: {
        Header: './src/components/Header.astro',
      },
      sidebar: [{ label: "Getting Started", autogenerate: { directory: "getting-started" } },
      { label: "Equations Field", autogenerate: { directory: "equations-field" } },
      { label: "Encoder Decoder", autogenerate: { directory: "encoder-decoder" } },
      { label: "Mandlebrot", autogenerate: { directory: "mandlebrot" } },
      { label: "Math Chaos", autogenerate: { directory: "math-chaos" } },
      ],
    }),
    tailwind(),
    react(),
  ],
})
