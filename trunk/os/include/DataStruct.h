/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_DATASTRUCT_H
#define IGAME_OS_DATASTRUCT_H

#include <cassert>

#ifdef __cplusplus
extern "C" {
#endif

#define IGAME_FREELIST_HEAD(name, size)                                                    \
struct name {                                                                              \
        unsigned m_Size;                                                                   \
        unsigned m_Count;                                                                  \
        void*    m_Nodes[size];                                                            \
}

#define IGAME_FREELIST_INIT(head, size)                                                    \
do {                                                                                       \
        (head)->m_Size = size; (head)->m_Count = 0;                                        \
} while (0)
        
#define IGAME_FREELIST_PUSH(head, node)                                                    \
        ((head)->m_Count == (head)->m_Size ? 0 :                                           \
        ((head)->m_Nodes[(head)->m_Count++] = node))

#define IGAME_FREELIST_POP(head)                                                           \
        ((head)->m_Count == 0 ? NULL : (head)->m_Nodes[--(head)->m_Count])

#define IGAME_FREELIST_EMPTY(head) ((head)->m_Count == 0)

/**
 * Single List Define
 */

#define IGAME_LIST_HEAD(name, type)                                                        \
struct name {                                                                              \
    struct type *m_First;    /* first element */                                           \
    void* m_UsrPtr;          /* reversed for user */                                       \
}
 
#define IGAME_LIST_ENTRY(type)                                                             \
struct {                                                                                   \
    struct type *m_Next;     /* next element */                                            \
}

#define	IGAME_LIST_FIRST(head) ((head)->m_First)
#define	IGAME_LIST_END(head) NULL
#define	IGAME_LIST_EMPTY(head) (IGAME_LIST_FIRST(head) == IGAME_LIST_END(head))
#define	IGAME_LIST_NEXT(elm, field) ((elm)->field.m_Next)

#define	IGAME_LIST_FOREACH(var, head, field)                                               \
    for((var) = IGAME_LIST_FIRST(head);  (var) != IGAME_LIST_END(head);                    \
    (var) = IGAME_LIST_NEXT(var, field))

#define	IGAME_LIST_INIT(head) {                                                            \
    IGAME_LIST_FIRST(head) = IGAME_LIST_END(head);                                         \
    (head)->m_UsrPtr = NULL;                                                               \
}

#define	IGAME_LIST_INSERT_AFTER(slistelm, elm, field) do {                                 \
    (elm)->field.m_Next = (slistelm)->field.m_Next;                                        \
     (slistelm)->field.m_Next = (elm);                                                     \
} while (0)

#define	IGAME_LIST_INSERT_HEAD(head, elm, field) do {                                      \
    (elm)->field.m_Next = (head)->m_First;                                                 \
    (head)->m_First = (elm);                                                               \
} while (0)

#define	IGAME_LIST_REMOVE_HEAD(head, field) do {                                           \
    (head)->m_First = (head)->m_First->field.m_Next;                                       \
} while (0)

/**
 * Double List Define
 */

#define IGAME_DLIST_HEAD(name, type)                                                       \
struct name {                                                                              \
    struct type *m_First;    /* first element */                                           \
    struct type **m_Last;    /* address of last next element */                            \
}

#define IGAME_DLIST_ENTRY(type)                                                            \
struct {                                                                                   \
   struct type *m_Next;     /* next element */                                             \
   struct type **m_Prev;    /* address of previous next element */                         \
}

#define	IGAME_DLIST_FIRST(head) ((head)->m_First)
#define	IGAME_DLIST_END(head) NULL
#define	IGAME_DLIST_NEXT(elm, field) ((elm)->field.m_Next)
#define IGAME_DLIST_LAST(head, headname)                                                   \
    (*(((struct headname *)((head)->m_Last))->m_Last))
#define IGAME_DLIST_PREV(elm, headname, field)                                             \
    (*(((struct headname *)((elm)->field.m_Prev))->m_Last))
#define	IGAME_DLIST_EMPTY(head)                                                            \
    (IGAME_DLIST_FIRST(head) == IGAME_DLIST_END(head))

#define IGAME_DLIST_FOREACH(var, head, field)                                              \
    for((var) = IGAME_DLIST_FIRST(head); (var) != IGAME_DLIST_END(head);                   \
    (var) = IGAME_DLIST_NEXT(var, field))

#define	IGAME_DLIST_INIT(head) do {                                                        \
    (head)->m_First = NULL;                                                                \
    (head)->m_Last = &(head)->m_First;                                                     \
} while (0)

#define IGAME_DLIST_INSERT_HEAD(head, elm, field) do {                                     \
   if (((elm)->field.m_Next = (head)->m_First) != NULL)                                    \
        (head)->m_First->field.m_Prev = &(elm)->field.m_Next;                              \
    else                                                                                   \
        (head)->m_Last = &(elm)->field.m_Next;                                             \
    (head)->m_First = (elm);                                                               \
    (elm)->field.m_Prev = &(head)->m_First;                                                \
} while (0)

#define IGAME_DLIST_INSERT_TAIL(head, elm, field) do {                                     \
    (elm)->field.m_Next = NULL;                                                            \
    (elm)->field.m_Prev = (head)->m_Last;                                                  \
    *(head)->m_Last = (elm);                                                               \
    (head)->m_Last = &(elm)->field.m_Next;                                                 \
} while (0)

#define IGAME_DLIST_INSERT_AFTER(head, listelm, elm, field) do {                           \
    if (((elm)->field.m_Next = (listelm)->field.m_Next) != NULL)                           \
        (elm)->field.m_Next->field.m_Prev = &(elm)->field.m_Next;                          \
    else                                                                                   \
        (head)->m_Last = &(elm)->field.m_Next;                                             \
    (listelm)->field.m_Next = (elm);                                                       \
    (elm)->field.m_Prev = &(listelm)->field.m_Next;                                        \
} while (0)

#define	IGAME_DLIST_INSERT_BEFORE(listelm, elm, field) do {                                \
    (elm)->field.m_Prev = (listelm)->field.m_Prev;                                         \
    (elm)->field.m_Next = (listelm);                                                       \
    *(listelm)->field.m_Prev = (elm);                                                      \
    (listelm)->field.m_Prev = &(elm)->field.m_Next;                                        \
} while (0)

#define IGAME_DLIST_REMOVE(head, elm, field) do {                                          \
    if (((elm)->field.m_Next) != NULL)                                                     \
        (elm)->field.m_Next->field.m_Prev = (elm)->field.m_Prev;                           \
    else                                                                                   \
        (head)->m_Last = (elm)->field.m_Prev;                                              \
    *(elm)->field.m_Prev = (elm)->field.m_Next;                                            \
} while (0)


/**
 * RB Tree Define
 */

#define IGAME_RB_HEAD(name, type)                                                          \
struct name {                                                                              \
struct type *rbh_root; /* root of the tree */                                              \
}

#define IGAME_RB_INITIALIZER(root)                                                         \
    { NULL }

#define IGAME_RB_INIT(root) do {                                                           \
    (root)->rbh_root = NULL;                                                               \
} while (0)

#define IGAME_RB_BLACK	0
#define IGAME_RB_RED		1
#define IGAME_RB_ENTRY(type)                                                               \
struct {                                                                                   \
struct type *rbe_left;          /* left element */                                         \
struct type *rbe_right;	        /* right element */                                        \
struct type *rbe_parent;        /* parent element */                                       \
int rbe_color;                  /* node color */                                           \
}

#define IGAME_RB_LEFT(elm, field)             (elm)->field.rbe_left
#define IGAME_RB_RIGHT(elm, field)            (elm)->field.rbe_right
#define IGAME_RB_PARENT(elm, field)           (elm)->field.rbe_parent
#define IGAME_RB_COLOR(elm, field)            (elm)->field.rbe_color
#define IGAME_RB_ROOT(head)                   (head)->rbh_root
#define IGAME_RB_EMPTY(head)                  (IGAME_RB_ROOT(head) == NULL)

#define IGAME_RB_SET(elm, parent, field) do {                                              \
    IGAME_RB_PARENT(elm, field) = parent;                                                  \
    IGAME_RB_LEFT(elm, field) = IGAME_RB_RIGHT(elm, field) = NULL;                         \
    IGAME_RB_COLOR(elm, field) = IGAME_RB_RED;                                             \
} while (0)

#define IGAME_RB_SET_BLACKRED(black, red, field) do {                                      \
    IGAME_RB_COLOR(black, field) = IGAME_RB_BLACK;                                         \
    IGAME_RB_COLOR(red, field) = IGAME_RB_RED;                                             \
} while (0)

#ifndef IGAME_RB_AUGMENT
#define IGAME_RB_AUGMENT(x) {} // Hack by hailong
#endif

#define IGAME_RB_ROTATE_LEFT(head, elm, tmp, field) do {                                   \
    (tmp) = IGAME_RB_RIGHT(elm, field);                                                    \
    if ((IGAME_RB_RIGHT(elm, field) = IGAME_RB_LEFT(tmp, field))) {                        \
    IGAME_RB_PARENT(IGAME_RB_LEFT(tmp, field), field) = (elm);                             \
    }                                                                                      \
    IGAME_RB_AUGMENT(elm);                                                                 \
    if ((IGAME_RB_PARENT(tmp, field) = IGAME_RB_PARENT(elm, field))) {                     \
    if ((elm) == IGAME_RB_LEFT(IGAME_RB_PARENT(elm, field), field))                        \
    IGAME_RB_LEFT(IGAME_RB_PARENT(elm, field), field) = (tmp);                             \
        else                                                                               \
        IGAME_RB_RIGHT(IGAME_RB_PARENT(elm, field), field) = (tmp);                        \
    } else                                                                                 \
    (head)->rbh_root = (tmp);                                                              \
    IGAME_RB_LEFT(tmp, field) = (elm);                                                     \
    IGAME_RB_PARENT(elm, field) = (tmp);                                                   \
    IGAME_RB_AUGMENT(tmp);                                                                 \
    if ((IGAME_RB_PARENT(tmp, field)))                                                     \
    IGAME_RB_AUGMENT(IGAME_RB_PARENT(tmp, field));                                         \
} while (0)

#define IGAME_RB_ROTATE_RIGHT(head, elm, tmp, field) do {                                  \
    (tmp) = IGAME_RB_LEFT(elm, field);                                                     \
    if ((IGAME_RB_LEFT(elm, field) = IGAME_RB_RIGHT(tmp, field))) {                        \
    IGAME_RB_PARENT(IGAME_RB_RIGHT(tmp, field), field) = (elm);                            \
    }                                                                                      \
    IGAME_RB_AUGMENT(elm);                                                                 \
    if ((IGAME_RB_PARENT(tmp, field) = IGAME_RB_PARENT(elm, field))) {                     \
    if ((elm) == IGAME_RB_LEFT(IGAME_RB_PARENT(elm, field), field))                        \
    IGAME_RB_LEFT(IGAME_RB_PARENT(elm, field), field) = (tmp);                             \
        else                                                                               \
        IGAME_RB_RIGHT(IGAME_RB_PARENT(elm, field), field) = (tmp);                        \
    } else                                                                                 \
    (head)->rbh_root = (tmp);                                                              \
    IGAME_RB_RIGHT(tmp, field) = (elm);                                                    \
    IGAME_RB_PARENT(elm, field) = (tmp);                                                   \
    IGAME_RB_AUGMENT(tmp);                                                                 \
    if ((IGAME_RB_PARENT(tmp, field)))                                                     \
    IGAME_RB_AUGMENT(IGAME_RB_PARENT(tmp, field));                                         \
} while (0)

/* Generates prototypes and inline functions */
#define IGAME_RB_PROTOTYPE(name, type, field, cmp)                                         \
void name##_IGAME_RB_INSERT_COLOR(struct name *, struct type *);                           \
void name##_IGAME_RB_REMOVE_COLOR(struct name *, struct type *, struct type *);            \
struct type *name##_IGAME_RB_REMOVE(struct name *, struct type *);                         \
struct type *name##_IGAME_RB_INSERT(struct name *, struct type *);                         \
struct type *name##_IGAME_RB_FIND(struct name *, struct type *);                           \
struct type *name##_IGAME_RB_NEXT(struct type *);                                          \
struct type *name##_IGAME_RB_MINMAX(struct name *, int);                                   \
                                                                                           \

/* Main rb operation.
* Moves node close to the key of elm to top
*/
#define IGAME_RB_GENERATE(name, type, field, cmp)                                          \
void                                                                                       \
name##_IGAME_RB_INSERT_COLOR(struct name *head, struct type *elm)                          \
{                                                                                          \
    struct type *parent, *gparent, *tmp;                                                   \
    while ((parent = IGAME_RB_PARENT(elm, field)) &&                                       \
        IGAME_RB_COLOR(parent, field) == IGAME_RB_RED) {                                   \
        gparent = IGAME_RB_PARENT(parent, field);                                          \
        if (parent == IGAME_RB_LEFT(gparent, field)) {                                     \
            tmp = IGAME_RB_RIGHT(gparent, field);                                          \
            if (tmp && IGAME_RB_COLOR(tmp, field) == IGAME_RB_RED) {                       \
                IGAME_RB_COLOR(tmp, field) = IGAME_RB_BLACK;                               \
                IGAME_RB_SET_BLACKRED(parent, gparent, field);                             \
                elm = gparent;                                                             \
                continue;                                                                  \
            }                                                                              \
            if (IGAME_RB_RIGHT(parent, field) == elm) {                                    \
                IGAME_RB_ROTATE_LEFT(head, parent, tmp, field);                            \
                tmp = parent;                                                              \
                parent = elm;                                                              \
                elm = tmp;                                                                 \
            }                                                                              \
            IGAME_RB_SET_BLACKRED(parent, gparent, field);	                           \
            IGAME_RB_ROTATE_RIGHT(head, gparent, tmp, field);	                           \
       } else {                                                                            \
           tmp = IGAME_RB_LEFT(gparent, field);                                            \
           if (tmp && IGAME_RB_COLOR(tmp, field) == IGAME_RB_RED) {	                   \
               IGAME_RB_COLOR(tmp, field) = IGAME_RB_BLACK;	                           \
               IGAME_RB_SET_BLACKRED(parent, gparent, field);                              \
               elm = gparent;                                                              \
               continue;                                                                   \
           }                                                                               \
           if (IGAME_RB_LEFT(parent, field) == elm) {                                      \
               IGAME_RB_ROTATE_RIGHT(head, parent, tmp, field);                            \
               tmp = parent;                                                               \
               parent = elm;                                                               \
               elm = tmp;                                                                  \
           }                                                                               \
           IGAME_RB_SET_BLACKRED(parent, gparent, field);                                  \
           IGAME_RB_ROTATE_LEFT(head, gparent, tmp, field);                                \
       }                                                                                   \
    }                                                                                      \
    IGAME_RB_COLOR(head->rbh_root, field) = IGAME_RB_BLACK;                                \
}                                                                                          \
                                                                                           \
void                                                                                       \
name##_IGAME_RB_REMOVE_COLOR(struct name *head, struct type *parent, struct type *elm)     \
{                                                                                          \
    struct type *tmp;                                                                      \
    while ((elm == NULL || IGAME_RB_COLOR(elm, field) == IGAME_RB_BLACK) &&	           \
        elm != IGAME_RB_ROOT(head)) {                                                      \
        if (IGAME_RB_LEFT(parent, field) == elm) {                                         \
            tmp = IGAME_RB_RIGHT(parent, field);                                           \
            if (IGAME_RB_COLOR(tmp, field) == IGAME_RB_RED) {                              \
                IGAME_RB_SET_BLACKRED(tmp, parent, field);                                 \
                IGAME_RB_ROTATE_LEFT(head, parent, tmp, field);                            \
                tmp = IGAME_RB_RIGHT(parent, field);                                       \
            }                                                                              \
            if ((IGAME_RB_LEFT(tmp, field) == NULL ||                                      \
                IGAME_RB_COLOR(IGAME_RB_LEFT(tmp, field), field) == IGAME_RB_BLACK) &&     \
                (IGAME_RB_RIGHT(tmp, field) == NULL ||                                     \
                IGAME_RB_COLOR(IGAME_RB_RIGHT(tmp, field), field) == IGAME_RB_BLACK)) {    \
                IGAME_RB_COLOR(tmp, field) = IGAME_RB_RED;                                 \
                elm = parent;                                                              \
                parent = IGAME_RB_PARENT(elm, field);                                      \
            } else {                                                                       \
                if (IGAME_RB_RIGHT(tmp, field) == NULL ||                                  \
                    IGAME_RB_COLOR(IGAME_RB_RIGHT(tmp, field), field) == IGAME_RB_BLACK) { \
                    struct type *oleft;                                                    \
                    if ((oleft = IGAME_RB_LEFT(tmp, field)))                               \
                        IGAME_RB_COLOR(oleft, field) = IGAME_RB_BLACK;                     \
                    IGAME_RB_COLOR(tmp, field) = IGAME_RB_RED;                             \
                    IGAME_RB_ROTATE_RIGHT(head, tmp, oleft, field);                        \
                    tmp = IGAME_RB_RIGHT(parent, field);                                   \
                }                                                                          \
                IGAME_RB_COLOR(tmp, field) = IGAME_RB_COLOR(parent, field);                \
                IGAME_RB_COLOR(parent, field) = IGAME_RB_BLACK;                            \
                if (IGAME_RB_RIGHT(tmp, field))                                            \
                    IGAME_RB_COLOR(IGAME_RB_RIGHT(tmp, field), field) = IGAME_RB_BLACK;    \
                IGAME_RB_ROTATE_LEFT(head, parent, tmp, field);                            \
                elm = IGAME_RB_ROOT(head);                                                 \
                break;                                                                     \
            }                                                                              \
        } else {                                                                           \
            tmp = IGAME_RB_LEFT(parent, field);                                            \
            if (IGAME_RB_COLOR(tmp, field) == IGAME_RB_RED) {                              \
                IGAME_RB_SET_BLACKRED(tmp, parent, field);                                 \
                IGAME_RB_ROTATE_RIGHT(head, parent, tmp, field);                           \
                tmp = IGAME_RB_LEFT(parent, field);                                        \
            }                                                                              \
            if ((IGAME_RB_LEFT(tmp, field) == NULL ||                                      \
                IGAME_RB_COLOR(IGAME_RB_LEFT(tmp, field), field) == IGAME_RB_BLACK) &&     \
                (IGAME_RB_RIGHT(tmp, field) == NULL ||                                     \
                IGAME_RB_COLOR(IGAME_RB_RIGHT(tmp, field), field) == IGAME_RB_BLACK)) {    \
                IGAME_RB_COLOR(tmp, field) = IGAME_RB_RED;                                 \
                elm = parent;                                                              \
                parent = IGAME_RB_PARENT(elm, field);                                      \
            } else {                                                                       \
                if (IGAME_RB_LEFT(tmp, field) == NULL ||                                   \
                    IGAME_RB_COLOR(IGAME_RB_LEFT(tmp, field), field) == IGAME_RB_BLACK) {  \
                    struct type *oright;                                                   \
                    if ((oright = IGAME_RB_RIGHT(tmp, field)))                             \
                        IGAME_RB_COLOR(oright, field) = IGAME_RB_BLACK;                    \
                    IGAME_RB_COLOR(tmp, field) = IGAME_RB_RED;                             \
                    IGAME_RB_ROTATE_LEFT(head, tmp, oright, field);                        \
                    tmp = IGAME_RB_LEFT(parent, field);                                    \
                }                                                                          \
                IGAME_RB_COLOR(tmp, field) = IGAME_RB_COLOR(parent, field);                \
                IGAME_RB_COLOR(parent, field) = IGAME_RB_BLACK;                            \
                if (IGAME_RB_LEFT(tmp, field))                                             \
                    IGAME_RB_COLOR(IGAME_RB_LEFT(tmp, field), field) = IGAME_RB_BLACK;     \
                IGAME_RB_ROTATE_RIGHT(head, parent, tmp, field);                           \
                elm = IGAME_RB_ROOT(head);                                                 \
                break;                                                                     \
            }                                                                              \
        }                                                                                  \
    }                                                                                      \
    if (elm)                                                                               \
        IGAME_RB_COLOR(elm, field) = IGAME_RB_BLACK;                                       \
}                                                                                          \
                                                                                           \
struct type *                                                                              \
name##_IGAME_RB_REMOVE(struct name *head, struct type *elm)                                \
{                                                                                          \
    struct type *child, *parent, *old = elm;                                               \
    int color;                                                                             \
    if (IGAME_RB_LEFT(elm, field) == NULL)                                                 \
        child = IGAME_RB_RIGHT(elm, field);                                                \
    else if (IGAME_RB_RIGHT(elm, field) == NULL)                                           \
        child = IGAME_RB_LEFT(elm, field);                                                 \
    else {                                                                                 \
        struct type *left;                                                                 \
        elm = IGAME_RB_RIGHT(elm, field);                                                  \
        while ((left = IGAME_RB_LEFT(elm, field)))                                         \
            elm = left;                                                                    \
        child = IGAME_RB_RIGHT(elm, field);                                                \
        parent = IGAME_RB_PARENT(elm, field);                                              \
        color = IGAME_RB_COLOR(elm, field);                                                \
        if (child)                                                                         \
            IGAME_RB_PARENT(child, field) = parent;                                        \
        if (parent) {                                                                      \
            if (IGAME_RB_LEFT(parent, field) == elm)                                       \
                IGAME_RB_LEFT(parent, field) = child;                                      \
            else                                                                           \
                IGAME_RB_RIGHT(parent, field) = child;                                     \
            IGAME_RB_AUGMENT(parent);                                                      \
        } else                                                                             \
            IGAME_RB_ROOT(head) = child;                                                   \
        if (IGAME_RB_PARENT(elm, field) == old)                                            \
            parent = elm;                                                                  \
        (elm)->field = (old)->field;                                                       \
        if (IGAME_RB_PARENT(old, field)) {                                                 \
            if (IGAME_RB_LEFT(IGAME_RB_PARENT(old, field), field) == old)                  \
                IGAME_RB_LEFT(IGAME_RB_PARENT(old, field), field) = elm;                   \
            else                                                                           \
                IGAME_RB_RIGHT(IGAME_RB_PARENT(old, field), field) = elm;                  \
            IGAME_RB_AUGMENT(IGAME_RB_PARENT(old, field));                                 \
        } else                                                                             \
            IGAME_RB_ROOT(head) = elm;                                                     \
        IGAME_RB_PARENT(IGAME_RB_LEFT(old, field), field) = elm;                           \
        if (IGAME_RB_RIGHT(old, field))                                                    \
            IGAME_RB_PARENT(IGAME_RB_RIGHT(old, field), field) = elm;                      \
        if (parent) {                                                                      \
            left = parent;                                                                 \
            do {                                                                           \
                IGAME_RB_AUGMENT(left);                                                    \
            } while ((left = IGAME_RB_PARENT(left, field)));                               \
        }                                                                                  \
        goto color;                                                                        \
    }                                                                                      \
    parent = IGAME_RB_PARENT(elm, field);                                                  \
    color = IGAME_RB_COLOR(elm, field);                                                    \
    if (child)                                                                             \
        IGAME_RB_PARENT(child, field) = parent;                                            \
    if (parent) {                                                                          \
        if (IGAME_RB_LEFT(parent, field) == elm)                                           \
            IGAME_RB_LEFT(parent, field) = child;                                          \
        else                                                                               \
            IGAME_RB_RIGHT(parent, field) = child;                                         \
        IGAME_RB_AUGMENT(parent);                                                          \
    } else                                                                                 \
        IGAME_RB_ROOT(head) = child;                                                       \
color:                                                                                     \
    if (color == IGAME_RB_BLACK)                                                           \
    name##_IGAME_RB_REMOVE_COLOR(head, parent, child);                                     \
    return (old);                                                                          \
}                                                                                          \
                                                                                           \
/* Inserts a node into the RB tree */                                                      \
struct type *                                                                              \
name##_IGAME_RB_INSERT(struct name *head, struct type *elm)                                \
{                                                                                          \
    struct type *tmp;                                                                      \
    struct type *parent = NULL;                                                            \
    int comp = 0;                                                                          \
    tmp = IGAME_RB_ROOT(head);                                                             \
    while (tmp) {                                                                          \
        parent = tmp;                                                                      \
        comp = (cmp)(elm, parent);                                                         \
        if (comp < 0)                                                                      \
            tmp = IGAME_RB_LEFT(tmp, field);                                               \
        else if (comp > 0)                                                                 \
            tmp = IGAME_RB_RIGHT(tmp, field);                                              \
        else                                                                               \
            return (tmp);                                                                  \
    }                                                                                      \
    IGAME_RB_SET(elm, parent, field);                                                      \
    if (parent != NULL) {                                                                  \
        if (comp < 0)                                                                      \
            IGAME_RB_LEFT(parent, field) = elm;                                            \
        else                                                                               \
            IGAME_RB_RIGHT(parent, field) = elm;                                           \
        IGAME_RB_AUGMENT(parent);                                                          \
    } else                                                                                 \
        IGAME_RB_ROOT(head) = elm;                                                         \
    name##_IGAME_RB_INSERT_COLOR(head, elm);                                               \
    return (NULL);                                                                         \
}                                                                                          \
                                                                                           \
/* Finds the node with the same key as elm */                                              \
struct type *                                                                              \
name##_IGAME_RB_FIND(struct name *head, struct type *elm)                                  \
{                                                                                          \
    struct type *tmp = IGAME_RB_ROOT(head);                                                \
    int comp;                                                                              \
    while (tmp) {                                                                          \
        comp = cmp(elm, tmp);                                                              \
        if (comp < 0)                                                                      \
            tmp = IGAME_RB_LEFT(tmp, field);                                               \
        else if (comp > 0)                                                                 \
            tmp = IGAME_RB_RIGHT(tmp, field);                                              \
        else                                                                               \
            return (tmp);                                                                  \
    }                                                                                      \
    return (NULL);                                                                         \
}                                                                                          \
                                                                                           \
struct type *                                                                              \
name##_IGAME_RB_NEXT(struct type *elm) const                                               \
{                                                                                          \
    if (IGAME_RB_RIGHT(elm, field)) {                                                      \
        elm = IGAME_RB_RIGHT(elm, field);                                                  \
        while (IGAME_RB_LEFT(elm, field))                                                  \
            elm = IGAME_RB_LEFT(elm, field);                                               \
    } else {                                                                               \
        if (IGAME_RB_PARENT(elm, field) &&                                                 \
            (elm == IGAME_RB_LEFT(IGAME_RB_PARENT(elm, field), field)))                    \
            elm = IGAME_RB_PARENT(elm, field);                                             \
        else {                                                                             \
            while (IGAME_RB_PARENT(elm, field) &&                                          \
                (elm == IGAME_RB_RIGHT(IGAME_RB_PARENT(elm, field), field)))               \
                elm = IGAME_RB_PARENT(elm, field);                                         \
            elm = IGAME_RB_PARENT(elm, field);                                             \
        }                                                                                  \
    }                                                                                      \
    return (elm);                                                                          \
}                                                                                          \
                                                                                           \
struct type *                                                                              \
name##_IGAME_RB_MINMAX(const struct name *head, int val) const                             \
{                                                                                          \
    struct type *tmp = IGAME_RB_ROOT(head);                                                \
    struct type *parent = NULL;                                                            \
    while (tmp) {                                                                          \
        parent = tmp;                                                                      \
        if (val < 0)                                                                       \
            tmp = IGAME_RB_LEFT(tmp, field);                                               \
        else                                                                               \
            tmp = IGAME_RB_RIGHT(tmp, field);                                              \
    }                                                                                      \
    return (parent);                                                                       \
}

#define IGAME_RB_NEGINF	-1
#define IGAME_RB_INF	1

#define IGAME_RB_INSERT(name, x, y)	name##_IGAME_RB_INSERT(x, y)
#define IGAME_RB_REMOVE(name, x, y)	name##_IGAME_RB_REMOVE(x, y)
#define IGAME_RB_FIND(name, x, y)	name##_IGAME_RB_FIND(x, y)
#define IGAME_RB_NEXT(name, x, y)	name##_IGAME_RB_NEXT(y)
#define IGAME_RB_MIN(name, x)		name##_IGAME_RB_MINMAX(x, IGAME_RB_NEGINF)
#define IGAME_RB_MAX(name, x)		name##_IGAME_RB_MINMAX(x, IGAME_RB_INF)

#define IGAME_RB_FOREACH(x, name, head)                                                    \
    for ((x) = IGAME_RB_MIN(name, head);                                                   \
    (x) != NULL;                                                                           \
    (x) = name##_IGAME_RB_NEXT(x))


#ifdef __cplusplus
}
#endif

#endif // #ifndef IGAME_OS_DATASTRUCT_H



