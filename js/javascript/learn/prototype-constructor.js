/******************************************    *******************************

      > File Name: prototype-constructor.js

      > Author: Ywl

      > Descripsion:

      > Created Time:     Mon 23 Apr 2018 06:53:51 PM PDT

      > Modify Time: 

 *********************************    ***************************************/
/* 重写原型对象 */
function Person(){
    Person.prototype.name = 'javascript';
}

var person = new Person();
console.log('before rewrite : ' + person.name);

Person.prototype = {
    constructor : Person, //手动指定构造函数,原本构造函数中
    name : 'js',
    age : 23,
};
console.log('after rewrite : ' + person.name);
console.log('Person after rewrite : ' + Person.prototype.name);
var person2 = new Person();     //这里需要特别注意
console.log('new after rewrite : ' + person2.name); //打印出来的仍然是javascript,
/* 注意调用构造函数后的执行顺序
 * 调用构造函数后：
 * 1.创建一个新的对象
 * 2.作用域给对象，并且prortoty属性也会自动分配给新的对象，prototype是指向原型对象的，此时name=js,
 * 3.执行代码（添加新对象的属性），这里执行代码时将原型对象中的name修改为了javascript
 * 4.返回新对象
    * */
console.log('new after rewrite : ' + person2.age);
console.log('new after rewrite : ' + Person.prototype.age);
/*  
 * 重写原型对象切断了现有原型与任何之前已经存在的对象实例之间的联系；它
 * 们引用的仍然是最初的原型
 * 从指针的角度来解释，每个实例中的prototype属性与类中的prototype属性的值相同，其值都等于原本的原型对象的指针，
 * 在类的原型对象重写后，类的prototype的值等于新的原型对象的地址，而之前创建的实例的prototype的值仍然等于原笨的原型对象的地址
    *  */

Person.prototype.age = 22;
Person.prototype.sayName = function(){
    return this.name;
};
console.log(Person.prototype.name);
console.log(Person.prototype.age);
console.log(Person.prototype.sayName());

//console.log(hasPrototypeProperty(person, 'name'));
//组合使用构造和原型模式
//思想在于实例拥有各自的属性拷贝，但是方法共享
function Animal(name){
    this.name = name;
}
Animal.prototype = {
    consructor : Animal,
    print_name : function(){
        console.log(this.name);
    },
};
var animal1 = new Animal('cat');
var animal2 = new Animal('dog');
console.log('组合构造+原型');
animal1.print_name();
animal2.print_name();

//动态原型模式
function Bird(name){
    this.name = name;
    if(typeof this.print_name != 'function')//要确保不会被实例属性给屏蔽(同名屏蔽)
    {
        Bird.prototype.print_name = function(){
            console.log(this.name);
        };
    }
}
var bird1 = new Bird('bird');
console.log('动态原型');
bird1.print_name();
