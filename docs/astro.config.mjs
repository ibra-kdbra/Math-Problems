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
          url: '/',
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
          ],
        },
        {
          group: 'Differentiator',
          type: 'differentiator',
          pages: [
            { name: 'Readme', url: '/differentiator/readme' },

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
