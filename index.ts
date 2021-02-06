import * as fs from 'fs'

let native
function getNative () {
  if (!native) {
    native = require('./build/Release/native-process-working-directory.node')
  }
  return native
}

export function getWorkingDirectoryFromPID (pid: number): string|null {
  if (process.platform === 'linux') {
    return fs.readlinkSync(`/proc/${pid}/cwd`)
  }
  return getNative().getWorkingDirectoryFromPID(pid)
}

export function getWorkingDirectoryFromHandle (handle: number): string|null {
  if (process.platform !== 'win32') {
    throw new Error('getWorkingDirectoryFromHandle() is only available on Windows')
  }
  return getNative().getWorkingDirectoryFromHandle(handle)
}
