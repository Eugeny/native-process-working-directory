import * as fs from 'fs'

let native
function getNative () {
  if (!native) {
    native = require('./build/Release/native-process-working-directory.node')
  }
  return native
}

function cleanupPath (path: string) {
  if (!path) {
    return null
  }
  if (path.charCodeAt(path.length - 1) < 32) {
    path = path.substring(0, path.length - 1)
  }
  return path
}

export function getWorkingDirectoryFromPID (pid: number): string|null {
  if (process.platform === 'linux') {
    return fs.readlinkSync(`/proc/${pid}/cwd`)
  }
  return cleanupPath(getNative().getWorkingDirectoryFromPID(pid))
}

export function getWorkingDirectoryFromHandle (handle: number): string|null {
  if (process.platform !== 'win32') {
    throw new Error('getWorkingDirectoryFromHandle() is only available on Windows')
  }
  return cleanupPath(getNative().getWorkingDirectoryFromHandle(handle))
}
