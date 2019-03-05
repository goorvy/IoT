/*
Convert Unix TimeStamp (seconds from 01.01.1970) to human readable date-time format.

*/

#ifndef __UNIXTIMEPARSE_H__
	#define __UNIXTIMEPARSE_H__
#endif

#define SECONDS_IN_MINUTE 60
#define SECONDS_IN_HOUR 3600
#define SECONDES_IN_DAY 86400
#define SECONDS_IN_NON_LEAP_YEAR 31536000
#define SECONDS_IN_4_YEARS 126230400
#define YEAR_FROM 1970

const char monthtable[2][12] = {{31,28,31,30,31,30,31,31,30,31,30,31}, {31,29,31,30,31,30,31,31,30,31,30,31}};

struct Date {
    int year;
    char month;
    char day;
    char hour;
    char minute;
    char second;
    char day_of_the_week;
};

class UnixTimestampParse {
    private:
        
        bool isleapyear(int y) {
            if (   ( (y % 4) == 0 ) || (  ( (y % 100) == 0 ) && ( (y % 400) == 0 )  )   )
                return true;
            else
                return false;
        }
        
        //returns a number corresponding to the day of the week (0-7 is Sunday to Saturday)
        char dow( int y, char m, char d ) {
            char t[] = {0,3,2,5,0,3,5,1,4,6,2,4};
            if ( m < 3 ) {
                y = y - 1;
            }
            return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7; // 0=Воскресенье!!! (1-Понедельник)
        }
        
    public:
        void parse(Date *date, unsigned long unix_timestamp, char time_zone) {
            unix_timestamp = unix_timestamp + time_zone*SECONDS_IN_HOUR;
            date->year = YEAR_FROM + unix_timestamp/SECONDS_IN_4_YEARS*4 + unix_timestamp%SECONDS_IN_4_YEARS/SECONDS_IN_NON_LEAP_YEAR;
            int days = unix_timestamp%SECONDS_IN_4_YEARS%SECONDS_IN_NON_LEAP_YEAR/SECONDES_IN_DAY;
            date->month = 1;
            while ( days > monthtable[isleapyear(date->year)][date->month-1] ) {
                days = days - monthtable[isleapyear(date->year)][date->month-1];
                date->month = date->month + 1;
            }
            date->day = days + 1;
            int remain_seconds_after_day = unix_timestamp%SECONDS_IN_4_YEARS%SECONDS_IN_NON_LEAP_YEAR%SECONDES_IN_DAY;
            date->hour = remain_seconds_after_day/SECONDS_IN_HOUR;
            date->minute = remain_seconds_after_day%SECONDS_IN_HOUR/SECONDS_IN_MINUTE;
            date->second = remain_seconds_after_day%SECONDS_IN_HOUR%SECONDS_IN_MINUTE;
            date->day_of_the_week = dow(date->year, date->month, date->day);
        }
};