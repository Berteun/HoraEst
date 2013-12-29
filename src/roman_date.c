#include <stdio.h>
#include <time.h>
#include <string.h>

#include "roman_numerals.h"

/* For addressing pripost-arrays */
#define PR 0
#define POS 1

/* Handles for switch statements */
#define PRID 2
#define POST 1
#define DIES 0

static const char* terse[4]={"","Kal","Non","Id"};
static const char* pripost_terse[2]={"pr","post"};
static const char* month_terse[13]={"", "Ian", "Feb", "Mar", "Apr", "Mai", "Iun", "Iul", "Aug", "Sep", "Oct", "Nov", "Dec"};

enum RomanDays
{
	ROMAN_DAY_KALENDS = 1,
	ROMAN_DAY_NONES = 2,
	ROMAN_DAY_IDES = 3,
};

int is_leap_year(int year)
{
	if (year % 4 != 0)
		return 0;
	if (year % 400)
		return 1;
	if (year % 100)
		return 0;
	return 1;
}

void date_to_roman(struct tm* t, char* out)
{            	
	enum RomanDays next_day;

	/* Kalends=1, Nones=2, Ides=3 */
	int days_to_next;   /* Number of days to the next */
	int bis; /* Is it the second of the two a.d. V Kal. Mar. of a leap year? */
	int year;/* In special cases (end of december), the year verbose and     */
	int month;/* terse are non-identical.                                    */

	bis=0;
	t->tm_mon++;
	/* 753 is the traditional founding of the city */
	t->tm_year += 753 + 1900;
	month=t->tm_mon;

	int leap = is_leap_year(t->tm_year);
	if (leap && t->tm_mon == 2 && t->tm_mday > 23)
	{        
		month++;
		/* Special treatment for leap year days!! */
		switch(t->tm_mday)
		{
			case 24:
				days_to_next=6;
				break;
			case 25:
				days_to_next=6;
				bis=1;
				break;
			case 26:
				days_to_next=5;
				break;
			case 27:
				days_to_next=4;
				break;
			case 28:
				days_to_next=3;
				break;
			case 29:
				days_to_next=PRID;  /* pridie */
				break;
			default:  /* should **NEVER** occur */
				strcat(out, "-");
				return;
		}
		next_day = ROMAN_DAY_KALENDS;
	}
	else if(t->tm_mon==3 || t->tm_mon==7 || t->tm_mon==10 || t->tm_mon==5)
	{
		switch(t->tm_mday)
		{
			case 1:
				next_day = ROMAN_DAY_KALENDS;
				days_to_next = DIES;
				break;
			case 2:
				next_day = ROMAN_DAY_KALENDS;
				days_to_next = POST;
				break;
			case 3:
			case 4:
			case 5:
				next_day = ROMAN_DAY_NONES;
				days_to_next = 8-t->tm_mday;
				break;
			case 6:
				next_day = ROMAN_DAY_NONES;
				days_to_next = PRID;
				break;
			case 7:
				next_day = ROMAN_DAY_NONES;
				days_to_next = DIES;
				break;
			case 8:
				next_day = ROMAN_DAY_NONES;
				days_to_next = POST;
				break;
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
				next_day = ROMAN_DAY_IDES;
				days_to_next = 16-t->tm_mday;
				break;
			case 14:
				next_day = ROMAN_DAY_IDES;
				days_to_next = PRID;
				break;
			case 15:
				next_day = ROMAN_DAY_IDES;
				days_to_next = DIES;
				break;
			case 16:
				next_day = ROMAN_DAY_IDES;
				days_to_next = POST;
				break;
			default:
				next_day = ROMAN_DAY_KALENDS;
				month++;
				days_to_next = 33-t->tm_mday;
				break;
		}
	}
	else
	{
		switch(t->tm_mday)
		{
			case 1:
				next_day = ROMAN_DAY_KALENDS;
				days_to_next = DIES;
				break;
			case 2:
				next_day = ROMAN_DAY_KALENDS;
				days_to_next = POST;
				break;
			case 3:
				next_day = ROMAN_DAY_NONES;
				days_to_next = 3;
				break;
			case 4:
				next_day = ROMAN_DAY_NONES;
				days_to_next = PRID;
				break;	    
			case 5:    
				next_day = ROMAN_DAY_NONES;
				days_to_next = DIES;
				break;
			case 6:
				next_day = ROMAN_DAY_NONES;
				days_to_next = POST;
				break;
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
				next_day = ROMAN_DAY_IDES;
				days_to_next = 14-t->tm_mday;
				break;
			case 12:
				next_day = ROMAN_DAY_IDES;
				days_to_next = PRID;
				break;
			case 13:
				next_day = ROMAN_DAY_IDES;
				days_to_next = DIES;
				break;
			case 14:
				next_day = ROMAN_DAY_IDES;
				days_to_next = POST;
				break;
			default:
				next_day = ROMAN_DAY_KALENDS;
				month++;
				if(t->tm_mon==2)
					days_to_next = 30-t->tm_mday;
				else if(t->tm_mon==4 || 
						t->tm_mon==6 || 
						t->tm_mon==9 || 
						t->tm_mon==11)
					days_to_next = 32-t->tm_mday;
				else
				{
					days_to_next = 33-t->tm_mday;
				}
		}   
	}

	/* What if we have a day in the end of December? */
	if(month==13)
	{
		month=1;
		year=t->tm_year+1;
	}
	else
	{
		year=t->tm_year;
	}

	if(days_to_next == DIES)  /* On a main day */
	{
		strcat(out, terse[next_day]);
		strcat(out, " ");
		strcat(out, month_terse[month]);
	}
	else
	{
		if(days_to_next==POST) /* postridie */
		{
			strcat(out, pripost_terse[POS]);
			strcat(out, " ");
			strcat(out, terse[next_day]);
			strcat(out, " ");
			strcat(out, month_terse[month]);
		}
		else if(days_to_next==PRID) /* pridie */
		{
			strcat(out, pripost_terse[PR]);
			strcat(out, " ");
			strcat(out, terse[next_day]);
			strcat(out, " ");
			strcat(out, month_terse[month]);
		}
		else if(bis==1) /* Second VI Kal. Mar. */
		{
			to_roman_numerals(days_to_next, out);
			strcat(out, " ");
			strcat(out, terse[next_day]);
			strcat(out, " ");
			strcat(out, month_terse[month]);
			//strcat(out, " bis");
		}
		else
		{
			to_roman_numerals(days_to_next, out);
			strcat(out, " ");
			strcat(out, terse[next_day]);
			strcat(out, " ");
			strcat(out, month_terse[month]);
		}
	}

	(void) year;
	//to_roman_numerals(year, out);

}
