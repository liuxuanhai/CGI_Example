/******************************************    *******************************

      > File Name: file-test.js

      > Author: Ywl

      > Descripsion:

      > Created Time:     Wed 14 Mar 2018 06:23:43 PM PDT

      > Modify Time: 

 *********************************    ***************************************/
var fs = require('fs');

fs.readFile('fs.txt', 'utf-8', function(err, data){
    if(err){
        console.error(err);
    }
    else if(data){
        console.log(data);
    }
});
console.log('end');     /* print 'end' before 'err' or 'data' */
