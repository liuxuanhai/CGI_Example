from django.db import models

# Create your models here.
#这里是创建两个模型，即数据库
class Poll(models.Model):
    #问题
    question = models.CharField(max_length=200) #变量名作为Field的实例，也是表的属性名，char是数据类型
    #时间
    pub_date = models.DateTimeField('date published')
    def __unicode__(self):
        return self.question

class Choice(models.Model):
    poll = models.ForeignKey(Poll, on_delete=models.CASCADE)#这里的作用是关联，即一个模型关联另一个模型,(一个问题，时间对应有问题内容和票数)
    #文本描述
    choice_text = models.CharField(max_length=200)
    #票数
    votes = models.IntegerField(default=0)
    def __unicode__(self):
        return self.choice_text

