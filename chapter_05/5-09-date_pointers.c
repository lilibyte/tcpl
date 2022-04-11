/*
 * Exercise 5-9.
 *
 * Rewrite the routines day_of_year and month_day with
 * pointers instead of indexing.
*/

#include <stdio.h>

static char daytab[2][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

/* day_of_year:  set day of year from month & day; return 0 on error */
int day_of_year(int year, int month, int day) {
	if (!year || !month || !day)
		return 0;
	if (month > 12 || month < 1)
		return 0;
	int leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
	if (*(*(daytab+leap)+month) < day)
		return 0;
	for (int i = 1; i < month; ++i)
		day += *(*(daytab+leap)+i);
	return day;
}

/* month_day:  set month, day from day of year; return 1 on error */
int month_day(int year, int yearday, int *pmonth, int *pday) {
	int leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
	if (!year || yearday > 365 + leap || yearday < 1)
		return 1;
	int i;
	for (i = 1; yearday > *(*(daytab+leap)+i); ++i)
		yearday -= *(*(daytab+leap)+i);
	*pmonth = i;
	*pday = yearday;
	return 0;
}

int main() {
	printf("%d\n", day_of_year(2222, 2, 28));
	printf("%d\n", day_of_year(2222, 2, 29));
	printf("%d\n", day_of_year(2220, 2, 29));
	printf("%d\n", day_of_year(0, 2, 29));
	printf("%d\n", day_of_year(2222, 0, 29));
	printf("%d\n", day_of_year(2222, 2, 0));

	int m = 0, d = 0, status;
	status = month_day(1988, 60, &m, &d);
	printf("m/d %d/%d status %d\n", m, d, status);
	status = month_day(0, 10, &m, &d);
	printf("m/d %d/%d status %d\n", m, d, status);
	status = month_day(1989, 60, &m, &d);
	printf("m/d %d/%d status %d\n", m, d, status);
	status = month_day(1988, 0, &m, &d);
	printf("m/d %d/%d status %d\n", m, d, status);
	status = month_day(1988, 3650, &m, &d);
	printf("m/d %d/%d status %d\n", m, d, status);
}
