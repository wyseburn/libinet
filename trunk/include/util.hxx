/**
 *  Version:     @(#)libinet/util.hxx    0.0.2 08/06/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     
 *
 *  This library is free software; you can redistribute it and/or modify it under 
 *  the terms of the GNU Lesser General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) any 
 *  later version.
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT ANY 
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 *  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along 
 *  with this library; if not, write to the Free Software Foundation, Inc., 
 *  59 Temple  Place, Suite 330, Boston, MA 02111-1307  USA
 */

#ifndef __LIBINET_UTIL_H__
#define __LIBINET_UTIL_H__

#define INET_QUEUE_HEAD(name, type)                                              \
struct name {                                                                    \
        struct type *qh_first;	/* first element */                              \
        struct type **qh_last;	/* addr of last next element */                  \
}

#define INET_QUEUE_ENTRY(type)                                                   \
struct {                                                                         \
        struct type *qe_next;	/* next element */                               \
        struct type **qe_prev;	/* address of previous next element */           \
}

#define	INET_QUEUE_FIRST(head) ((head)->qh_first)
#define	INET_QUEUE_END(head) NULL
#define	INET_QUEUE_NEXT(elm, field) ((elm)->field.qe_next)
#define INET_QUEUE_LAST(head, headname)                                          \
	(*(((struct headname *)((head)->qh_last))->qh_last))
#define INET_QUEUE_PREV(elm, headname, field)                                    \
        (*(((struct headname *)((elm)->field.qe_prev))->qh_last))
#define	INET_QUEUE_EMPTY(head)                                                   \
        (INET_QUEUE_FIRST(head) == INET_QUEUE_END(head))

#define INET_QUEUE_FOREACH(var, head, field)                                     \
        for((var) = INET_QUEUE_FIRST(head);                                      \
            (var) != INET_QUEUE_END(head);                                       \
            (var) = INET_QUEUE_NEXT(var, field))

#define	INET_QUEUE_INIT(head) do {                                               \
        (head)->qh_first = NULL;                                                 \
        (head)->qh_last = &(head)->qh_first;                                     \
} while (0)

#define INET_QUEUE_INSERT_HEAD(head, elm, field) do {                            \
        if (((elm)->field.qe_next = (head)->qh_first) != NULL)                   \
                (head)->qh_first->field.tqe_prev = &(elm)->field.qe_next;        \
        else                                                                     \
		(head)->qh_last = &(elm)->field.qe_next;                         \
        (head)->qh_first = (elm);                                                \
        (elm)->field.qe_prev = &(head)->qh_first;                                \
} while (0)

#define INET_QUEUE_INSERT_TAIL(head, elm, field) do {                            \
        (elm)->field.qe_next = NULL;                                             \
        (elm)->field.qe_prev = (head)->qh_last;                                  \
        *(head)->qh_last = (elm);                                                \
        (head)->qh_last = &(elm)->field.qe_next;                                 \
} while (0)

#define INET_QUEUE_INSERT_AFTER(head, listelm, elm, field) do {                  \
        if (((elm)->field.qe_next = (listelm)->field.qe_next) != NULL)           \
                (elm)->field.qe_next->field.qe_prev =	&(elm)->field.qe_next;   \
        else                                                                     \
                (head)->qh_last = &(elm)->field.qe_next;                         \
        (listelm)->field.qe_next = (elm);                                        \
        (elm)->field.qe_prev = &(listelm)->field.qe_next;                        \
} while (0)

#define	INET_QUEUE_INSERT_BEFORE(listelm, elm, field) do {                       \
        (elm)->field.qe_prev = (listelm)->field.qe_prev;                         \
        (elm)->field.qe_next = (listelm);                                        \
        *(listelm)->field.qe_prev = (elm);                                       \
        (listelm)->field.qe_prev = &(elm)->field.qe_next;                        \
} while (0)

#define INET_QUEUE_REMOVE(head, elm, field) do {                                 \
        if (((elm)->field.qe_next) != NULL)                                      \
                (elm)->field.qe_next->field.qe_prev =	(elm)->field.qe_prev;    \
        else                                                                     \
                (head)->qh_last = (elm)->field.qe_prev;                          \
        *(elm)->field.qe_prev = (elm)->field.qe_next;                            \
} while (0)

#define INET_QUEUE_REPLACE(head, elm, elm2, field) do {                          \
        if (((elm2)->field.qe_next = (elm)->field.qe_next) != NULL)              \
                (elm2)->field.qe_next->field.qe_prev = &(elm2)->field.qe_next;   \
        else                                                                     \
                (head)->qh_last = &(elm2)->field.qe_next;                        \
        (elm2)->field.qe_prev = (elm)->field.qe_prev;                            \
        *(elm2)->field.qe_prev = (elm2);                                         \
} while (0)

#endif // #ifndef __LIBINET_UTIL_H__

