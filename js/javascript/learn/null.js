/******************************************    *******************************

      > File Name: null.js

      > Author: Ywl

      > Descripsion:

      > Created Time:     Tue 20 Mar 2018 08:15:31 PM PDT

      > Modify Time: 

 *********************************    ***************************************/

console.log(typeof null);
//alert(typeof null);
var num = 0;
for ( var property in null ){
    num++;
}
function setObject(){
    this.number = 2;
    this.age = 1;
    //return this.number;
}
console.log(num);
var first = new setObject();
var second = new setObject();
console.log(first.number);
first.name = 'ywl';
console.log(first.name);

for ( var property in setObject ){
    console.log(property);
}
console.log('\n');
for ( var property in first ){
    console.log(property + ' = ' + first[property]);
}

/* 删除属性 */
delete first.name;
console.log((first.number));
console.log((second.number));

delete first.constructor.prototype.number;
console.log((first.number));
console.log((second.number));
