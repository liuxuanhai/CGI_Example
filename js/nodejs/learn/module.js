/******************************************    *******************************

      > File Name: module.js

      > Author: Ywl

      > Descripsion:

      > Created Time:     Wed 14 Mar 2018 06:56:43 PM PDT

      > Modify Time: 

 *********************************    ***************************************/
var name;

exports.setName = function(theName){
    name = theName;
};

exports.sayHello = function(){
    console.log('good bye ' + name);
};

/*
single.js
function Hello(){
    var name;
    this.setName = function(theName){
        name = theName;
    };

    this.sayHello = function(){
        console.log('good bye ' + name);
    };
}

exports.Hello = Hello;  //ouTput is exports

need use : require('./single').Hello;

hello.js

function Hello(){
    var name;
    this.setName = function(theName){
        name = theName;
    };

    this.sayHello = function(){
        console.log('good bye ' + name);
    };
}

module.exports = Hello;     //ouTput is module.exports

need use : require('./hello')
*/
