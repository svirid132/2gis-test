#include "RegExpIns.h"

QString RegExpIns::m_filter = "^([a-zA-Z-_0-9]+|[а-яА-Я-_0-9]+)$";

QString RegExpIns::filter() const
{
    return m_filter;
}
