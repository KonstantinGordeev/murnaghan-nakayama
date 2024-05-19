import time

R = QQ['a']

SF = SymmetricFunctions(QQ)
p = SF.powersum()
z = SF.zonal()
s = SF.schur()

_e = SF.elementary()
def e_basis_impl(mu):
  return _e[mu]

e_basis = cached_function(e_basis_impl)

def e2p_impl(mu):
  return p(e_basis(mu))

e2p = cached_function(e2p_impl)

def P_impl(n):
  return [mu for mu in Partitions(n)]

P = cached_function(P_impl)

def gs_impl(mu,a):
  lc = mu.conjugate().hook_product(a)
  g = e2p(mu)
  f = lc*e_basis(mu)
  n = mu.size()
  N = lc^2 * g.scalar_jack(g,a);
  sc = {}
  for nu in P(n): #Partitions(n,max_length=len(mu)): #ending=mu
    if nu == mu:
      break
    if nu.dominates(mu):
      sc[nu] = g.scalar_jack(e2p(nu),a)
      c0,f0 = gs(nu,a)
      ss = 0
      for (key,value) in f0.monomial_coefficients().items():
        ss = ss + value * sc[key]
      #ss = sum(value * sc[key] for (key,value) in f0.monomial_coefficients().items())
      c = ss*lc/c0
      f = f - c*f0
      N = N - c^2*c0
  return N,f

gs = cached_function(gs_impl)

def jack(mu,a):
  c,f = gs(mu.conjugate(),a)
  return p(f)

def test(n,a):
  return [jack(mu,a) for mu in P(n)]
