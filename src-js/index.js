const addon = require('../build/Release/demo').DemoClass;
const welcome = require('./hi');

const demo = new addon(require)

console.log(demo)

console.log(Object.keys(demo).map(el => ({[el]: typeof demo[el]})));

console.log(demo.sayHi())

/*


console.log('c++', demo.hello());

console.log('c++', demo.add(10.25, 2));

console.log(welcome(), typeof welcome)

console.log('NAPI Internals:');
demo.listInternals();

console.log('c++', demo.callRequire(require))*/
