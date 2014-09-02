#ifndef _TCUAPP_HPP
#define _TCUAPP_HPP
/*-------------------------------------------------------------------------
 * drawElements Quality Program Tester Core
 * ----------------------------------------
 *
 * Copyright 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *//*!
 * \file
 * \brief Tester application.
 *
 * Platform port (see tcuPlatform.hpp) must create App and issue calls to
 * App::iterate() until it signals that test execution is completed.
 *//*--------------------------------------------------------------------*/

#include "tcuDefs.hpp"
#include "qpWatchDog.h"
#include "qpCrashHandler.h"
#include "deMutex.hpp"

namespace tcu
{

class Archive;
class Platform;
class TestContext;
class TestExecutor;
class CommandLine;
class TestLog;

// Defined in tcuTestExecutor.hpp
class TestRunResult;

/*--------------------------------------------------------------------*//*!
 * \brief Test application
 *
 * Test application encapsulates full test execution logic. Platform port
 * must create App object and repeately call iterate() until it returns
 * false.
 *
 * On systems where main loop is not in control of application (such as
 * Android or iOS) iterate() should be called in application update/draw
 * callback.
 *
 * App is responsible of setting up crash handler (qpCrashHandler) and
 * watchdog (qpWatchDog).
 *
 * See tcuMain.cpp for example on how to implement application stub.
 *//*--------------------------------------------------------------------*/
class App
{
public:
							App					(Platform& platform, Archive& archive, TestLog& log, const CommandLine& cmdLine);
	virtual					~App				(void);

	bool					iterate				(void);

	const TestRunResult&	getResult			(void) const;

	void					onWatchdogTimeout	(void);
	void					onCrash				(void);

protected:
	Platform&				m_platform;
	qpWatchDog*				m_watchDog;
	qpCrashHandler*			m_crashHandler;
	de::Mutex				m_crashLock;
	bool					m_crashed;

	TestContext*			m_testCtx;
	TestExecutor*			m_testExecutor;
};

} // tcu

#endif // _TCUAPP_HPP
