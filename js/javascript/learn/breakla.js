/******************************************    *******************************

      > File Name: breakla.js

      > Author: Ywl

      > Descripsion:

      > Created Time:     Thu 15 Mar 2018 12:24:51 AM PDT

      > Modify Time: 

 *********************************    ***************************************/
var a = 10;

labl1:{
    console.log('in');
    if(a > 3)
    {
        break labl1;
    }
    console.log('ok');
}
console.log('out');
