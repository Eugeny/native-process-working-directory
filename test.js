var assert = require('assert');
var mod = require('.');

describe('#native-process-working-directory()', () => {
  it('should read cwd', () => {
    assert.equal(
      mod.getWorkingDirectoryFromPID(process.pid),
      process.cwd(),
    );
    process.chdir('src');
    assert.equal(
      mod.getWorkingDirectoryFromPID(process.pid),
      process.cwd(),
    );
    process.chdir('..');
    assert.equal(
      mod.getWorkingDirectoryFromPID(process.pid),
      process.cwd(),
    );
  });
});
