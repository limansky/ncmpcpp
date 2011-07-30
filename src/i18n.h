#ifndef _I18N_H
#define _I18N_H

#include <libintl.h>
#include <cstdlib>

#include "config.h"

#ifdef ENABLE_NLS
#define _(x) gettext(x)
#define _U(x) TO_WSTRING(gettext(x))
#else
#define _(x) x
#endif

#endif // _I18N_H
