import tornado
import tornado.websocket
from tornado.options import define, options

clients = []

class Handler(tornado.websocket.WebSocketHandler):
	def open(self):
		if self not in clients:
			clients.append(self)
		print('New connection.')

	def on_close(self):
		if self in clients:
			clients.remove(self)
		print('Connection closed.')

	def on_message(self, message):
		print(f'Message: {message}')

	def check_origin(self, origin):
		return True

app = tornado.web.Application([
	(r'/', Handler),
])

if __name__ == '__main__':
	http_server = tornado.httpserver.HTTPServer(app)
	http_server.listen(8888)
	tornado.ioloop.IOLoop.instance().start()
