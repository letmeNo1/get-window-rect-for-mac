const { getWindowRect } = require('bindings')('main.node')

console.log(getWindowRect(3813))     // 3
