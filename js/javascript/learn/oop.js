/******************************************    *******************************

      > File Name: oop.js

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sun 22 Apr 2018 07:53:23 PM PDT

      > Modify Time: 

 *********************************    ***************************************/
var person = {
    name : 'ywl',
    age : 23,
    job : 'soft eg',
    _sener : 'xx',
    print_name : function(){
        return this.name;
    }
};

Object.defineProperty(person, 'sener', {
    get:function(){
        return this._sener;
    },
});

console.log(person['age']);
console.log(person['sener']);
console.log(person['print_name']());

//define a object , this指针指向执行环境，全局调用时指向window对象，实例化对象调用时指向对象
function Person(name, age, job){
    this.name = name;
    this.age = age;
    this.job = job;
    this.print_name = function(){
        return this.name;
    };
}

var person1 = new Person('yuwangliang', 22, 'software');
console.log(person1.print_name());
console.log(person1.constructor);   //查看构造函数


//当作一个普通函数使用
Person('astralrovers', 23, 'manager');
//console.log(window.print_name()); //astralrovers, nodejs中无法使用window对象
var o = Object();       //使用new的弊端，每个实例都有一个print_name()函数，其地址时不一样的

Person.call(o, 'errsky', 23, 'google');     //调用Person函数，调用时this指向o对象
console.log(o.print_name());

function cal_factorial(number){
    if(number <= 1)
        return 1;
    else
        return number * arguments.callee(number-1);     // == number * cal_factorial(number - 1);好处在于如果函数名被修改了函数功能不变
}
console.log(cal_factorial(20));

var back_fun = cal_factorial;
console.log(back_fun(5));

cal_factorial = function(){     //这里要注意函数名也是一个对象变量，表示一个函数的地址,重新指向新的函数时原来的函数依然存在
    console.log('not using!');
}
cal_factorial();
