def to_p_impl(f):
  p(f)

to_p = cached_function(to_p_impl)

%time f = e[10,10,5,4,3,1,1]
%time t = to_p(f)



cache1 = {}

def to_p2(f):
  res = cache1[f]
  if f in cache1:
    return
    res = p(f)
    cache1[f] = res
  return res

%time t = to_p2(f)




def e2p_impl(mu):
  p(e[mu])

e2p = cached_function(e2p_impl)

%time pp = Partition([10,10,5,4,3,1,1])
%time t = e2p(pp)






p = [10,10,5,4,3,1,1]
n = (int)sum(p)
%time idx = XPartitions.index_of_partition(n,p)

def e2pi_impl(n,idx):
  p(e[XPartitions.partition_by_index(n,idx)])

e2pi = cached_function(e2pi_impl)

%time t = e2pi(n,idx)




