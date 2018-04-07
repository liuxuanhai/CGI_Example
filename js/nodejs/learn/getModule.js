/******************************************    *******************************

      > File Name: getModule.js

      > Author: Ywl

      > Descripsion:

      > Created Time:     Wed 14 Mar 2018 06:58:55 PM PDT

      > Modify Time: 

 *********************************    ***************************************/
var getmodule = require('./module');

getmodule.setName('Ywl');
getmodule.sayHello();

var getmodule2 = require('./module');

getmodule2.setName('Ywl');
getmodule.sayHello();       /* print ? 类似于创建实例，但又不同，这里两个变量指向同一个实例，输出由后者决定 */
