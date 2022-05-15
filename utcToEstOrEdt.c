#include <stdio.h>
#include <time.h>

void UTC_TO_EST_CONVERTER(char* dateField)
{
    int DSF=-1;
    time_t t1, t2, t3;
    struct tm tm = {0};
    char *s = strptime(dateField,"%Y-%m-%dT%H:%M:%SZ",&tm);
    struct tm day = { tm.tm_sec, tm.tm_min, tm.tm_hour,
                      tm.tm_mday, tm.tm_mon, tm.tm_year};    
    
    DSF = IS_EST_OR_EDT(tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour);
    t1 = mktime(&day);
    
    if(DSF)
    {
       //For Daylight Saving ON 
       t2 = t1 - (3600 * 4);
    }
    else
    {
        //For Daylight Saving OFF
        t2 = t1 - (3600 * 5);
    }
    struct tm *ptm = gmtime(&t2);    
    
    sprintf(dateField,"%d-%02d-%02dT%02d:%02d:%02dZ",ptm->tm_year+1900,ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}

int IS_EST_OR_EDT(int year, int mon, int mday, int hour)
{
    char buf[50]={'\0'};
    struct tm tm = {0};
    sprintf(buf,"%d%s%s",year,"03","15");
    strptime(buf,"%Y%m%d",&tm);
    int SecSunMarch = tm.tm_mday - tm.tm_wday;
    SecSunMarch = (SecSunMarch % 7) + 7; //2nd Sunday of March
    
    sprintf(buf,"%d%s%s",year,"11","15");
    strptime(buf,"%Y%m%d",&tm);
    int FirstSunNov = tm.tm_mday- tm.tm_wday;
    FirstSunNov = (FirstSunNov % 7); //1st Sunday of November
    
    if(mon >= 4 && mon <= 10)
    {
        return 0; //EDT (UTC-4)
    }
    else if(mon==12 || mon==1 || mon==2)
    {
        return 1; //EST (UTC-5)
    }
    else if(mon==3)
    {
        if(mday < SecSunMarch)
        {
            return 1; //EST
        }
        else if(mday > SecSunMarch)
        {
            return 0; //EDT
        }
        else if(mday == SecSunMarch)
        {
            if(hour < 2)
            {
                return 1; //EST
            }
            else
            {
                return 0; //EDT
            }
        }
    }
    else if(mon==11)
    {
        if(mday < FirstSunNov)
        {
            return 0; //EDT
        }
        else if(mday > FirstSunNov)
        {
            return 1; //EST
        }
        else if(mday == FirstSunNov)
        {
            if(hour < 2)
            {
                return 0; //EDT
            }
            else
            {
                return 1; //EST
            }
        }
    }
    
}

int main(){
    char dateField[]={"2022-11-06T03:00:00Z"};
    UTC_TO_EST_CONVERTER(dateField);
    printf("%s",dateField);
    return 0;
}
