import json
from datetime import datetime

d = []
with open('file_name') as f:
  for line in f.readlines():
  j = json.loads(line)
  d.append(j)

sorted(d, key = lambda x: x['time'])
for row in d:
  print('{0}\t{1}'.format(row['time'],row['name']))
