import sys
from dis import dis

def foo():
	x = 1
	def bar(y):
		z = y + 2
		return z
	return bar(x)

def func():
	f = sys._getframe(0)
	print(f.f_lasti)
	print(f.f_back)
	yield 1

	print(f.f_lasti)
	print(f.f_back)
	yield 2

a = func()
dis(func)
next(a)
next(a)
