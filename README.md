# native-process-working-directory
![Node CI](https://github.com/Eugeny/native-process-working-directory/workflows/Node%20CI/badge.svg)

Gets another process' working directory, natively, without parsing `lsof` output or whatnot.

```ts
export function getWorkingDirectoryFromPID (pid: number): string|null

// Windows only
export function getWorkingDirectoryFromHandle (handle: number): string|null
```

Note: `getWorkingDirectoryFromPID()` on Windows might require administrator privileges, use `getWorkingDirectoryFromHandle()` if you own the process.
