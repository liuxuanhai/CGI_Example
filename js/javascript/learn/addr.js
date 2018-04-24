/******************************************    *******************************

      > File Name: addr.js

      > Author: Ywl

      > Descripsion:

      > Created Time:     Mon 12 Mar 2018 08:19:16 PM PDT

      > Modify Time: 

 *********************************    ***************************************/
var str = 'hello';              //val
var obj = new String(str);      //object、array、regexp、function have address

function newToString(){
    return 'hello world';
}
function func(val){
    val.toString = newToString;
}

func(str);
console.log(str);

func(obj);
console.log(obj);
console.log(obj.toString());
console.log(String(obj));


function send_value(obj){
    obj.name = 'yuan';          //
    obj = new Object();         //局部变量的值等于另一个对象实例的地址,而传入的值（地址）没有发生变化，这里很好解释了js传参只传值不传引用，否则的话这一步的操作会导致g_obj所代表的地址已经改变了
    obj.name = 'pan';
}

g_obj = new Object();           //这里的g_obj等于创建的object对象实例的地址
g_obj.name = 'yu';
send_value(g_obj);              //js传参数只传变量的值,g_obj所代表的变量值本身就是一个地址
console.log(g_obj.name);
