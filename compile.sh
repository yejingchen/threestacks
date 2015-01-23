# For convenience to compile on my android phone.
# If you are using a desktop, use make.

CC=$tcc

$CC threestacks.c argtable3.c $fix_o -o threestacks -I$CINC -L$CLIB
