#include <allegro.h>
/* XPM */
static const char *allegico_xpm[] = {
/* columns rows colors chars-per-pixel */
"32 32 186 2",
"   c black",
".  c #17172C2C5A5A",
"X  c #16332DD865BB",
"o  c #16162F2F6868",
"O  c #18182F2F6868",
"+  c #1B1B31DC6610",
"@  c #1C2A34426AA3",
"#  c #1BC6368C71C7",
"$  c #1DEB39A0750E",
"%  c #212135355E5E",
"&  c #2121358B640E",
"*  c #212137376B6B",
"=  c #23A4393966E7",
"-  c #23F13A876B1E",
";  c #21A23DBE7BBB",
":  c #2A2A45456565",
">  c #282841416A6A",
",  c #232341417F7F",
"<  c #2B2B47477171",
"1  c #282842427979",
"2  c #303049497D7D",
"3  c #39394F4F7575",
"4  c #252543C48383",
"5  c #2C2C49498282",
"6  c #2A104A978C8C",
"7  c #2E8451A795EB",
"8  c #35354F4F8484",
"9  c #323251518E8E",
"0  c #3E3E5F5F8484",
"q  c #317155959B5B",
"w  c #323258589F9F",
"e  c #3BBC5D5D9C9C",
"r  c #35185B5BA34D",
"t  c #36365E5EA9A9",
"y  c #38E3620CACAC",
"u  c #3B6E6666B34C",
"i  c #3F3F68E9B5B5",
"p  c #3EE96C6CBB65",
"a  c #41EC5C0683D9",
"s  c #44445C5C8B8B",
"d  c #4343629585EC",
"f  c #414160608C8C",
"g  c #4A4A67678888",
"h  c #49CA68688B8B",
"j  c #494967679F9F",
"k  c #54546AEB9111",
"l  c #549472329292",
"z  c #53D474749C1C",
"x  c #5A5A77779999",
"c  c #5B5B7CFD9C9C",
"v  c #454569EAABAB",
"b  c #43436B6BB1B1",
"n  c #40736CD3BB21",
"m  c #45C671F2BF3F",
"M  c #52527070AAAA",
"N  c #57577B7BA1A1",
"B  c #5C5C7474A1A1",
"V  c #51517878BBBB",
"C  c #60607F7F9D9D",
"Z  c #60607A7AA4A4",
"A  c #61617E7EB0B0",
"S  c #44197373C51A",
"D  c #47477878CACA",
"F  c #48487777C8C8",
"G  c #4B917B92CCCC",
"H  c #4E057EC7D0F5",
"J  c #53537F7FCACA",
"K  c #58588383A1A1",
"L  c #630D83D9A3A3",
"P  c #63E48404AD2D",
"I  c #65658B8BA8A8",
"U  c #6AEB8E8EAA2A",
"Y  c #66668484B9B9",
"T  c #6C6C8787B1B1",
"R  c #6E188CE2B60B",
"E  c #69EA9595B535",
"W  c #70708E8EB4B4",
"Q  c #78789494AAAA",
"!  c #71F29393B4B4",
"~  c #75759292BABA",
"^  c #73F49999B535",
"/  c #71719D9DB9B9",
"(  c #7C7C9898B1B1",
")  c #7D7DA323BDBD",
"_  c #55558383CF4F",
"`  c #527D8282D227",
"'  c #58D98707D4D4",
"]  c #5D778AF1D4EE",
"[  c #5C5C8B8BD8D8",
"{  c #66668A0AC4C4",
"}  c #6E6E8F8FC4C4",
"|  c #618C8E38D680",
" . c #61E28F8FD9D9",
".. c #6A6A9090CFCF",
"X. c #661091E7D6D6",
"o. c #65E692D2DADA",
"O. c #6AEB9494D5D5",
"+. c #6A8495FCDAA7",
"@. c #6ED59966DA73",
"#. c #74749595C3C3",
"$. c #7BFC9A9ACD4D",
"%. c #72729898D1D1",
"&. c #739E9D9DDD07",
"*. c #7B7B9C9CD2D2",
"=. c #7DFEA3A3C242",
"-. c #7B7BA9A9C5C5",
";. c #7BAEA389DDF7",
":. c #7676A121E262",
">. c #7A7AA323E161",
",. c #84849B9BB0B0",
"<. c #824FA309BBEE",
"1. c #8CE2A44EBD67",
"2. c #8C8CA8A8BFBF",
"3. c #9313A626BBBB",
"4. c #85859F9FC1C1",
"5. c #8787ADADC6C6",
"6. c #8888A0A0C0C0",
"7. c #8AE0AD57C56F",
"8. c #8E8EAAAACDCD",
"9. c #8383A2A2D1D1",
"0. c #8202A727DE5E",
"q. c #8383A9A9DF5F",
"w. c #8B8BADADDDDD",
"e. c #8181B1B1CFCF",
"r. c #8D0DB9B9D3D3",
"t. c #9494AD57C61B",
"y. c #9090ADADD7D7",
"u. c #9696B2B2C6C6",
"i. c #9595B6B6CDCD",
"p. c #9F9FB0B0C7C7",
"a. c #9898B0B0CCCC",
"s. c #9797B6B6DFDF",
"d. c #9515BE3ED555",
"f. c #9C46B55FD429",
"g. c #9B45BEBED47E",
"h. c #84CDAB86E3BE",
"j. c #8964AEF7E299",
"k. c #8C8CB106E4E4",
"l. c #9212B636E726",
"z. c #9595B7B7ECEC",
"x. c #9640B963E7E7",
"c. c #9616BA3AE8E8",
"v. c #9B9BBD67E7E7",
"b. c #9C69BE8BE9E9",
"n. c #A5A5BB3BD252",
"m. c #9797C6C6DEDE",
"M. c #9F9FC2C2D7D7",
"N. c #9C9CC5C5DCDC",
"B. c #9696C9C9E4E4",
"V. c #9E1EC141EB6B",
"C. c #A525C242D4D4",
"Z. c #A1A1C7C7DDDD",
"A. c #ABABC2C2D7D7",
"S. c #AEAEC3C3DFDF",
"D. c #A9A9CBCBDEDE",
"F. c #B7B7C9C9DEDE",
"G. c #A5A5C2C2E6E6",
"H. c #A4A4C47BEC34",
"J. c #A2A2CBCBE2E2",
"K. c #ADADCC76EF44",
"L. c #ACACCB4BF2F2",
"P. c #A3A3D0D0E7E7",
"I. c #A7A7D3D3E9E9",
"U. c #AC56D17BE5E5",
"Y. c #AAAAD4D4EAEA",
"T. c #AA2ADA5AF272",
"R. c #B1B1C5C5E1E1",
"E. c #B5B5CACAE0E0",
"W. c #B8B8CCCCE5E5",
"Q. c #BBBBCFCFEAEA",
"!. c #B1B1CF4FF1F1",
"~. c #BB3BDC1BEDED",
"^. c #B3B3D0D0F2F2",
"/. c #BCFCD6D6F575",
"(. c #BEBED9D9F6F6",
"). c #C2C2D6D6EDED",
"_. c #C7C7DCDCF4F4",
"`. c #C4C4DE33FBFB",
"'. c #C6C6E58FF4F4",
"]. c #CB4BE767F575",
"[. c #C949EAEAFB7B",
"{. c #DBDBEEEEF7F7",
"}. c #D8D8EEEEFAFA",
"|. c #D2D2F0F0FFFF",
" X c #DCDCF2F2FCFC",
".X c None",
/* pixels */
".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X.X.X.X.XC.u..X.X.X.X.X.X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X.X.X.X~.3.p.U..X.X.X.X.X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X.X.X.X<.t.a.^ .X.X.X.X.X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X.X.X2.W.^.H.H.4.Z..X.X.X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X.X2._.!.V.j.j.z.W .X.X.X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X~.t./.v.k.>.@.>.T g..X.X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.XD.1.^.c.j.&. .O.A g..X.X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X.Xt./.V.h.+. .@.B M..X.X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X.X5.R.L.h.@.@.$.I .X.X.X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X.XT.=.8.w.;.{ P B..X.X.X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X.Xr./ L ! ~ z E e..X.X.X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X7.n.S.G.x.j.h.0.4.W =..X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X7.A.)./.^.V.k.;.>.&.:.h.9.6..X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.XU.<.F.`.^.H.V.k.>.&.O. .[ ] :.l.W g.i..X.X.X.X.X.X",
".X.X.X.X.X.XQ 3.E.(.K.H.V.k.j.;.&.| | [ '  .+.;.~ C ]..X.X.X.X.X",
".X.X.X.X.X.XM.n._.K.V.V.c.k.>.&.@.+.+. .] G `  .*.^ [..X.X.X.X.X",
".X.X.X.X.X.X( /.K.V.V.c.k.;.>.@.+.+.+. .H H G J ..R Y..X.X.X.X.X",
".X.X.X.X.XI.f.^.V.x.c.k.;.@.+.] _ | [ ' G G F m V A I .X.X.X.X.X",
".X.X.X.XU.-.f.H.c.k.;.;.@.X.' ` H H H H S n p b v M N m.g..X.X.X",
".X.X.X<.1.l s.l.;.;.@.X.[ ' ` H H H F S p p y t e e f h k ,..X.X",
".X.XC.Q.`.y.q.;.@.X.| ] ` G G D G S S p u y w 7 6 5 2 s B k  X.X",
".X.X.X,.a.l %.O.' ` D S m m n i u u u t r q 6 ; $ * < d 3 <..X.X",
".X.X.X.Xr.E } ] G m u u t r r t r r q 7 6 ; # X + @ 0 T.(..X.X.X",
".X.X.X.X.XI.R { v t r r 7 6 6 6 6 6 4 4 $ O X X @ = l .X.X.X.X.X",
".X.X.X.X.X(.c Y v q 6 6 4 4 $ $ $ $ # O X X O + * > .X.X.X.X.X.X",
".X.X.X.X.X.XB.z j 9 , 1 # @ @ @ @ @ X X X X + * = ^ .X.X.X.X.X.X",
".X.X.X.X.X.X[.c a 8 1 # @ @ @ @ . $ @ @ @ @ * & % ) .X.X.X.X.X.X",
".X.X.X.X.X.X.Xl L d = * * - & d K = = - * - > I h ,..X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.XU d a d U '.[.N.K a g : 5.|.}..X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X.X]..X.X.X.X.X X.X.X'..X.X.X.X.X.X.X.X.X.X",
".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X"
};
#if defined ALLEGRO_WITH_XWINDOWS && defined ALLEGRO_USE_CONSTRUCTOR
extern void *allegro_icon;
CONSTRUCTOR_FUNCTION(static void _set_allegro_icon(void));
static void _set_allegro_icon(void)
{
    allegro_icon = allegico_xpm;
}
#endif
