import { icons } from 'lucide-react'
import { memo } from 'react'

import { type ClassValue, clsx } from 'clsx'
import { twMerge } from 'tailwind-merge'

export function cn(...inputs: ClassValue[]) {
  return twMerge(clsx(inputs))
}

export type IconProps = {
  name: keyof typeof icons
  className?: string
  strokeWidth?: number
}

export const Icon = memo(({ name, className, strokeWidth }: IconProps) => {
  const IconComponent = icons[name]

  if (!IconComponent) {
    return null
  }

  return (
    <IconComponent
      className={cn('w-4 h-4', className)}
      strokeWidth={strokeWidth || 2.5}
    />
  )
})

Icon.displayName = 'Icon'
