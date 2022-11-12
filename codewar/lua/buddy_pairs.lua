local solution = {}
local analysis
analysis = function(num)
	if not solution.tmp[num] then
		solution.tmp[num] = { [1] = true, [num] = true }
	end
	for i = 2, math.ceil(math.sqrt(num)) do
		if num % i == 0 then
			solution.tmp[num][i] = true
			solution.tmp[num][num / i] = true
			if not solution.tmp[i] then
				analysis(i)
			end
			if not solution.tmp[num / i] then
				analysis(num / i)
			end

			for k1 in pairs(solution.tmp[i] or {}) do
				solution.tmp[num][k1] = true
				for k2 in pairs(solution.tmp[num / i] or {}) do
					if k1 * k2 <= num then
						solution.tmp[num][k1 * k2] = true
					end
				end
			end
		end
	end
	local r = 0
	for i in pairs(solution.tmp[num]) do
		if i ~= num then r = r + i end
	end
	return r
end

solution.buddy = function(start, limit)
	if not solution.tmp then
		solution.tmp = {}
	end

	local res = {}
	for m = start, limit do
		local rm = analysis(m)
		res[m] = rm
		local n = rm - 1
		if n > m then
			local rn = res[n] or analysis(n)
			res[n] = rn
			if rn == m + 1 then
				print("m: ", m, " n: ", n)
				return m, n
			end
		end
	end
	return -1, -1
end

solution.buddy(1079625, 1103735)
