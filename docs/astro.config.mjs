import react from '@astrojs/react'
import starlight from '@astrojs/starlight'
import tailwind from '@astrojs/tailwind'
import { defineConfig } from 'astro/config'

export default defineConfig({
  integrations: [
    starlight({
      title: 'astrodocs',
      logo: {
        light: './src/assets/dark-logo.svg',
        dark: './src/assets/light-logo.svg',
      },
      customCss: ['./src/styles/custom.css', './src/styles/tailwind.css'],
      social: {
        github: 'https://github.com/withastro/starlight',
      },
      components: {
        Header: './src/components/Header.astro',
      },
      sidebar: [
        {
          label: 'Getting Started',
          items: [
            {
              label: 'Introduction',
              slug: 'getting-started/introduction',
            },
            {
              label: 'Quickstart',
              slug: 'getting-started/quickstart',
            },
          ],
        },
        {
          label: 'Equation Field',
          items: [
            {
              label: 'Differentiator',
              slug: 'equations-field/differentiator',
            },
            {
              label: 'Equa Plot',
              slug: 'equations-field/equa-plot',
            },
          ],
        },
        {
          label: 'Encoder Decoder',
          items: [
            {
              label: 'DnsLib',
              slug: 'encoder-decoder/dnslib',
            },
            {
              label: 'Image encoding',
              slug: 'encoder-decoder/img-encoding',
            },
          ],
        },
        {
          label: 'Mandlebrot',
          items: [
            {
              label: 'Fractal Gh',
              slug: 'mandlebrot/fractal-gh',
            },
            {
              label: 'Mandlebrot Coloring',
              slug: 'mandlebrot/coloring',
            },
          ],
        },
      ],
    }),
    tailwind(),
    react(),
  ],
})
