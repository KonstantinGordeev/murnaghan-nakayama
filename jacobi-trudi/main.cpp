#include <pari/pari.h>
#include <string>
#include "../tools/partitions.h"

static GEN p;
static GEN x;

// zee(lambda) = the order of the S_n centralizer of a permutation of
// cycle type lambda; i.e., 1^(m1)*m1!*2^(m2)*m2!*...
size_t zee(const Partition<>& mu) {
    size_t m = 1, res = 1;
    for (auto& item: mu) {
        res *= item;
    }
    for (size_t index = 1; index < mu.size(); ++index) {
        if (mu[index] < mu[index - 1]) {
            m = 1;
        } else {
            m += 1;
        }
        res *= m;
    }
    return res;
}

GEN top(GEN f, std::string basis = "p") {
    return f;
}

//    `SF/top`:=proc() local f,bases,sp,i,mu,b,d,a;
//    f:=args[1]; bases:=`SF/getbase`(args) minus {'p'};
//    sp:=SF['varset'](f,bases minus {'h','e'});
//    for b in bases minus {'h','e','s[]'} do
//        a:=op(0,b); if sp[a]=[] then next fi;
//    if assigned(`SF/dual`[a]) then
//    f:=`SF/dualize`(a,sp[a],f)
//    else
//    f:=`SF/added`(a,sp[a],f); bases:=bases union {'e'}
//            fi
//    od;
//    if member('s[]',bases) then
//    f:=subs({seq(s[op(mu)]=`SF/jt_det`(mu,0),mu=sp['s'])},f);
//    bases:={op(bases),'h','e'}
//    fi;
//    if member('h',bases) then f:=`SF/to_ehp`('h','p',f) fi;
//    if member('e',bases) then f:=`SF/to_ehp`('e','p',f) fi;
//    d:=SF['varset'](f,'p');
//    collect(f,[seq(cat('p',i),i=1..d)],'distributed',normal);
//    end:

// sf2char(mu) applies the inverse characteristic map to schur(mu)
// The result is expressed as a linear combination of characteristic
// functions cl[mu] for partitions mu.
std::vector<int> sf2char(const Partition<>& mu) {
    GEN f = top(GEN());
    auto d = mu.num();
    std::vector<double> coefs = {1./3, 1./4};

}

GEN
test_det(void)	  /* void */
{
    GEN m,x,p1;	  /* vec */
    m = pol_x(fetch_user_var("m"));
    x = pol_x(0);
    p1 = cgetg(3, t_MAT);
    gel(p1, 1) = cgetg(3, t_COL);
    gel(p1, 2) = cgetg(3, t_COL);
    gcoeff(p1, 1, 1) = gaddgs(gsqr(x), 1);
    gcoeff(p1, 1, 2) = gaddgs(gmulsg(2, x), 7);
    gcoeff(p1, 2, 1) = gaddgs(x, 6);
    gcoeff(p1, 2, 2) = gsubgs(gpowgs(x, 3), 1);
    m = p1;
    return det(m);
}

int
main()
{
    GEN d;
    pari_init(1000000,2);
    d = test_det();

    pari_printf("factor = %Ps\n", d);
    pari_close();
    return 0;
}