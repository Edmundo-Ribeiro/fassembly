SECTION TEXT
0 input n
2 copy n, resto
5 copy dd, conta
8 loop: copy resto, ref 
10 load resto
12 div DOIS
14 STORE resto
16 mul DOIS
18 store aux
20 load ref
22 sub aux
24 store aux
26 OUTPUT aux
28 load conta
30 sub um
32 store conta
34 JMPP loop
36 stop

SECTION DATA
37 dd: const 16
38 dois: const 2
39 um: const 1
40 conta: SPACE
41 n: space
42 aux: space
43 ref: space
44 resto: space