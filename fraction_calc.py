import fractions


def cont_frac(*args):
    it = iter(reversed(args))
    a = fractions.Fraction(next(it))
    for elem in it:
        a = elem + 1/a
    return a

# copy from wolfram alpha and replace '346;' with '0,'
frac_parts = [0, 1, 1, 5, 1, 1, 13, 1, 32, 4, 1, 525]
int_part = 346 # this is the part before ; in continued fraction representation

full_frac = int_part + cont_frac(*frac_parts)

print("deviation, cont_frac, fraction")
for length in range(len(frac_parts)):
    appr = cont_frac(*frac_parts[:length+1])
    if appr + int_part == full_frac:
        print("={:12}%{:4} {}".format(0, frac_parts[length], appr))
    else:
        print( "{:+.10%}{:4} {}".format(float((int_part+appr)/full_frac)-1, frac_parts[length], appr))
    
