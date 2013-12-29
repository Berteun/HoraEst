#include <string.h>

/* The array is in order from larger to smaller because Roman numerals
 * should be converted by first converting the multiples of 1000, then
 * of 100, etc.
 */
static const char* thousands = "MMMMMMMMM";

static const char* roman_digits[3][10] = {
	/* 100 - 900 */
	{"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"},
	/* 10 - 90 */
	{"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"},
	/* 1 - 9 */
	{"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"},
};


void to_roman_numerals(int n, char* out)
{
	int i, d;

	if (n <= 0 || n >= 9999)
	{
		strcat(out, "-");
		return;
	}

	strcat(out, &thousands[9 - (n / 1000)]);

	for (n %= 1000, d = 100, i = 0; d > 0; n %= d, d /= 10, i += 1)
		strcat(out, roman_digits[i][n / d]);
}

/* vim: set ts=8 noet sw=8 cc=80 tw=79 number: */
