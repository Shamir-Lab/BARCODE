import sys
import scipy
from scipy.optimize import minimize
from scipy.misc import comb
def cost(r, err_rate, Gen_len=6e7, n=6e6, end_len=100, read_len=100):
	Q = 2*Gen_len*(3*end_len+1)
	c=.6185
	# constants assuming 0.003 base error rate
	# read length 100
	one_err_rate = comb(read_len, 1) * (1-err_rate)**(read_len - 1) * \
		err_rate #0.247 #0.223
	two_err_rate = comb(read_len, 2) * (1-err_rate)**(read_len - 2) * \
		err_rate**2 #0.043 #0.033
	# bf_cost = (1+((2*Q*c**(2*r))/n))* r
	# expl_cost = ((1-(float(end_len)/read_len)) * one_err_rate + 2*two_err_rate + c**(2*r)) * 2 * read_len

	p_cost = (1+(Q*c**r)/n)* (r / (1 - c**r))
	n_cost = 0 #((1-(float(end_len)/read_len)) * one_err_rate + 2*two_err_rate) * read_len
	# print "bf cost is %f, expl cost is %f" % (bf_cost, expl_cost)
	# return bf_cost + expl_cost
	if (end_len != 0):
		print "p cost is %.3f, n cost is %.3f" % (p_cost, n_cost)
		return p_cost + n_cost
	else:	
		print p_cost
		return p_cost

def get_f(numreads, genome_len, read_len):
        print(scipy.__version__)
	print "genome_len is %d, numreads is %d, read_len is %d" % (genome_len, numreads, read_len)
        if (numreads!=0):
	    vals = minimize(cost, x0=5, args=(0.0, genome_len, numreads, 0, read_len), method='L-BFGS-B')
            r = vals.x
	    exp_cost = vals.fun
	    c = 0.6185
	    f = c**r
	if (numreads==0 or f>1):
		f = 0.1
		print "default (f=0.1) used"
                f_file = open('f_val.txt', 'w')
                f_file.write(str(f))

	else:
		print "expected cost per unique read is %f, f is %.3f" % (exp_cost, f)
                f_file = open('f_val.txt', 'w')
                f_file.write(str(f[0]))
        f_file.close()	
	return f

#get_f(3000000,60000000,100)
get_f(long(float(sys.argv[1])), long(float(sys.argv[2])), long(float(sys.argv[3])))
