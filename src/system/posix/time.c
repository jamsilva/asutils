#include <as/private_.h>

/* Public API */

astime _(totime)(ulong secs)
{
	astime time;
	ulong days_in_year;
	time.second = secs  % 60;
	ulong mins  = secs  / 60;
	time.minute = mins  % 60;
	ulong hours = mins  / 60;
	time.hour   = hours % 24;
	ulong days  = hours / 24;
	time.year   = 1970;

	while(1)
	{
		days_in_year = 365 + (!(time.year % 400) - !(time.year % 100) + !(time.year % 4));
		++time.year;

		if(days < days_in_year)
		{
			--time.year;
			break;
		}

		days -= days_in_year;
	}

	if(days_in_year == 366)
	{
		if(days <= 31 + 28)
		{
			time.month = 1 + (days >= 31);
			time.day = 1 + days % 31;
			return time;
		}

		--days;
	}

	time.month = 1;
	ushort days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	ulong i = 0;

	while(days > days_in_month[i])
	{
		++time.month;
		days -= days_in_month[i++];
	}

	time.day = (utiny) (1 + days);
	return time;
}
