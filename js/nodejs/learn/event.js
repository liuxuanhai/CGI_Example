/******************************************    *******************************

      > File Name: event.js

      > Author: Ywl

      > Descripsion:

      > Created Time:     Wed 14 Mar 2018 06:47:48 PM PDT

      > Modify Time: 

 *********************************    ***************************************/
var EventEmitter = require('events').EventEmitter;

var event = new EventEmitter();

event.on('some_event', function(){
    console.log('event print');
});

setTimeout(function(){
event.emit('some_event');
}, 1000);       /* 1ms */
