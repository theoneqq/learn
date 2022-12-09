#n >= m
def c(n, m):
	if m == 0:
		return 1
	else:
		res = 1
		for i in range(1, m + 1):
			res = res * (n - i + 1) / i
		return round(res)

def c(n, m):
	if m == 0:
		return 1
	else:
		res = 1
		for i in range(1, m + 1):
			res = res * (n - i + 1) / i
		return round(res)

def lucas(n, m, p):
	if n < m:
		return 1
	else:
		res = 1
		while m > 0:
			res = res * c((n % p), (m % p))
			n, m = int(n / p), int(m / p)
		return res % p

def triangle(row):
	a = []
	for char in row:
		if char == 'R':
			a.append(0)
		elif char == 'G':
			a.append(1)
		else:
			a.append(2)
	sum = 0
	length = len(a)
	for i in range(0, length):
		if a[i] != 0:
			mate = length - 1 - i
			mate_num = a[mate]
			if mate_num > 3:
				sum = sum + (a[mate] - 3) * a[i]
			else:
				l = lucas(length - 1, i, 3)
				sum = sum + l * a[i]
				a[i] = l + 3
	sum = ((-1 if length % 2 == 0 else 1) * sum) % 3
	if sum == 0:
		return 'R'
	elif sum == 1:
		return 'G'
	else:
		return 'B'
		
print("res: ", triangle("RGBG"))
print("res: ", triangle("RBRGBRB"))
print("res: ", triangle("RBRGBRBGGRRRBGBBBGG"))
