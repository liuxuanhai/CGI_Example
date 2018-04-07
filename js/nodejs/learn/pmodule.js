/******************************************    *******************************

      > File Name: pmodule.js

      > Author: Ywl

      > Descripsion:

      > Created Time:     Wed 14 Mar 2018 06:38:19 PM PDT

      > Modify Time: 

 *********************************    ***************************************/
function Module(id, parent) {
  this.id = id;
  this.exports = {};
  this.parent = parent;
  this.filename = null;
  this.loaded = false;
  this.children = [];
}

module.exports = Module;

var module = new Module(filename, parent);

console.log('module.id: ', module.id);
console.log('module.exports: ', module.exports);
console.log('module.parent: ', module.parent);
console.log('module.filename: ', module.filename);
console.log('module.loaded: ', module.loaded);
console.log('module.children: ', module.children);
console.log('module.paths: ', module.paths);
