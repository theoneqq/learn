import asyncio

async def compute(x, y):
	print('compute %s + %s' % (x, y))
	await asyncio.sleep(1.0)
	return x + y

async def print_sum(x, y):
	result = await compute(x, y)
	print('print sum %s + %s = %s' % (x, y, result))


if __name__ == '__main__':
	print("hello")
	loop = asyncio.get_event_loop()
	loop.run_until_complete(print_sum(1, 2))
	loop.close()

	print('hello, git')