#pragma once
#include <assert.h>

#define KASSERT(VALUE) assert(!(VALUE))
#define KDELETE(VALUE) if (VALUE != nullptr) { delete VALUE; VALUE = nullptr;}
#define KRELESE(VALUE) if (VALUE != nullptr) { VALUE->Release();}
#define KDESTROY(VALUE) if (VALUE != nullptr) { VALUE->Destroy();}
#define BBY KASSERT(true);
#define KUINT unsigned int
#define PATHNUM 128
#define NAMENUM 32