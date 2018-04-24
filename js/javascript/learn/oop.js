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


function New_person(){
    New_person.prototype.name = 'person';       //prototype是每个函数都有的属性，也是一个指针，指向一个对象，这个对象包含所有实例共享的属性和方法
    New_person.prototype.age = 23;              //也就是说创建的所有实例共享属性和方法
    New_person.prototype.print_name = function(){
        return this.name;
    };
}
//所有属性和方法都被添加到了prototype属性中，构造函数是空函数

person2 = new New_person();
person3 = new New_person();

console.log(person2.print_name());
console.log('has own name : ' + person2.hasOwnProperty('name'));
person2.name = 'change';
console.log('add object property : ' + person2.print_name());
console.log('has own name : ' + person2.hasOwnProperty('name'));
console.log(person3.print_name());

console.log(person3.print_name());//既然共享属性和方法，那么为什么person2修改了，person3还是原来的值
//上述中，如果实例重写了与原型对象属性相同名称的变量，实际上是在示例中创建了一个属性，访问时原型对象的会被屏蔽，
New_person.prototype.name = 'one-plus';
console.log('modify prototype : ' + person3.print_name());
delete person2.name;
console.log('after delete : ' + person2.print_name());

console.log(New_person.prototype.isPrototypeOf(person2));//查看实例与原型对象是否存在关联
console.log(New_person.prototype.isPrototypeOf(person3));

console.log('get prototype : ' + Object.getPrototypeOf(person2));
console.log(Object.getPrototypeOf(person2).name);//查看原型对象的name值

console.log("person2 : " + Object.keys(person2));   //查看所有可遍历属性，同for-in
console.log("person2 : " + Object.keys(New_person.prototype));
