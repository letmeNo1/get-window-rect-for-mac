const { getWindowRect } = require('bindings')('main.node')

console.log(getWindowRect(0))     // 3
