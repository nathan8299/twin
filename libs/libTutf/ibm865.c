
hwfont Tutf_IBM865_to_UTF_16[0x100] = {
#define EL(x) T_UTF(UTF_16,x),
	T_LIST(IBM865,EL)
#undef EL
};

static byte flag_IBM865;

static utf_to_ch array_IBM865 [] = {
#define EL(x) { T_UTF(UTF_16,x), T_UTF(IBM865,x) },
    T_NLIST(IBM865,EL)
#undef EL
    { T_UTF_16_CHECK_MARK, T_UTF(IBM865,_SQUARE_ROOT) },
};

hwfont Tutf_UTF_16_to_IBM865(hwfont c) {
    static utf_to_ch key;
    utf_to_ch *res;
    
    /* Codepage 865 obviously cannot contain all unicode chars. this is just a best effort. */
    if (!flag_IBM865) {
	flag_IBM865 = TRUE;
	QSORT(array_IBM865);
    }
    if (c == key.utf)
	return key.ch;
    if ((c & ~0x00ff) == 0xf000 ||
	/* direct-to-font area */
	(c >= ' ' && c <= '~') ||
	/* ASCII area */
	(c > '~' && c < 0x100 && Tutf_IBM865_to_UTF_16[c] == c))
	/* c has the same meaning in Unicode and this charset... sheer luck! */

	return c & 0x00ff;
    
    key.utf = c;
    res = BSEARCH(&key, array_IBM865);
    
    if (res)
	c = res->ch;
    else if (c > '~')
	/* try to approximate */
	c = T_CAT(Tutf_IBM437_to_,T_MAP(ASCII)) [ Tutf_UTF_16_to_IBM437(c) ];
    /* else c = c; */

    return key.ch = c;
}

