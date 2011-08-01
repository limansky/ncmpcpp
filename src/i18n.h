#ifndef _I18N_H
#define _I18N_H


#include "config.h"

#ifdef ENABLE_NLS
#include <libintl.h>

#define _(x) (const char*)gettext(x)

#else
#define _(x) x
#define ngettext(s, m, d) (d == 1 ? s : m)
#endif

#endif // _I18N_H
