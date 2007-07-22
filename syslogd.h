/* common header for syslogd
 * Copyright 2007 Rainer Gerhards and Adiscon GmbH.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * A copy of the GPL can be found in the file "COPYING" in this distribution.
 */
#ifndef	SYSLOGD_H_INCLUDED
#define	SYSLOGD_H_INCLUDED 1

#include "syslogd-types.h"

#ifdef USE_NETZIP
#include <zlib.h>
/* config param: minimum message size to try compression. The smaller
 * the message, the less likely is any compression gain. We check for
 * gain before we submit the message. But to do so we still need to
 * do the (costly) compress() call. The following setting sets a size
 * for which no call to compress() is done at all. This may result in
 * a few more bytes being transmited but better overall performance.
 * Note: I have not yet checked the minimum UDP packet size. It might be
 * that we do not save anything by compressing very small messages, because
 * UDP might need to pad ;)
 * rgerhards, 2006-11-30
 */
#define	MIN_SIZE_FOR_COMPRESS	60
#endif

#define	MAXLINE		2048		/* maximum line length */

#ifdef SYSLOG_INET
#define INET_SUSPEND_TIME 60		/* equal to 1 minute 
					 * rgerhards, 2005-07-26: This was 3 minutes. As the
					 * same timer is used for tcp based syslog, we have
					 * reduced it. However, it might actually be worth
					 * thinking about a buffered tcp sender, which would be 
					 * a much better alternative. When that happens, this
					 * time here can be re-adjusted to 3 minutes (or,
					 * even better, made configurable).
					 */
#define INET_RETRY_MAX 30		/* maximum of retries for gethostbyname() */
	/* was 10, changed to 30 because we reduced INET_SUSPEND_TIME by one third. So
	 * this "fixes" some of implications of it (see comment on INET_SUSPEND_TIME).
	 * rgerhards, 2005-07-26
	 */
#endif
#if defined(__GLIBC__)
#define dprintf mydprintf
#endif /* __GLIBC__ */

void dprintf(char *, ...);
void logerror(char *type);
void logerrorSz(char *type, char *errMsg);
void logerrorInt(char *type, int iErr);

void printchopped(char *hname, char *msg, int len, int fd, int iSourceType);
void freeAllSockets(int **socks);
int isAllowedSender(struct AllowedSenders *pAllowRoot, struct sockaddr *pFrom, const char *pszFromHost);
char *getFwdSyslogPt(selector_t *f);
void getCurrTime(struct syslogTime *t);
int formatTimestampToMySQL(struct syslogTime *ts, char* pDst, size_t iLenDst);
int formatTimestamp3339(struct syslogTime *ts, char* pBuf, size_t iLenBuf);
int formatTimestamp3164(struct syslogTime *ts, char* pBuf, size_t iLenBuf);
void  iovCreate(selector_t *f);
char *iovAsString(selector_t *f);

extern int glblHadMemShortage; /* indicates if we had memory shortage some time during the run */
extern syslogCODE rs_prioritynames[];
extern syslogCODE rs_facilitynames[];
extern char LocalHostName[];
extern int family;
extern int NoHops;
extern int *finet;
extern int send_to_all;
extern int option_DisallowWarning;
extern int Debug;
extern char**LocalHosts;
extern int DisableDNS;
extern char **StripDomains;
extern char *LocalDomain;
extern int bDropMalPTRMsgs;
extern struct AllowedSenders *pAllowedSenders_TCP;

#endif /* #ifndef SYSLOGD_H_INCLUDED */
