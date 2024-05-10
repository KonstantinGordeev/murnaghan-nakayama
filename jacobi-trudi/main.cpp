#include <pari/pari.h>
#include <string>

static GEN p;
static GEN x;

GEN
test_factor(void)	  /* void */
{
    p = pol_x(fetch_user_var("p"));
    x = pol_x(0);
    p = gaddgs(gadd(gsqr(x), gmulsg(2, x)), 1);
    return factor(p);
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

GEN top(GEN f, std::string basis = "p") {
    return f;

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
}

GEN sf2char(GEN f, std::string basis = "p") {
    f = top(f, basis);
    return f;
}
//`:=proc() local f,i,d,res,cfs,term,mu;
//f:=SF['top'](args); d:=SF['varset'](f,'p');
//cfs:=[coeffs(f,[seq(cat('p',i),i=1..d)],'term')];
//term:=[term]; res:=0;
//for i to nops(cfs) do
//mu:=`SF/shape`(term[i],'p',d);
//res:=res+SF['zee'](mu)*cfs[i]*cl[op(mu)];
//od; res
//        end:

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