#pragma once

#include "copyinterceptor.h"

class CopyInterceptor::Private
{
public:
	QJSValue copy;
	QJSValue paste;

	QJSValue generateJSValueFromClipboard();
};
