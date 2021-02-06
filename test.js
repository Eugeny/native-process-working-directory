var assert = require('assert');
var fs = require('fs');
var mod = require('.');

describe('#native-process-working-directory()', () => {
  it('should read cwd', () => {
    assert.equal(
      fs.realpathSync(mod.getWorkingDirectoryFromPID(process.pid)),
      process.cwd(),
    );
    process.chdir('src');
    assert.equal(
      fs.realpathSync(mod.getWorkingDirectoryFromPID(process.pid)),
      process.cwd(),
    );
    process.chdir('..');
    assert.equal(
      fs.realpathSync(mod.getWorkingDirectoryFromPID(process.pid)),
      process.cwd(),
    );
  });
});
