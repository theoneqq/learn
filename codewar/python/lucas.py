#n >= m
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

print("c(10, 2)", c(10, 2))
print("c(10, 5)", c(10, 5))
print("c(3, 3)", c(3, 3))

print("lucas(5, 2, 3)", lucas(5, 2, 3))
print("lucas(3, 3, 3)", lucas(3, 3, 3))
			
			
