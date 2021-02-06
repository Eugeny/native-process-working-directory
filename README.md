# native-process-working-directory

```ts
export function getWorkingDirectoryFromPID (pid: number): string|null

// Windows only
export function getWorkingDirectoryFromHandle (handle: number): string|null
```

Note: `getWorkingDirectoryFromPID()` on Windows might require administrator priveleges, use `getWorkingDirectoryFromHandle()` if you own the process.
