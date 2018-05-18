#!/usr/bin/python3
# coding=utf-8

"""
#########################################################################################

      > File Name: schat.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Thu 17 May 2018 08:18:07 PM PDT

      > Modify Time: 

#########################################################################################
"""
from twisted.internet.protocol import Factory
from twisted.protocols.basic import LineReceiver
from twisted.internet import reactor

class Chat(LineReceiver):
    def __init__(self, users):
        self.users = users
        self.name = None
        self.state = 'GETNAME'

    def connectionMade(self):
        self.sendLine(str("What is your name?"))

    def connectionLost(self, reason):
        if self.name in self.users:
            del self.users[self.name]

    def lineReceived(self, line):
        if self.state == 'GETNAME':
            self.handle_GETNAME(line)
        else:
            self.handle_CHAT(line)

    def handle_GETNAME(self, name):
        if name in self.users:
            self.sendLine("Name taken, please choose another.")
            return
        self.sendLine("Welcome %s!" % (name,))
        self.name = name
        self.state = "CHAT"

    def handle_CHAT(self, message):
        message = "<%s> %s" % (self.name, massage)
        for name, protocol in self.users.iteritems():
            if protocol != self:
                protocol.sendLine(message)

class chat_factory(Factory):
    def __init__(self):
        self.users = {}

    def buildProtocol(self, addr):
        return Chat(self.users)

reactor.listenTCP(8123, chat_factory())
reactor.run()
