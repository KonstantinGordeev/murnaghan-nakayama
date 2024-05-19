import time

SF = SymmetricFunctions(QQ)
p = SF.powersum()
e = SF.elementary()
z = SF.zonal()
s = SF.schur()

# P = Partitions(12)
# %time tmp = [s(p[mu]) for mu in P]; [[x.coefficient(y) for y in P] for x in tmp];


F = FractionField(QQ['t'])
JJ = SymmetricFunctions(F).jack().J()
JP = SymmetricFunctions(F).jack().P()
P = SymmetricFunctions(F).powersum()



def ct(f, b, n):
    P = Partitions(n)
    m = len(P)
    res = []
    for x in P:
        tmp = f(b[x])
        res = res + [[tmp.coefficient(y) for y in Partitions(n)]]
    return res	

def test(f, b, n):
  global cumulative_time
  start = time.time()
  res = ct(f, b, n)
  end = time.time()
  cumulative_time += end - start
  print(n, end - start, cumulative_time, sep="\t")
  return res
  
#ct(s, p, 5)
#ct(z, p, 5)
#ct(p, z, 5)

#ct(P,JJ,2)
#ct(JP,P,2)

print("\nz->p\n")
cumulative_time = 0
for n in range(1, 13):
  res = test(z, p, n)

print("\nz->e\n")
cumulative_time = 0
for n in range(1, 13):
  res = test(z, e, n)

print("\ne->p\n")
cumulative_time = 0
for n in range(1, 25):
  res = test(e, p, n)

print("\np->e\n")
cumulative_time = 0
for n in range(1, 25):
  res = test(p, e, n)



#for n in range(1, 28):
#  res = test(s, p, n)

#for n in range(1, 25):
#  res = test(z, p, n)