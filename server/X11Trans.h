// Copyright (C)2004 Landmark Graphics Corporation
// Copyright (C)2005, 2006 Sun Microsystems, Inc.
// Copyright (C)2010-2011, 2014 D. R. Commander
//
// This library is free software and may be redistributed and/or modified under
// the terms of the wxWindows Library License, Version 3.1 or (at your option)
// any later version.  The full license is in the LICENSE.txt file included
// with this distribution.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// wxWindows Library License for more details.

#ifndef __X11TRANS_H__
#define __X11TRANS_H__

#include "Thread.h"
#include "Frame.h"
#include "GenericQ.h"
#include "Profiler.h"


namespace vglserver
{
	class X11Trans : public vglutil::Runnable
	{
		public:

			X11Trans(void);

			virtual ~X11Trans(void)
			{
				deadYet = true;
				q.release();
				if(thread) { thread->stop();  delete thread;  thread = NULL; }
				for(int i = 0; i < NFRAMES; i++)
				{
					if(frames[i]) delete frames[i];
					frames[i] = NULL;
				}
			}

			bool isReady(void);
			void synchronize(void);
			void sendFrame(vglcommon::FBXFrame *, bool sync = false);
			void run(void);
			vglcommon::FBXFrame *getFrame(Display *dpy, Window win, int width,
				int height);

		private:

			static const int NFRAMES = 3;
			vglutil::CriticalSection mutex;
			vglcommon::FBXFrame *frames[NFRAMES];
			vglutil::Event ready;
			vglutil::GenericQ q;
			vglutil::Thread *thread;
			bool deadYet;
			vglcommon::Profiler profBlit, profTotal;
	};
}

#endif  // __X11TRANS_H__
