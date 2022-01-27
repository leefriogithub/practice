/******************************************************************************/
/*  Name        : util.c                                                      */
/*  Description : get system date or time                                     */
/*  Copyright   : Futures Brain Com.,LTD                                      */
/*  Date        : 2009.05.25                                                  */
/*  Remark      :                                                             */
/*                                                                            */
/******************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>        /* gmtime */
#include <sys/time.h>    /* 2021.08.04 appended for gettimeofday */
#include <sys/types.h>
#include <sys/stat.h>

#define uint 		unsigned int 
#define uchar		unsigned char 

#ifndef MAP_FILE
#define MAP_FILE	0
#endif

/* Function Prototypes */
int GetFIXUTCTime(char *out);
int GetDateUTime(char *pOut);
int GetDateTime(char *pOut);
int GetFDate(char *pOut);
int GetDate(char *pOut);
int GetUTime(char *pOut);
int GetMTime(char *pOut);
int GetFTime(char *pOut);
int GetTime(char *pOut);
int GetHMTime(char *pOut);
int GetNumDay(char *inday);
int GetAbsPath(const char *pEnv, const char *pName, char *pPath);
int ReadLine(int nFd, char *pOut);
int LogProc(const char *pNmae, const char *fmt, ...);
int FindString(const char *pSource, const char *pFind);
int GetField(char *pName, char *pSection, char *pField, char *pOut);
int SetField(char *pName, char *pSection, char *pField, char *pIn);
int ToUpperString(char *pIn, char *pOut, int nSize);
int ToLowerString(char *pIn, char *pOut, int nSize);
int StrToLong(const char *pIn, int nSize);
int RTrim(const char *pIn, char *pOut);
int LTrim(const char *pIn, char *pOut);
int Trim(const char *pIn, char *pOut);
static uchar GetRandChar();
void Encryption(uchar *pIn, uchar *pOut);
void Decryption(uchar *pIn, uchar *pOut);
char *MemoryMap(int nFd, int mode);
int MemoryUnMap(int nFd, char *pPos);
char *MMap(int fd, int fsize, int offset);
int MUnMap(char *addr, int fsize);
int GetIniRightValue(char *fname, char *section, char *lval, char *rval);
int CompareStringToken(const char *s1, const char *s2);
int GetSeqFromFile(const char *fname, int *seq);
int SetSeqToFile(const char *fname, const char *seq);
char *ToUpper(const char *in);
int GetLineStat(const char *fname, int *stat);
int SetLineStat(const char *fname, int stat);
int SMSLog(char *prcenm, char *errtp, char *errcd, char *errmsg);

int CompareStringToken2(const char *s1, const char *s2); /* 2021.08.04 appended */
int GetOrgStr(const char *src, char *des); /* 2021.08.04 appended */

int GetFIXUTCTime(char *out)
{
	struct timeval t;
	struct tm *tm;

	gettimeofday(&t, NULL);
	tm = gmtime(&t.tv_sec);

	sprintf(out, "%04d%02d%02d-%02d:%02d:%02d",
		tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec);

	return 0;
}

/* get system date & time YYYYMMDDHHMMSSssssss */
int GetDateUTime(char *pOut)
{
	struct timeval t;
	struct tm *tm;

	gettimeofday(&t, NULL);
	tm = localtime(&t.tv_sec);
	sprintf(pOut,"%04d%02d%02d%02d%02d%02d%06d",
			tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec, t.tv_usec);
	return 0;
}

/* get system date & time YYYYMMDDHHMMSS */
int GetDateTime(char *pOut)
{
	time_t t;
	struct tm *tm;

	/* YYYYMMDDmmss */
	time(&t);
	tm = localtime(&t);
	sprintf(pOut, "%04d%02d%02d%02d%02d%02d",
			tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec);
	return 0;
}

/* get system date YYYY/MM/DD */
int GetFDate(char *pOut)
{
	time_t t;
	struct tm *tm;

	/* YYYYMMDD */
	time(&t);
	tm = localtime(&t);
	sprintf(pOut, "%04d/%02d/%02d",
			tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
	return 0;
}

/* get system date YYYYMMDD */
int GetDate(char *pOut)
{
	time_t t;
	struct tm *tm;

	/* YYYYMMDD */
	time(&t);
	tm = localtime(&t);
	sprintf(pOut, "%04d%02d%02d",
			tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
	return 0;
}

/* get system time HHMMSSmmmmmm */
int GetUTime(char *pOut)
{
	struct timeval t;
	struct tm *tm;

	gettimeofday(&t, NULL);
	tm = localtime(&t.tv_sec);
	sprintf(pOut,"%02d%02d%02d%06d",
			tm->tm_hour, tm->tm_min, tm->tm_sec, t.tv_usec);

	return 0;
}

/* get system time HHMMSSmm */
int GetMTime(char *pOut)
{
	struct timeval t;
	struct tm *tm;

	gettimeofday(&t, NULL);
	tm = localtime(&t.tv_sec);
	sprintf(pOut,"%02d%02d%02d%02d",
			tm->tm_hour, tm->tm_min, tm->tm_sec, t.tv_usec/10000);

	return 0;
}

/* get system time HH:MM:SS */
int GetFTime(char *pOut)
{
	time_t t;
	struct tm *tm;

	/* HHMMSS */
	time(&t);
	tm = localtime(&t);
	sprintf(pOut,"%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

	return 0;
}
/* get system time HHMMSS */
int GetTime(char *pOut)
{
	time_t t;
	struct tm *tm;

	/* HHMMSS */
	time(&t);
	tm = localtime(&t);
	sprintf(pOut,"%02d%02d%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

	return 0;
}

/* get system time HHMM */
int GetHMTime(char *pOut)
{
	time_t t;
	struct tm *tm;

	/* HHMMSS */
	time(&t);
	tm = localtime(&t);
	sprintf(pOut,"%02d%02d", tm->tm_hour, tm->tm_min);

	return 0;
}

/* 1: 월요일 ~ */
int GetNumDay(char *inday)
{
	char        tmp[10];
	char        wday[4];
	time_t      ctt;
	struct tm   ctm;

	memset((char *)&ctm, '\0', sizeof(ctm));
	memset((char *)&ctt, '\0', sizeof(ctt));

	sprintf(tmp, "%4.4s", inday);
	ctm.tm_year = atoi(tmp) - 1900;

	sprintf(tmp, "%2.2s", inday+4);
	ctm.tm_mon =  atoi(tmp) - 1;

	sprintf(tmp, "%2.2s", inday+6);
	ctm.tm_mday = atoi(tmp);

	ctt = mktime(&ctm);
	strftime(wday, sizeof(wday), "%w", localtime(&ctt));

	return atoi(wday);
}

/* get absulutly path name */
int GetAbsPath(const char *pEnv, const char *pName, char *pPath)
{
	char *pValue;

	pValue = getenv(pEnv);
	if(pValue == NULL)
	{
		*pPath = '\0';
		return 0;
	}

	sprintf(pPath, "%s/%s", pValue, pName);

	return strlen(pValue);
}

int ReadLine(int nFd, char *pOut)
{
	int nPos = 0;
	int nRet;
	char str[1024];

	while(1)
	{
		nRet = read(nFd, &str[nPos], 1);
		if(nRet < 0)
		{
			return -1;
		}

		if(nRet == 0)
		{
			pOut[0] = '\0';
			return 0;
		}

		if(str[nPos] == '\n' || str[nPos] == '\r')
		{
			if(nPos > 0)
			{
				str[nPos+1] = '\0';
				strcpy(pOut, str);
				break;
			}

			nPos = 0;
			continue;
		}

		nPos++;
	}

	return nPos;
}

#ifdef SFDASDFASFASFDASDFASFDAASDFFASASDFSDASFSDSDDSFASDFFASD
int Log(const char *pName, int line, const char *fmt, ...)
{
	int	fd;
	int	len;
	va_list	ap;
	struct timeval  t;
	struct tm       *tm;
	char	str[4096 * 10];
	char    path[100];
	char    logdt[10];

	memset(path, '\0', sizeof(path));

	/* YYYYMMDDhhmmss PID Grade */
	gettimeofday(&t, NULL);
	tm = localtime(&t.tv_sec);

	sprintf(logdt, "%02d%02d", tm->tm_mon+1, tm->tm_mday);
	sprintf(path, "%s_%s", pName, logdt);

/*******
	len = sprintf(str, "[%02d%02d %02d%02d%02d %06d](%04d) ",
		tm->tm_mon+1, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec, t.tv_usec, line);
*******/

	len = sprintf(str, "[%02d:%02d:%02d.%03d](%04d) ",
		tm->tm_hour, tm->tm_min, tm->tm_sec, t.tv_usec/1000, line);

	va_start(ap, fmt);
	len = vsprintf(str+len, fmt, ap) + len;
	va_end(ap);

	fd = open(path, O_WRONLY|O_CREAT|O_APPEND, 0666);
	if(fd >= 0)
	{ 
		write(fd, str, len);
		close(fd);
	}

	return 0;
}
#endif

#ifdef SFDASDFASFASFDASDFASFDAASDFFASASDFSDASFSDSDDSFASDFFASD
/*   <--- 2021.08.04 appended.
=========================== Error File Information ==============================
Error File Name >
        /JOBLOG/SMS/error_log ( 디렉토리 및 파일은 정하세요... )
Format >

        Date|Time|hostname|Biz_Group|Proc_name|Status|Msg_id|message|@END

        Data  ==>  yyyymmdd
        Time  ==>  hh:mm:ss.99
        Hostname ==> {rsum365p|rsum365s}
        Biz_Group ==> {AC|BK..|COM|HIQ..}
        Proc_name ==> Process name
        Status ==> ERR
        Msg_id ==> 99999
        Message ==> 50 Bytes 이내   (휴대폰 전송 TEXT)
        suffix ==> @END


Example)
20091016|09:00:30|rssm_fep_dev|FEP|프로세스NAME|ERR|00000|[성공] GL CONNECTION 실패입니다|@END

=> Status  "ERR" 인 경우만 SMS로 전송 됨.  운영자 알아야할 정보성 메시지 "INF" 로 하면 됩니다.   

이런 예로 error_log 파일에 write 하시면 됩니다.. */
*/
#endif

int SMSLog(char *prcenm, char *errtp, char *errcd, char *errmsg)
{
	int	fd;
	time_t t;
	struct tm       *tm;
	char    hostnm[30];
	char    smstime[20];
	char    path[100];
	char    smsbuf[200];
	char    newline[2];

	time(&t);
	tm = localtime(&t);
	sprintf(smstime, "%04d%02d%02d%02d%02d%02d",
			tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec);

	sprintf(path, "%s/log/smslog/error_log.%8.8s", getenv("FBDIR"), smstime);

	gethostname(hostnm, sizeof(hostnm));

	/**
	20091016|09:00:30|rssm_fep_dev|FEP|프로세스NAME|ERR|00000|[성공] GL CONNECTION 실패입니다|@END
	***/

	memset(smsbuf, '\0', sizeof(smsbuf));

	sprintf(smsbuf, "%8.8s|%2.2s:%2.2s:%2.2s|%s|FEP|%s|%s|%s|%-50.50s|@END\n",
		smstime, smstime+8, smstime+10, smstime+12, hostnm, prcenm, errtp, errcd, errmsg);

	newline[0] = '\n';

	fd = open(path, O_WRONLY|O_CREAT|O_APPEND, 0666);
	if(fd >= 0)
	{ 
		write(fd, smsbuf, strlen(smsbuf));
		close(fd);
	}

	return 0;
}

/* print log message */
int LogProc(const char *pName, const char *fmt, ...)
{
	int	fd;
	int	len;
	va_list	ap;
	struct timeval  t;
	struct tm       *tm;
	char	str[4096 * 10];
	char    path[100];
	char    logdt[10];

	/* YYYYMMDDhhmmss PID Grade */
	gettimeofday(&t, NULL);
	tm = localtime(&t.tv_sec);

	sprintf(logdt, "%02d%02d", tm->tm_mon+1, tm->tm_mday);
	sprintf(path, "%s_%s", pName, logdt);

/*********
	len = sprintf(str, "[%02d%02d %02d%02d%02d %06d] ",
		tm->tm_mon+1, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec, t.tv_usec);
**********/

	len = sprintf(str, "[%02d:%02d:%02d.%03d] ",
		tm->tm_hour, tm->tm_min, tm->tm_sec, t.tv_usec/1000);

	va_start(ap, fmt);
	len = vsprintf(str+len, fmt, ap) + len;
	va_end(ap);

	fd = open(path, O_WRONLY|O_CREAT|O_APPEND, 0666);
	if(fd >= 0)
	{ 
		write(fd, str, len);
		close(fd);
	}

	return 0;
}

int FindString(const char *pSource, const char *pFind)
{
	int i;
	int nLen_1;
	int nLen_2;

	nLen_1 = strlen(pSource);
	nLen_2 = strlen(pFind);

	if(!memcmp(pSource, pFind, nLen_2))
		return 1;
/***
	for(i = 0; i < (nLen_1 - nLen_2); i++)
	{
		if(!memcmp(pSource+i, pFind, nLen_2))
			return 1;

		continue;
	}
***/
	return 0;
}

#ifdef laskfjklsadjf
int GetField(char *pName, char *pSection, char *pField, char *pOut)
{
	int i;
	int nRet = 0;
	char sSection[100];
	char sField[1024];
	char sBuf[1024];
	char sTmp[1024];
	char sArg[10][100];

	FILE *pFd;

	pFd = fopen(pName, "r");
	if(pFd == NULL)
		return -1;

	/* find section */
	sprintf(sSection, "[%s]", pSection);

	while(1)
	{
		memset(sBuf, '\0', sizeof(sBuf));
		if(fgets(sBuf, sizeof(sBuf), pFd) == NULL)
			break;

		if(sBuf[0] == '#')
			continue;

		if(!(nRet = FindString(sBuf, sSection)))
			continue;

		break;
	}

	if(!nRet)
	{
		fclose(pFd);
		return -2;
	}

	/* find field */
	while(1)
	{
		memset(sBuf, '\0', sizeof(sBuf));
		if(fgets(sBuf, sizeof(sBuf), pFd) == NULL)
			break;

		if(sBuf[0] == '#')
			continue;

		for(i = 0; i < strlen(sBuf); i++)
		{
			if(sBuf[i] == '=')
			{
				nRet = 1;
				break;
			}
			else if(sBuf[i] == '[')
			{
				nRet = 0;
				break;
			}
		}

		if(i == strlen(sBuf))
			continue;

		if((nRet = FindString(sBuf, pField)))
			break;
	}
	fclose(pFd);

	if(!nRet)
		return -3;

	strcpy(sField, &sBuf[i+1]);

	nRet = sscanf(sField, "%s%s%s%s%s%s%s%s%s%s",
		&sArg[0], &sArg[1], &sArg[2], &sArg[3], &sArg[4],
		&sArg[5], &sArg[6], &sArg[7], &sArg[8], &sArg[9]);


	memset(sTmp, '\0', sizeof(sTmp));
	for(i = 0; i <  nRet; i++)
	{
		strcat(sTmp, sArg[i]);
		if(i != (nRet - 1))
			strcat(sTmp, " ");
	}
	strcpy(pOut, sTmp);

	return 1;
}
#endif

int GetField(char *fname, char *section, char *lval, char *rval)
{
	int     i;
	int     n = 0;
	char    buff[1024];
	char    temp[1024];
	char    schar[100];
	char    rchar[1024];
	char    aval[10][100];
	FILE    *fp;

	fp = fopen(fname, "r");
	if(fp == NULL)
		return -1;

	sprintf(schar, "[%s]", section);

	n = 0;
	while(1)
	{
		memset(buff, '\0', sizeof(buff));
		if(fgets(buff, sizeof(buff), fp) == NULL)
			break;

		if(buff[0] == '#' || buff[0] == '\n' ||
			buff[0] == '\r' || buff[0] == '\t')
			continue;

		if(buff[0] == '[')
		{
			n = CompareStringToken2(buff, schar);
			if(n)
				break;
		}
	}

	if(!n)
	{
		fclose(fp);
		return n;
	}

	n = 0;

	while(1)
	{
		memset(buff, '\0', sizeof(buff));
		if(fgets(buff, sizeof(buff), fp) == NULL)
			break;

		if(buff[0] == '#' || buff[0] == '\n' ||
			buff[0] == '\r' || buff[0] == '\t')
			continue;

		if(buff[0] == '[')
			break;

		for(i = 0; i < strlen(buff); i++)
		{
			if(buff[i] == '=')
			{
				memset(temp, '\0', sizeof(temp));
				sprintf(temp, "%*.*s", i, i, buff);

				n = CompareStringToken2(temp, lval);
				break;
			}
		}

		if(n) break;
	}

	fclose(fp);

	if(!n) return n;

	memset(rchar, '\0', sizeof(rchar));
	strcpy(rchar, &buff[i+1]);

	n = sscanf(rchar, "%s%s%s%s%s%s%s%s%s%s",
		&aval[0], &aval[1], &aval[2], &aval[3],
		&aval[4], &aval[5], &aval[6], &aval[7], &aval[8], &aval[9]);

	memset(temp, '\0', sizeof(temp));
	for(i = 0; i <  n; i++)
	{
		strcat(temp, aval[i]);
		if(i != (n - 1))
			strcat(temp, " ");
	}
	strcpy(rval, temp);

	return 1;
}

int CompareStringToken2(const char *s1, const char *s2)
{
	int	  alen;
	int	  tlen;
	char  tmps1[1024];
	char  tmps2[1024];

	memset(tmps1, '\0', sizeof(tmps1));
	memset(tmps2, '\0', sizeof(tmps2));

	alen = GetOrgStr(s1, tmps1);
	tlen = GetOrgStr(s2, tmps2);

	if(alen != tlen)
		return 0;

	if(!memcmp(s1, s2, tlen))
		return 1;

	return 0;
}

int GetOrgStr(const char *src, char *des)
{
	int	i;
	int	j;
	int	len;

	len = strlen(src);
	des[0] = '\0';

	for(i = len-1, j = 0; i >= 0; i--)
	{
		if( src[i] == ' ' || src[i] == '\n' || 
			src[i] == '\r' || src[i] == '\t') 
		{
			j++;
			continue;
		}

		memcpy(des, src, len-j);
		des[len-j] = '\0';
		break;
	}

	return len-j;
}
int SetField(char *pName, char *pSection, char *pField, char *pIn)
{
	int nRet = 0;
	char sSection[100];
	char sBuf[1024];

	FILE *pFd;

	pFd = fopen(pName, "r+w");
	if(pFd == NULL)
		return -1;

	/* find section */
	sprintf(sSection, "[%s]", pSection);

	while(1)
	{
		memset(sBuf, '\0', sizeof(sBuf));

		if(fgets(sBuf, sizeof(sBuf), pFd) == NULL)
			break;

		if(sBuf[0] == '#')
			continue;

		if(!(nRet = FindString(sBuf, sSection)))
			continue;

		break;
	}

	if(!nRet)
	{
		fclose(pFd);
		return nRet;
	}

	while(1)
	{
		memset(sBuf, '\0', 1024) ;

		if(fgets(sBuf, 1024, pFd)==NULL )
			break ;

		if( sBuf[0] =='#')
			continue ;

		if( sBuf[0] =='[')
			continue ;

		if( FindString(sBuf, pField))
		{
			fseek(pFd, -(strlen(pIn) + 1), SEEK_CUR);
			fprintf(pFd, "%-*.*s", strlen(pIn), strlen(pIn), pIn);
			fseek(pFd, strlen(pIn) + 1, SEEK_CUR);
			break ;
		}
	}

	fclose(pFd);

	return 1;
}

int ToUpperString(char *pIn, char *pOut, int nSize)
{
	int i;

	for(i = 0; i < nSize; i++)
	{
		pOut[i] = toupper(pIn[i]);
	}
	pOut[i] = '\0';

	return 0;
}

int ToLowerString(char *pIn, char *pOut, int nSize)
{
	int i;

	for(i = 0; i < nSize; i++)
	{
		pOut[i] = tolower(pIn[i]);
	}
	pOut[i] = '\0';

	return 0;
}

int StrToLong(const char *pIn, int nSize)
{
        return strtol(pIn, NULL, nSize);
}

int LTrim(const char *pIn, char *pOut)
{
	int i;
	int nLen;

	nLen = strlen(pIn);
	pOut[0] = '\0';

	for(i = 0; i < nLen; i++)
	{
		if(pIn[i] == ' ') 
			continue;

		memcpy(pOut, &pIn[i], nLen - i);
		pOut[nLen - i] = '\0';
		break;
	}

	return (nLen - i);
}

int RTrim(const char *pIn, char *pOut)
{
	int i, j;
	int nLen;

	nLen = strlen(pIn);
	pOut[0] = '\0';

	for(i = nLen - 1, j = 0; i >= 0; i--)
	{
		if(pIn[i] == ' ') 
		{
			j++;
			continue;
		}

		memcpy(pOut, pIn, nLen - j);
		pOut[nLen - j] = '\0';
		break;
	}

	return (nLen - j);
}

int Trim(const char *pIn, char *pOut)
{
	int nRet;
	char sTmp[1024];

	pOut[0] = '\0';
	nRet = LTrim(pIn, sTmp);
	if(nRet == 0)
		return 0;

	return RTrim(sTmp, pOut);
}

static uchar GetRandChar()
{
	time_t ltime;
	uchar	ch;

	time(&ltime);

	ltime += rand();

	srand(ltime);

	ch=(uchar)((rand() % 20) + 68);

	return ch;
}

void Encryption(uchar *pIn, uchar *pOut)
{
	short i;
	short len;
	char sBuf[128] = {0};
	char *p = sBuf;

	len=strlen((char *)pIn);

	for(i = 0; i < len; i++)
	{
		uchar	ch;

		ch = GetRandChar();
		*p ++= (uchar)(((*pIn & 0xc0) >> 6) | (ch & 0xfc));

		ch = GetRandChar();
		*p ++= (uchar)(((*pIn & 0x30) >> 4) | (ch & 0xfc));

		ch = GetRandChar();
		*p ++= (uchar)(((*pIn & 0x0c) >> 2) | (ch & 0xfc));

		ch = GetRandChar();
		*p ++= (uchar)((*pIn & 0x03) | (ch & 0xfc));

		pIn++;
	}

	len = strlen(sBuf);

	for(i = 1; i <= len; i++)
		*pOut ++= sBuf[len-i];
}

void Decryption(uchar *pIn, uchar *pOut)
{
	short i;
	short nLen;
	char sBuf[128] = {0};
	char *p = sBuf;

	nLen = strlen((char *)pIn); 

	for(i=1; i <= nLen; i++)
		sBuf[nLen-i] = *pIn++;

	for(i=0; i < nLen; i++)
	{
		uchar	ch[4];
		uchar	Bit[4];

		ch[0]=(*p++ & 0x03) << 6;
		Bit[0] = (char)(((rand() % 23) + 8) * 128 + 189);

		ch[1]=(*p++ & 0x03) << 4;
		Bit[1] = (char)(((rand() % 123) + 8) * 128 + 892);

		ch[2]=(*p++ & 0x03) << 2;
		Bit[2] = (char)(((rand() % 13) + 8) * 128 + 679);

		ch[3]=(*p++ & 0x03);
		Bit[3] = (char)(((rand() % 99) + 8) * 128 + 208);

		*pOut++ = ch[0] | ch[1] | ch[2] | ch[3];
		*pOut = ch[0] | Bit[0]| ch[1] | Bit[1] | ch[2] | Bit[2] | ch[3] | Bit[3];
	}

	*pOut = 0x00;
}

char *MemoryMap(int nFd, int mode)
{
	int nRet;
	char *pPos;
	struct stat statbuf;

	nRet = fstat(nFd, &statbuf);
	if(nRet < 0)
	{
		return NULL;
	}

	/* memory mapped */
	pPos = mmap(0, statbuf.st_size, mode,
			MAP_FILE | MAP_SHARED, nFd, 0);
	if(pPos == (caddr_t) -1)
	{
		return NULL;
	}

	return pPos;
}

int MemoryUnMap(int nFd, char *pPos)
{
	int nRet;
	struct stat statbuf;

	nRet = fstat(nFd, &statbuf);
	if(nRet < 0)
	{
		return -1;
	}

	/* memory unmapped */
	nRet = munmap((caddr_t)pPos, statbuf.st_size);
	if(nRet < 0)
	{
		return -2;
	}

	return 0;
}

char *MMap(int fd, int fsize, int offset)
{
	char *addr;

	addr = mmap(0, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
	if(addr == (void *) -1)
		return NULL;

	return addr;
}

int MUnMap(char *addr, int fsize)
{
	int n;

	n = munmap((void *)addr, fsize);
	if(n < 0)
		return -1;

	return 0;
}

int GetIniRightValue(char *fname, char *section, char *lval, char *rval)
{
        int     i;
        int     n = 0;
        char    buff[1024];
        char    temp[1024];
        char    schar[100];
        char    rchar[1024];
        char    lchar[1024];
        char    aval[10][100];
        FILE    *fp;

        fp = fopen(fname, "r");
        if(fp == NULL)
                return -1;

        sprintf(schar, "[%s]", section);

        n = 0;
        while(1)
        {
                memset(buff, '\0', sizeof(buff));
                if(fgets(buff, sizeof(buff), fp) == NULL)
                        break;

                if(buff[0] == '#')
                        continue;

                if(!(n = CompareStringToken(buff, schar)))
                        continue;
                break;
        }

        if(!n)
        {
                fclose(fp);
                return n;
        }
        while(1)
        {
                memset(buff, '\0', sizeof(buff));
                if(fgets(buff, sizeof(buff), fp) == NULL)
                        break;

                if(buff[0] == '#')
                        continue;

                for(i = 0; i < strlen(buff); i++)
                {
                        if(buff[i] == '=')
                        {
                                n = 1;
                                break;
                        }

                        else if(buff[i] == '[')
                        {
                                n = 0;
                                break;
                        }
                }

                if(i == strlen(buff))
                        continue;

                if((n = CompareStringToken(buff, lval)))
                        break;
        }

        fclose(fp);
        if(!n)
                return n;

        strcpy(rchar, &buff[i+1]);

        n = sscanf(rchar, "%s%s%s%s%s%s%s%s%s%s",
                &aval[0], &aval[1], &aval[2], &aval[3],
                &aval[4], &aval[5], &aval[6], &aval[7], &aval[8], &aval[9]);

        memset(temp, '\0', sizeof(temp));
        for(i = 0; i <  n; i++)
        {
                strcat(temp, aval[i]);
                if(i != (n - 1))
                        strcat(temp, " ");
        }
        strcpy(rval, temp);

        return 1;
}

int CompareStringToken(const char *s1, const char *s2)
{
	int i;
	int alen;
	int tlen;
	
	alen = strlen(s1);
	tlen = strlen(s2);
	
	if(!memcmp(s1, s2, tlen))
		return 1;
	
	return 0;
}

int GetSeqFromFile(const char *fname, int *seq)
{
	int     num;
	char    str[100];
	char    spastr[100];
	char    zerostr[100];
	FILE    *fp;
	
	if((fp = fopen(fname, "r")) == NULL)
		return -1;
	
	memset(str, '\0', sizeof(str));
	memset(spastr, ' ', sizeof(spastr));
	memset(zerostr, '0', sizeof(zerostr));
	
	if(fgets(str, 100, fp) == NULL)
	{
		fclose(fp);
		return -2;
	}
	
	if(str[strlen(str)-1] == '\r' || str[strlen(str)-1] == '\n')
		str[strlen(str)-1] = '\0';
	
	num = atoi(str);
	if(num <= 0)
	{
		if( !memcmp(str, spastr, strlen(str)) ||
			!memcmp(str, zerostr, strlen(str)) )
		{
			*seq = 0;
		}
		else
		{
			fclose(fp);
			return -3;
		}
	}
	else
	{
		*seq = num;
	}
	
	fclose(fp);
	
	return num;
}

int SetSeqToFile(const char *fname, const char *seq)
{
	int     n;
	int     fd;
	
	if((fd = open(fname, O_RDWR)) <  0)
	        return -1;
	
	n = write(fd, seq, strlen(seq));
	if(n <= 0)
	{
		close(fd);
		return -2;
	}
	
	close(fd);
	
	return n;
}

char *ToUpper(const char *in)
{
	int             i;
	int             len;
	static char     str[1024];
	
	len = strlen(in);
	for(i = 0; i < len; i++)
	{
		str[i] = toupper(in[i]);
	}
	str[len] = '\0';

	return str;
}

int GetLineStat(const char *fname, int *stat)
{
	int     num;
	char    str[100];
	char    spastr[100];
	char    zerostr[100];
	FILE    *fp;

	if((fp = fopen(fname, "r")) == NULL)
		return -1;

	memset(str, '\0', sizeof(str));
	memset(spastr, ' ', sizeof(spastr));
	memset(zerostr, '0', sizeof(zerostr));

	if(fgets(str, 100, fp) == NULL)
	{
		fclose(fp);
		return -2;
	}

	if(str[strlen(str)-1] == '\r' || str[strlen(str)-1] == '\n')
		str[strlen(str)-1] = '\0';

	num = atoi(str);
	if(num <= 0)
	{
		if( !memcmp(str, spastr, strlen(str)) ||
		    !memcmp(str, zerostr, strlen(str)) )
		{
			*stat = 0;
		}
		else
		{
			fclose(fp);
			return -3;
		}
	}
	else
	{
		*stat = num;
	}

	fclose(fp);

	return num;
}

int SetLineStat(const char *fname, int stat)
{
	int     n;
	int     fd;
	char	str[20];

	if((fd = open(fname, O_RDWR)) <  0)
		return -1;

	memset(str, '\0', sizeof(str));

	sprintf(str, "%d",stat);	
	n = write(fd, str, strlen(str));
	if(n <= 0)
	{
		close(fd);
		return -2;
	}

	close(fd);

	return n;
}

/******************************************************************************/
/*                            End of File                                     */
/******************************************************************************/
