const addon = require('../build/Release/demo');
const welcome = require('./hi');

const demo = new addon.DemoClass(require);

console.log('NAPI Internals:');
addon.listInternals();

console.log(Object.keys(demo).map(el => ({[el]: typeof demo[el]})));

console.log(demo.sayHi());

console.log('c++', addon.hello());

console.log('c++', addon.add(10.25, 2));

console.log(welcome(), typeof welcome);

console.log('c++', addon.callRequire(require));
