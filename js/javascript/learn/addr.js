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
