#include "charset.h"


// ----------------------------- utf-8 ----------------------------------------
static int my_utf8_uni( my_wc_t * pwc, const uchar *s, const uchar *e)
{
	unsigned char c;

	if (s >= e)
		return MY_CS_TOOSMALL;

	c= s[0];
	if (c < 0x80)
	{
		*pwc = c;
		return 1;
	}
	else if (c < 0xc2)
		return MY_CS_ILSEQ;
	else if (c < 0xe0)
	{
		if (s+2 > e) /* We need 2 characters */
			return MY_CS_TOOSMALL2;

		if (!((s[1] ^ 0x80) < 0x40))
			return MY_CS_ILSEQ;

		*pwc = ((my_wc_t) (c & 0x1f) << 6) | (my_wc_t) (s[1] ^ 0x80);
		return 2;
	}
	else if (c < 0xf0)
	{
		if (s+3 > e) /* We need 3 characters */
			return MY_CS_TOOSMALL3;
		if (!((s[1] ^ 0x80) < 0x40 && (s[2] ^ 0x80) < 0x40 && (c >= 0xe1 || s[1] >= 0xa0)))
			return MY_CS_ILSEQ;

		*pwc = ((my_wc_t) (c & 0x0f) << 12)   |
			((my_wc_t) (s[1] ^ 0x80) << 6) |
			(my_wc_t) (s[2] ^ 0x80);

		return 3;
	}
#ifdef UNICODE_32BIT
	else if (c < 0xf8 && sizeof(my_wc_t)*8 >= 32)
	{
		if (s+4 > e) /* We need 4 characters */
			return MY_CS_TOOSMALL4;

		if (!((s[1] ^ 0x80) < 0x40 &&
			(s[2] ^ 0x80) < 0x40 &&
			(s[3] ^ 0x80) < 0x40 &&
			(c >= 0xf1 || s[1] >= 0x90)))
			return MY_CS_ILSEQ;

		*pwc = ((my_wc_t) (c & 0x07) << 18)    |
			((my_wc_t) (s[1] ^ 0x80) << 12) |
			((my_wc_t) (s[2] ^ 0x80) << 6)  |
			(my_wc_t) (s[3] ^ 0x80);

		return 4;
	}
	else if (c < 0xfc && sizeof(my_wc_t)*8 >= 32)
	{
		if (s+5 >e) /* We need 5 characters */
			return MY_CS_TOOSMALL5;

		if (!((s[1] ^ 0x80) < 0x40 &&
			(s[2] ^ 0x80) < 0x40 &&
			(s[3] ^ 0x80) < 0x40 &&
			(s[4] ^ 0x80) < 0x40 &&
			(c >= 0xf9 || s[1] >= 0x88)))
			return MY_CS_ILSEQ;
		*pwc = ((my_wc_t) (c & 0x03) << 24) |
			((my_wc_t) (s[1] ^ 0x80) << 18) |
			((my_wc_t) (s[2] ^ 0x80) << 12) |
			((my_wc_t) (s[3] ^ 0x80) << 6) |
			(my_wc_t) (s[4] ^ 0x80);
		return 5;
	}
	else if (c < 0xfe && sizeof(my_wc_t)*8 >= 32)
	{
		if ( s+6 >e ) /* We need 6 characters */
			return MY_CS_TOOSMALL6;

		if (!((s[1] ^ 0x80) < 0x40   &&
			(s[2] ^ 0x80) < 0x40   &&
			(s[3] ^ 0x80) < 0x40   &&
			(s[4] ^ 0x80) < 0x40   &&
			(s[5] ^ 0x80) < 0x40   &&
			(c >= 0xfd || s[1] >= 0x84)))
			return MY_CS_ILSEQ;

		*pwc = ((my_wc_t) (c & 0x01) << 30)
			| ((my_wc_t) (s[1] ^ 0x80) << 24)
			| ((my_wc_t) (s[2] ^ 0x80) << 18)
			| ((my_wc_t) (s[3] ^ 0x80) << 12)
			| ((my_wc_t) (s[4] ^ 0x80) << 6)
			| (my_wc_t) (s[5] ^ 0x80);
		return 6;
	}
#endif
	return MY_CS_ILSEQ;
}


static int my_ismbchar_utf8( const char *b, const char *e)
{
	my_wc_t wc;
	int  res=my_utf8_uni( &wc, (const uchar*)b, (const uchar*)e);
	return (res>1) ? res : 0;
}

static int my_mbcharlen_utf8( uint c)
{
	if (c < 0x80)
		return 1;
	else if (c < 0xc2)
		return 0; /* Illegal mb head */
	else if (c < 0xe0)
		return 2;
	else if (c < 0xf0)
		return 3;
#ifdef UNICODE_32BIT
	else if (c < 0xf8)
		return 4;
	else if (c < 0xfc)
		return 5;
	else if (c < 0xfe)
		return 6;
#endif
	return 0; /* Illegal mb head */;
}

// -------------------------------------------------------------------------------



#ifdef ENCODING_GBK
static int my_ismbchar(const char* p, const char *e)
{
	return (isgbkhead(*(p)) && (e)-(p)>1 && isgbktail(*((p)+1))? 2: 0);
}

static int my_mbcharlen(uchar c)
{
	return (isgbkhead(c)? 2 : 1);
}
#endif // ENCODING_GBK

#ifdef ENCODING_UTF8
static int my_ismbchar(const char* p, const char *e)
{
	return my_ismbchar_utf8(p, e);
}

static int my_mbcharlen(uchar c)
{
	return my_mbcharlen_utf8(c);
}
#endif // ENCODING_UTF8




ulong escape_string_for_mysql(char *to, ulong to_length, const char *from, ulong length)
{
	const char *to_start= to;
	const char *end, *to_end=to_start + (to_length ? to_length-1 : 2*length);
	bool overflow= false;
#ifdef USE_MB
	bool use_mb_flag= true; //use_mb(charset_info);
#endif
	for (end= from + length; from < end; from++)
	{
		char escape= 0;
#ifdef USE_MB
		int tmp_length;
		if (use_mb_flag && (tmp_length= my_ismbchar(from, end)))
		{
			if (to + tmp_length > to_end)
			{
				overflow= TRUE;
				break;
			}
			while (tmp_length--)
				*to++= *from++;
			from--;
			continue;
		}
		/*
		*      If the next character appears to begin a multi-byte character, we
		*      escape that first byte of that apparent multi-byte character. (The
		*      character just looks like a multi-byte character -- if it were actually
		*      a multi-byte character, it would have been passed through in the test
		*      above.)
		*
		*      Without this check, we can create a problem by converting an invalid
		*      multi-byte character into a valid one. For example, 0xbf27 is not
		*      a valid GBK character, but 0xbf5c is. (0x27 = ', 0x5c = \)
		*/
		if (use_mb_flag && (tmp_length= my_mbcharlen(*from)) > 1)
			escape= *from;
		else
#endif
			switch (*from) {
				case 0:				/* Must be escaped for 'mysql' */
					escape= '0';
					break;
				case '\n':				/* Must be escaped for logs */
					escape= 'n';
					break;
				case '\r':
					escape= 'r';
					break;
				case '\\':
					escape= '\\';
					break;
				case '\'':
					escape= '\'';
					break;
				case '"':				/* Better safe than sorry */
					escape= '"';
					break;
				case '\032':			/* This gives problems on Win32 */
					escape= 'Z';
					break;
		}
		if (escape)
		{
			if (to + 2 > to_end)
			{
				overflow= TRUE;
				break;
			}
			*to++= '\\';
			*to++= escape;
		}
		else
		{
			if (to + 1 > to_end)
			{
				overflow= TRUE;
				break;
			}
			*to++= *from;
		}
	}
	*to= 0;
	return overflow ? (ulong)~0 : (ulong) (to - to_start);
}

