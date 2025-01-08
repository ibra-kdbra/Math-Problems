import mdx from '@astrojs/mdx'
import react from '@astrojs/react'
import tailwind from '@astrojs/tailwind'
import vercel from '@astrojs/vercel/serverless'
import { defineConfig } from 'astro/config'
import astroDocs from './integrations/astroDocs'

export default defineConfig({
  output: 'hybrid',
  adapter: vercel({
    webAnalytics: {
      enabled: true,
    },
  }),
  prefetch: true,
  integrations: [
    react(),
    tailwind(),
    mdx(),
    astroDocs({
      anchors: [
        {
          name: 'Community',
          icon: 'discord',
          url: '/',
        },
        {
          name: 'Blog',
          icon: 'newspaper',
          url: 'https://banije.vercel.app/',
        },
      ],
      navigation: [
        {
          group: 'Getting Started',
          type: 'getting-started',
          pages: [
            { name: 'Introduction', url: '/' },
            { name: 'Quickstart', url: '/getting-started/quickstart' },
            { name: 'Creating pages', url: '/getting-started/creating-pages' },
            { name: 'Components', url: '/getting-started/components' },
            { name: 'Deploy', url: '/getting-started/deploy' },

          ],
        },
        {
          group: 'Differentiator',
          type: 'differentiator',
          pages: [
            { name: 'Differentiator-readme', url: '/differentiator/differentiator-readme' },

          ],
        },
      ],
    }),
  ],
  markdown: {
    shikiConfig: {
      theme: 'dracula',
      wrap: true,
    },
  },
  content: {
    collections: ['getting-started', 'essentials', 'differentiator'],
  },
})
