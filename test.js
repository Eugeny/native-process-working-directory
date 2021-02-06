#!/usr/bin/env node
const { getWorkingDirectoryFromPID } = require('./build/Release/native-process-working-directory.node')
console.log(getWorkingDirectoryFromPID(parseInt(process.argv[2])))
