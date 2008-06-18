/**
 *  Version:     @(#)libinet/util.hxx    0.0.3 08/06/2008
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

#include <cassert>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Single List Define
 */

#define INET_LIST_HEAD(name, type)                                             \
struct name {                                                                  \
    struct type *first_;    /* first element */                                \
    void* usrptr_;          /* reversed for user */                            \
}
 
#define INET_LIST_ENTRY(type)                                                  \
struct {                                                                       \
    struct type *next_;     /* next element */                                 \
}

#define	INET_LIST_FIRST(head) ((head)->first_)
#define	INET_LIST_END(head) NULL
#define	INET_LIST_EMPTY(head) (INET_LIST_FIRST(head) == INET_LIST_END(head))
#define	INET_LIST_NEXT(elm, field) ((elm)->field.next_)

#define	INET_LIST_FOREACH(var, head, field)                                    \
    for((var) = INET_LIST_FIRST(head);  (var) != INET_LIST_END(head);          \
    (var) = INET_LIST_NEXT(var, field))

#define	INET_LIST_INIT(head) {                                                 \
    INET_LIST_FIRST(head) = INET_LIST_END(head);                               \
    (head)->usrptr_ = NULL;                                                    \
}

#define	INET_LIST_INSERT_AFTER(slistelm, elm, field) do {                      \
    (elm)->field.next_ = (slistelm)->field.next_;                              \
     (slistelm)->field.next_ = (elm);                                          \
} while (0)

#define	INET_LIST_INSERT_HEAD(head, elm, field) do {                           \
    (elm)->field.next_ = (head)->first_;                                       \
    (head)->first_ = (elm);                                                    \
} while (0)

#define	INET_LIST_REMOVE_HEAD(head, field) do {                                \
    (head)->first_ = (head)->first_->field.next_;                              \
} while (0)

/**
 * Double List Define
 */

#define INET_DLIST_HEAD(name, type)                                            \
struct name {                                                                  \
    struct type *first_;    /* first element */                                \
    struct type **last_;    /* address of last next element */                 \
}

#define INET_DLIST_ENTRY(type)                                                 \
struct {                                                                       \
   struct type *next_;     /* next element */                                  \
   struct type **prev_;    /* address of previous next element */              \
}

#define	INET_DLIST_FIRST(head) ((head)->first_)
#define	INET_DLIST_END(head) NULL
#define	INET_DLIST_NEXT(elm, field) ((elm)->field.next_)
#define INET_DLIST_LAST(head, headname)                                        \
    (*(((struct headname *)((head)->last_))->last_))
#define INET_DLIST_PREV(elm, headname, field)                                  \
    (*(((struct headname *)((elm)->field.prev_))->last_))
#define	INET_DLIST_EMPTY(head)                                                 \
    (INET_DLIST_FIRST(head) == INET_DLIST_END(head))

#define INET_DLIST_FOREACH(var, head, field)                                   \
    for((var) = INET_DLIST_FIRST(head); (var) != INET_DLIST_END(head);         \
    (var) = INET_DLIST_NEXT(var, field))

#define	INET_DLIST_INIT(head) do {                                             \
    (head)->first_ = NULL;                                                     \
    (head)->last_ = &(head)->first_;                                           \
} while (0)

#define INET_DLIST_INSERT_HEAD(head, elm, field) do {                          \
   if (((elm)->field.next_ = (head)->first_) != NULL)                          \
        (head)->first_->field.tprev_ = &(elm)->field.next_;                    \
    else                                                                       \
        (head)->last_ = &(elm)->field.next_;                                   \
    (head)->first_ = (elm);                                                    \
    (elm)->field.prev_ = &(head)->first_;                                      \
} while (0)

#define INET_DLIST_INSERT_TAIL(head, elm, field) do {                          \
    (elm)->field.next_ = NULL;                                                 \
    (elm)->field.prev_ = (head)->last_;                                        \
    *(head)->last_ = (elm);                                                    \
    (head)->last_ = &(elm)->field.next_;                                       \
} while (0)

#define INET_DLIST_INSERT_AFTER(head, listelm, elm, field) do {                \
    if (((elm)->field.next_ = (listelm)->field.next_) != NULL)                 \
        (elm)->field.next_->field.prev_ = &(elm)->field.next_;                 \
    else                                                                       \
        (head)->last_ = &(elm)->field.next_;                                   \
    (listelm)->field.next_ = (elm);                                            \
    (elm)->field.prev_ = &(listelm)->field.next_;                              \
} while (0)

#define	INET_DLIST_INSERT_BEFORE(listelm, elm, field) do {                     \
    (elm)->field.prev_ = (listelm)->field.prev_;                               \
    (elm)->field.next_ = (listelm);                                            \
    *(listelm)->field.prev_ = (elm);                                           \
    (listelm)->field.prev_ = &(elm)->field.next_;                              \
} while (0)

#define INET_DLIST_REMOVE(head, elm, field) do {                               \
    if (((elm)->field.next_) != NULL)                                          \
        (elm)->field.next_->field.prev_ = (elm)->field.prev_;                  \
    else                                                                       \
        (head)->last_ = (elm)->field.prev_;                                    \
    *(elm)->field.prev_ = (elm)->field.next_;                                  \
} while (0)

#ifdef __cplusplus
}
#endif

#endif // #ifndef __LIBINET_UTIL_H__

