/******************************************    *******************************

      > File Name: array.js

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sun 22 Apr 2018 01:14:00 PM CST

      > Modify Time: 

 *********************************    ***************************************/
var color = new Array(3);
console.log(color.length);


color[0] = 'red';
color[1] = 'green';
color[2] = 'yellow';

console.log(color.push('blue', 'gray'));    //栈
console.log(color.pop());

console.log(color.shift());
console.log(color.unshift('pure'));         //队列

console.log('\n');
color.length = 4;
for (var index in color){
    console.log(typeof index);
    console.log(color[index]);
}
console.log(Array.isArray(color));

//切片
console.log(color.slice(1, 3));     //起始到结束位置，只有起始位置默认到末尾
//splice(),插入，删除，替换:基本三个参数 == 起始位置，要删除的项数，要插入的数据

