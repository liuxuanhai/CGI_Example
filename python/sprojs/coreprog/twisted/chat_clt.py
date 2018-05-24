# coding:utf-8
from twisted.internet import reactor, task
from twisted.internet.protocol import Protocol, ClientFactory
import struct
from twisted.python import log
import sys
import json
log.startLogging(sys.stdout)


class EchoClient(Protocol):
    def __init__(self):
        self.command_func_dict = {
            101: self.handle_verify_s,
            102: self.handle_single_chat_s,
            103: self.handle_group_chat_s,
            104: self.handle_broadcast_chat_s
        }
        self.version = 0
        self.state = "VERIFY"
        self.phone_number = ""

    def connectionMade(self):
        log.msg("New connection", self.transport.getPeer())

    def dataReceived(self, data):
        length, self.version, command_id = struct.unpack('!3I', data[:12])
        content = data[12:length]
        if self.state == "VERIFY" and command_id == 101:
            self.handle_verify_s(content)
        else:
            self.handle_data(command_id, content)

    def handle_data(self, command_id, pack_data):
        self.command_func_dict[command_id](pack_data)

    def connectionLost(self, reason):
        log.msg("connection lost")

    def handle_verify_s(self, pack_data):
        """
        接受验证结果
        """
        content = json.loads(pack_data)
        code = content.get('code')
        if code == 1:
            log.msg('验证通过')
        self.state = "Data"

    def handle_single_chat_s(self, pack_data):
        """
        接受单聊
        """
        content = json.loads(pack_data)
        chat_from = content.get('chat_from')
        chat_content = content.get('chat_content')
        log.msg("[单聊][%s]:%s" % (chat_from.encode('utf-8'), chat_content.encode('utf-8')))

    def handle_group_chat_s(self, pack_data):
        """
        接受组聊
        """
        content = json.loads(pack_data)
        chat_from = content.get('chat_from')
        chat_content = content.get('chat_content')
        log.msg("[组聊][%s]:%s" % (chat_from.encode('utf-8'), chat_content.encode('utf-8')))

    def handle_broadcast_chat_s(self, pack_data):
        """
        接受广播
        """
        content = json.loads(pack_data)
        chat_from = content.get('chat_from')
        chat_content = content.get('chat_content')
        log.msg("[群聊][%s]:%s" % (chat_from.encode('utf-8'), chat_content.encode('utf-8')))

    def send_verify(self, phone_number):
        """
        发送验证
        """
        content = json.dumps(dict(phone_number=phone_number))
        self.send_data(content, 1)

    def send_single_chat(self, chat_from, chat_to, chat_content):
        """
        发送单聊内容
        """
        content = json.dumps(dict(chat_from=chat_from, chat_to=chat_to, chat_content=chat_content))
        self.send_data(content, 2)

    def send_group_chat(self, chat_from, chat_to, chat_content):
        """
        发送组聊内容
        """
        content = json.dumps(dict(chat_from=chat_from, chat_to=chat_to, chat_content=chat_content))
        self.send_data(content, 3)

    def send_broadcast_chat(self, chat_from, chat_content):
        """
        发送群聊内容
        """
        content = json.dumps(dict(chat_from=chat_from, chat_content=chat_content))
        self.send_data(content, 4)

    def send_data(self, send_content, command_id):
        """
        发送函数
        """
        length = 12 + len(send_content)
        version = self.version
        command_id = command_id
        header = [length, version, command_id]
        header_pack = struct.pack('!3I', *header)
        self.transport.write(header_pack + send_content)


class EchoClientFactory(ClientFactory):
    def __init__(self):
        self.p = EchoClient()

    def startedConnecting(self, connector):
        log.msg("Started to connect")

    def buildProtocol(self, addr):
        log.msg("Connected.")
        return self.p

    def clientConnectionFailed(self, connector, reason):
        log.msg("Lost connection. Reason:", reason)

    def clientConnectionLost(self, connector, reason):
        log.msg("Connection failed. Reason:", reason)


if __name__ == '__main__':
    cf = EchoClientFactory()
    chat_from = sys.argv[1]
    all_phone_numbers = ['000001', '000002', '000003', '000004']
    all_phone_numbers.remove(chat_from)
    import random
    reactor.callLater(3, cf.p.send_verify, chat_from)
    reactor.callLater(10, cf.p.send_single_chat, chat_from, random.choice(all_phone_numbers), '你好,这是单聊')
    reactor.callLater(11, cf.p.send_group_chat, chat_from, [random.choice(all_phone_numbers), random.choice(all_phone_numbers)], '你好,这是组聊')
    reactor.callLater(12, cf.p.send_broadcast_chat, chat_from, '你好,这是群聊')

    reactor.connectTCP('127.0.0.1', 8124, cf)

    reactor.run()