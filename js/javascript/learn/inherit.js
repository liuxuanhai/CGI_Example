/******************************************    *******************************

      > File Name: inherit.js

      > Author: Ywl

      > Descripsion:

      > Created Time:     Mon 23 Apr 2018 10:51:34 PM PDT

      > Modify Time: 

 *********************************    ***************************************/
/* 几种继承，js没有实现继承，只支持继承 */

/* 原型链继承 */
function Person(){  //father
    this.name = true;
}

function New_person(){  //son
    this.age = false;
}
//给父类原型对象加方法
Person.prototype.getname = function(){
    return this.name;
};

New_person.prototype = new Person();    /*将父类的一个实例给子类的原型对象，
                                        那么子类的原型对象指向了父类的一个实例，
                                        而示例中包含name属性(之后这个属性成为了子类原型对象的属性)
                                        和父类的原型对象包含getname方法*/
New_person.prototype.getage = function(){//给子类原型对象添加方法
    return this.age;
};

/* 也是由弊端的，父类的属性会被继承到子类的原型对象中 */
var new_person = new New_person();
/*
 * new_person = age,
 *              new_person.prototype = getage,
 *                                      Person.name,
 *                                      Person.prototype = getname
 *
 * */
console.log(new_person.getname() + ' ' + new_person.getage());
console.log(new_person.getname + ' ');    //不是函数式调用直接输出源码?, 这里是转换为字符串了


/* 组合继承,缺陷是会调用两次父类 */
function Tools(name){
    this.name = name;
}
Tools.prototype.print_name = function(){
    return this.name;
};
function Pen(name, price){
    //第二次调用父类
    Tools.call(this, name);    //将父类的属性当作了自己的属性
    this.price = price;
}

//第一次调用父类
Pen.prototype = new Tools();    /*原型对象为父类实例，父类中的属性会被子类中的属性屏蔽
                                (因为调用构造函数的时候重父类哪里得到了属性名相同的属性)
                                其实name属性由两组
                                */
Pen.prototype.constructor = Pen;

Pen.prototype.print_price = function(){
    return this.price;
};
var pen1 = new Pen('pen', 1.5);
console.log('组合继承: ' + pen1.print_name());
console.log('组合继承: ' + pen1.print_price());

/* 寄生组合继承,思想就是只得到父类的原型对象 */
/* 改写64~68行 */
/*
 * var prorotype = Object(Tools.prototype);
 * prototype.constructor = Pen;
 * Pen.prototype = prototype;
 *
 * */
