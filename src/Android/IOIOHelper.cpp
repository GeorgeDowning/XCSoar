/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2012 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Android/IOIOHelper.hpp"
#include "PortBridge.hpp"
#include "Java/Class.hpp"

jclass IOIOHelper::cls;
jmethodID IOIOHelper::ctor, IOIOHelper::open_method,
  IOIOHelper::openUart_method;

bool
IOIOHelper::Initialise(JNIEnv *env)
{
  assert(cls == NULL);
  assert(env != NULL);

  jclass _cls = env->FindClass("org/xcsoar/IOIOHelper");
  if (_cls == NULL) {
    env->ExceptionClear();
    return false;
  }

  cls = (jclass)env->NewGlobalRef(_cls);
  env->DeleteLocalRef(_cls);

  ctor = env->GetMethodID(cls, "<init>", "()V");
  open_method = env->GetMethodID(cls, "open", "()Z");
  openUart_method = env->GetMethodID(cls, "openUart",
                                     "(II)Lorg/xcsoar/AndroidPort;");
  return true;
}

void
IOIOHelper::Deinitialise(JNIEnv *env)
{
  if (cls != NULL)
    env->DeleteGlobalRef(cls);
}

PortBridge *
IOIOHelper::openUart(JNIEnv *env, unsigned ID, unsigned baud)
{
  jobject obj = env->CallObjectMethod(Get(), openUart_method, ID, (int)baud);
  if (obj == NULL)
    return NULL;

  PortBridge *bridge = new PortBridge(env, obj);
  env->DeleteLocalRef(obj);
  return bridge;
}

IOIOHelper::IOIOHelper(JNIEnv *env)
{
  jobject obj = env->NewObject(cls, ctor);
  assert(obj != NULL);

  Set(env, obj);

  env->DeleteLocalRef(obj);
}
