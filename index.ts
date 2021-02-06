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
  let result = getNative().getWorkingDirectoryFromPID(pid)
  if (result) {
    result = fs.realpathSync(result)
  }
  return result
}

export function getWorkingDirectoryFromHandle (handle: number): string|null {
  if (process.platform !== 'win32') {
    throw new Error('getWorkingDirectoryFromHandle() is only available on Windows')
  }
  let result = getNative().getWorkingDirectoryFromHandle(handle)
  if (result) {
    result = fs.realpathSync(result)
  }
  return result
}
