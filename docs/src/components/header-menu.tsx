import {
  DropdownMenu,
  DropdownMenuContent, 
  DropdownMenuItem,
  DropdownMenuTrigger,
 } from "./dropdown-menu"
 import { MoreVertical } from "lucide-react"
 
 export function HeaderMenu() {
  return (
    <DropdownMenu>
      <DropdownMenuTrigger>
        <MoreVertical className="h-4 w-4 text-muted-foreground" />
      </DropdownMenuTrigger>
      <DropdownMenuContent align="end">
        <DropdownMenuItem asChild>
          <a 
            href="https://github.com/ibra-kdbra/Math_Problems/issues" 
            target="_blank" 
            rel="noopener noreferrer"
          >
            Support
          </a>
        </DropdownMenuItem>
        <DropdownMenuItem asChild>
          <a 
            href="https://github.com/ibra-kdbra/Math-Problems/tree/main/docs" 
            target="_blank" 
            rel="noopener noreferrer"
          >
            GitHub
          </a>
        </DropdownMenuItem>
      </DropdownMenuContent>
    </DropdownMenu>
  )
 }