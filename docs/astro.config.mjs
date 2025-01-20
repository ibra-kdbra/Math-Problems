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
        Head: './src/components/Head.astro',
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
            {
              label: 'Creating Pages',
              slug: 'getting-started/creating-pages',
            },
            {
              label: 'Deployments',
              slug: 'getting-started/deployments',
            },
            {
              label: 'Theming',
              slug: 'getting-started/theming',
            },
            {
              label: 'Images',
              slug: 'getting-started/images',
            },
          ],
        },
        {
          label: 'Components',
          items: [
            {
              label: 'Badge',
              slug: 'components/badge',
            },
            {
              label: 'Asides',
              slug: 'components/asides',
            },
            {
              label: 'Tabs',
              slug: 'components/tabs',
            },
          ],
        },
        {
          label: 'Differentiator',
          items: [
            {
              label: 'Differentiator',
              slug: 'differentiator/differentiator-readme',
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
      ],
    }),
    tailwind(),
    react(),
  ],
})
