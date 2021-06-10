// ACSHook.cpp : Implementation of CACSHook. Context specific functionality
//
// Change history:
//
// POS22495 Work Request:16656 Name:Robert Susanto Date:October 28, 2011
// POS15617 Work Request:16632 Name:Matt Condra Date:August 4, 2011
// POS15617 Work Request:15196 Name:Matt Condra Date:July 20, 2011
// POS14165 Work Request:13752 Name:Robert Susanto Date:May 23, 2011
// POS13246 Work Request:13752 Name:Robert Susanto Date:April 19, 2011
// POS13219 Work Request:13752 Name:Robert Susanto Date:April 19, 2011
// POS11767 Work Request:13752 Name:Matt Condra Date:March 11, 2011
//

#include "stdafx.h"
#include "ACSHook.h"
#include "ACSContext.h"
#include "DataKit.h"
#include "Utils.h"
#include "TraceImpl.h"
#include <iomanip>

bool static bInReadContextsCriticalSection     = false;  // POS15617: For debugging purposes
bool static bInDispatchContextsCriticalSection = false;  // POS15617: For debugging purposes

DWORD WINAPI ReadContextsProc(LPVOID pvParam)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	_ASSERT(SUCCEEDED(hr));

	DWORD retVal = -1;
	CACSHook* pHook = (CACSHook*)pvParam;

	if (pHook)
	{
		retVal = pHook->ReadContexts();
	}

	CoUninitialize();
	return retVal;
}


DWORD CACSHook::ReadContexts(void)
{
	Trace trace(L"CACSHook::ReadContexts");

	static long nContextsSinceCritSectFail = 0;  // POS15617: For debugging purposes

	//POS15617: Make sure the context semaphore is created prior to thread execution because the thread
	// uses the semaphore.
	m_hContextSemaphore = CreateSemaphore(NULL, 0, INT_MAX, NULL);
	m_hContextThread = NULL;
	if (m_hContextSemaphore)
	{
		trace(DCM_INFO, _T("Context semaphore created."));
		m_hContextThread = CreateThread(NULL, 0, DispatchContextsProc, (PVOID)this, 0, NULL);
	}
	else
	{
		trace(DCM_INFO, _T("Context semaphore failed to be created."));
	}

	if (m_hContextThread)
	{
		//POS15617: No need to create context semaphore here because it was created earlier.
		//m_hContextSemaphore = CreateSemaphore(NULL, 0, INT_MAX, NULL);

		/* this is a foreign thread, remember this when accessing class data */
		trace(DCM_INFO, _T("Entering Context Listener and dispatcher worker thread."));
		bool exitThread = false;

		HANDLE hEvents[] = { m_hDisconnectEvent };

		while (!exitThread)
		{
			trace(DCM_INFO, _T("Waiting for context to arrive..."));  // POS248467 rrs
			DWORD dwRet = ACSIOWaitForDataEventMsg(m_hClient, 1, hEvents, INFINITE, QS_ALLINPUT, 0);
			trace(DCM_INFO, _T("Done Waiting for context.")); // POS248467 rrs

			if( ACSIO_OBJECT_0 == dwRet )
			{
				/* Process context - return value can either be an error or ACSIO_NO_DATA */
				
				BSTR contextName = 0;
				// VARIANT contextData;
				//POS14165 - managing the context information from acsio using the variant pointer
				VARIANT* pContextData = new VARIANT;
				bool dataQueued = false;
				// e pos14165

				HRESULT hr = ACSIOGetContextFields(m_hClient, &contextName, pContextData);
				
				/* +SOTF8781 */
				if (FAILED(hr) && (hr != ACSIO_NO_DATA))
				{
					trace(DCM_ERROR) 
						<< _T("An error occured retrieving a context with ACSIOGetContext. Error [0x") << std::hex << hr << std::dec
						<< _T("]: ") << ACSIOClient::GetErrorText(m_hClient) << NCR::Util::endl;

#ifndef NDEBUG
					/* The duplication here is annoying, but apparently the assertion macro only takes ASCII strings. -- PMP */
					std::stringstream errorMsg;
					errorMsg << "An error occured retrieving a context with ACSIOGetContext. Error [0x" << std::hex << hr << std::dec
						<< "]: " << ACSIOClient::GetErrorText(m_hClient);
					
					_ASSERT_BASE(FALSE, errorMsg.str().c_str());
#endif
				}
				else if (hr != ACSIO_NO_DATA)
				{
					if (contextName && SysStringLen(contextName) > 0)
					{
						trace(DCM_INFO) << _T("ACS Context: '") << (LPCTSTR)contextName << _T("'") << NCR::Util::endl; // POS15617

						{	// Begin Critical Section so that the context queue is not compromised.
							CCriticalSectionLock lock(m_csContextQueue, TRUE); // POS15617: Add TRUE parameter to enable critical section.

							// POS15617: For debugging purposes, keep track of critical section stability.
							bInReadContextsCriticalSection = true;

							if (bInDispatchContextsCriticalSection)
							{
								// POS15617: Critical section has been compromised, so log failure.
								trace(DCM_INFO) << _T("FAILURE: In critical section") << NCR::Util::endl;
								if (!nContextsSinceCritSectFail) { nContextsSinceCritSectFail++; }
							}

							/* POS14165 - Transfer ownership of the contextName and pContextData pointers 
							to the queue. */
							m_contextQueue.push(std::make_pair(contextName, pContextData));

							/* Note that the pointers were queued so that we don't try to free 
							them later. */
							dataQueued = true;
							// e pos14165

							// POS15617: For debugging purposes, keep track of critical section stability.
							bInReadContextsCriticalSection = false;
						} // End of Critical Section

						 /* POS14165 - NOTE: Once execution leaves the above critical section, 
						WE DO NOT OWN THE DATA ANYMORE! If dataQueued is true, then DO NOT
						read or write contextName or pContextData. Don't free them. Don't 
						delete them. Don't even acknowledge that they exist. */

						if (nContextsSinceCritSectFail)
						{
							// POS15617: Log this each time thru in case original critical-section failure message gets bump out of log due to excessive logging.
							trace(DCM_INFO) << _T("FAILURE: In critical section about ") << nContextsSinceCritSectFail << _T(" contexts ago.") << NCR::Util::endl;
							nContextsSinceCritSectFail++;
						}

						/* Notify the dispatcher thread that there is a new message available by 
						incrementing the semaphore. This count will be automatically decremented 
						when the dispatcher's wait function is signaled by this object. */
						ReleaseSemaphore(m_hContextSemaphore, 1, NULL);
					}
					else
					{
						trace(DCM_ERROR) << _T("Error: Retrieved ACS context with no context name.") << NCR::Util::endl;

#ifndef NDEBUG
						_ASSERT_BASE(FALSE, "Error: Retrieved ACS context with no context name.");
#endif
					}
				}
				else
				{
					/* This isn't supposed to happen, and likely indicates an error. DON'T IGNORE 
					THIS ASSERTION! GOT IT?! */
					trace(DCM_ERROR, _T("Error: ACSIOGetContextFields returned ACSIO_NO_DATA, indicating that no data was waiting."));
#ifndef NDEBUG
					_ASSERT_BASE(FALSE, "Error: ACSIOGetContextFields returned ACSIO_NO_DATA, indicating that no data was waiting.");
#endif
				}

				/* -SOTF8781 */

                // POS22495 - Only delete the content if the return is not ACSIO_NO_DATA - rrs
				// POS14165 - delete the BSTR and pointer if any failure happened to put the data in the queue
				if (hr != ACSIO_NO_DATA && !dataQueued)
				{
					/* We still own this data if the pointers were not transferred to 
					the queue. */

					/* TODO: Add a trace line explaining that this context was not 
					written to the queue. Output the context name. */
					trace(DCM_ERROR) 
						<< _T("An error occured putting a context in a queue[") << (LPCTSTR)contextName
						<< _T("]: ") << NCR::Util::endl;

					SysFreeString(contextName);
					VariantClear(pContextData);
					delete pContextData;
				}
				//e pos14165
			}
			else if (WAIT_OBJECT_0 == dwRet )
			{
				trace(DCM_INFO, _T("Disconnection from ACSIO was requested, exiting dispatcher..."));
				exitThread = true;
			}
			else if ((WAIT_OBJECT_0 + 1) == dwRet)
			{
				MSG msg = { 0 };

				while (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE)) 
				{ 
					if (msg.message == WM_QUIT)
					{
						trace(DCM_INFO, _T("Received a WM_QUIT Message, exiting Context dispatcher..."));
						exitThread = true;
					}

					TranslateMessage(&msg);
					DispatchMessage(&msg);			
				}
			}
			else 
			{
				// other returns are WAIT_FAILED, WAIT_ABANDONED, WAIT_TIMEOUT which we should never get
				trace(DCM_INFO, _T("Unexpected DispatchContexts Wait return code."));
				_ASSERT(FALSE);
				exitThread = true;
			}
		}

		CloseHandle(m_hContextSemaphore);

		trace(DCM_INFO, _T("Exiting Context Listener and dispatcher worker thread."));
	}

	// POS15617: Terminate dispatcher thread only if it exists.
	if (m_hContextThread)
	{
		DWORD waitResult = WaitForSingleObject(m_hContextThread, 20000);

		if (waitResult == WAIT_TIMEOUT)
		{
			trace(DCM_ERROR, L"Dispatch thread has not closed. Terminating thread forcibly.");
			TerminateThread(m_hContextThread, -1);
		}

		CloseHandle(m_hContextThread);
	}

	return 0;
}


DWORD WINAPI DispatchContextsProc(LPVOID pvParam)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	_ASSERT(SUCCEEDED(hr));

	DWORD retVal = -1;
	CACSHook* pHook = (CACSHook*)pvParam;

	if (retVal)
	{
		retVal = pHook->DispatchContexts();
	}

	CoUninitialize();
	return retVal;
}


DWORD CACSHook::DispatchContexts(void)
{
	Trace trace(_T("CACSHook::DispatchContexts"));

	HANDLE waitHandles[] = { m_hContextSemaphore, m_hDisconnectEvent };
	static long nContextsSinceCritSectFail = 0;  // POS15617: For debugging purposes
	static long nContextsSinceQueueEmpty   = 0;  // POS15617: For debugging purposes
	bool bQueueEmpty; //POS15617
	bool exitThread = false;

	while (!exitThread)
	{
		/* Wait with no timeout for the object that signals when the monitor thread had 
		queued a context message. If this function returns WAIT_OBJECT_0 then the object 
		is signaled and a context is waiting. */
		DWORD waitResult = WaitForMultipleObjects(
			sizeof(waitHandles) / sizeof(waitHandles[0]), 
			waitHandles, 
			FALSE, 
			INFINITE);

		/* A context is waiting on the queue */
		if (waitResult == WAIT_OBJECT_0)
		{
			/* SOTF10760 */
			BSTR contextName = 0;
			VARIANT* pContextData = 0;

			// POS15617: Can a Critical Section be bypassed? Not sure, so indicate queue-empty just in case.
			bQueueEmpty = true;

			{	// Begin Critical Section so that the context queue is not compromised.
				// Keep critical section as short as possible, so perform processing 
				// of context after this section. 
				CCriticalSectionLock lock(m_csContextQueue, TRUE); //POS15617: Add TRUE parameter to enable critical section.

				// POS15617: For debugging purposes, keep track of critical section stability.
				bInDispatchContextsCriticalSection = true;

				if (bInReadContextsCriticalSection)
				{
					// POS15617: Critical section has been compromised, so log failure.
					trace(DCM_INFO) << _T("FAILURE: In critical section") << NCR::Util::endl;
					if (!nContextsSinceCritSectFail) { nContextsSinceCritSectFail++; }
				}

				// POS15617: The context queue should not be empty but check anyway so that a false context is not read from the queue.
				bQueueEmpty = m_contextQueue.empty();
				if (bQueueEmpty)
				{
					trace(DCM_INFO) << _T("FAILURE: Queue is empty") << NCR::Util::endl;
					if (!nContextsSinceQueueEmpty) { nContextsSinceQueueEmpty++; }
				}

				if (!bQueueEmpty) // POS15617: If context queue is not empty, read next context from queue
				{
					// Read next context from queue
					ContextPair contextPair = m_contextQueue.front();
					//e POS 13219

					/* POS14165 - Transfer ownership of context name and context data pointer from the queue 
					to this thread. We are now responsible for freeing them when we're done 
					with them. */
					contextName = contextPair.first;

					pContextData = contextPair.second;
					//e POS14165

					if (SysStringLen(contextName) == 0)
					{
						trace(DCM_ERROR) << _T("Error: Retrieved ACS context with no context name.") << NCR::Util::endl;
					}

					// POS15617: It has been proven that it is okay to pop (remove) the context from queue before it is processed.
					m_contextQueue.pop();
				}

				// POS15617: For debugging purposes, keep track of critical section stability.
				bInDispatchContextsCriticalSection = false;
			}	// End of Critical Section

			if (!bQueueEmpty) // POS15617: If context queue was not empty, process context.
			{
				// Process the context outside the Critical Section to keep performance good.
				ProcessContexts(contextName, *pContextData);

				trace(DCM_INFO) << _T("ACS Context after processing: '") << (contextName ? (LPCTSTR)contextName : _T("<null>")) << _T("'") << NCR::Util::endl; // POS15617

				// Processing of context is complete, so free the buffers.
				// pos14165 - free 
				SysFreeString(contextName);
				VariantClear(pContextData);
				delete pContextData;
			}

			if (nContextsSinceCritSectFail)
			{
				// POS15617: Log this each time thru in case original critical-section failure message gets bump out of log due to excessive logging.
				trace(DCM_INFO) << _T("FAILURE: In critical section about ") << nContextsSinceCritSectFail << _T(" contexts ago.") << NCR::Util::endl;
				nContextsSinceCritSectFail++;
			}

			if (nContextsSinceQueueEmpty)
			{
				// POS15617: Log this each time thru in case original "Queue is empty" message gets bump out of log due to excessive logging.
				trace(DCM_INFO) << _T("FAILURE: Queue was found empty about ") << nContextsSinceQueueEmpty << _T(" contexts ago.") << NCR::Util::endl;
				nContextsSinceQueueEmpty++;
			}

			/* SOTF10760 */
		}
		/* An exit event was signaled. */
		else
		{
			exitThread = true;
		}
	}

	return 0;
}

/* SOTF8781 -- Changed parameter types from BSTR and VARIANT */
void CACSHook::ProcessContexts(const BSTR contextName, const VARIANT& contextData)
{
   Trace trace(_T("CACSHook::ProcessContexts"));
	trace(DCM_INFO) 
		<< _T("ACS Context arrived: '") << (contextName ? (LPCTSTR)contextName : _T("<null>")) /* SOTF9397 */
		<< _T("'") << NCR::Util::endl;

	/* SOTF9397 PLEASE report this assertion to the ACSIO integration team. This may be serious. -- PMP */
	_ASSERTE(contextName);

	/* SOTF9397 Test for null context name */
	if (contextName)
	{
		// POS14165 the original call is bad as it will only create a temp class for aContextName that goes out of scope immediately
		// and would create a nullpointer exception.  Changed the call with the proper way
		//LPCSTR aContextName = CW2A(contextName);
		CW2A aContextName(contextName);

		if (trace.IsLevel(DCM_DEBUG))
		{
			HANDLE contfielditer = DkAllocContextFieldIterator(aContextName);

			if (contfielditer)
			{
				SAFEARRAY* pSA = contextData.parray;
				SAFEARRAYBOUND indices[1] = {0};

				LPCSTR fieldName = 0;
				VARTYPE type = VT_EMPTY;
				DWORD size = 0;
				DWORD offset = 0;
				long aLong[1] = {0};
				long index = 0;
				indices[0].lLbound = 0;
				indices[0].cElements = DkGetContextFieldCount(aContextName);
				VARIANT var;
				VariantInit(&var);

				while (DkQueryContextFieldIteratorEx(contfielditer, &fieldName, &type, NULL, NULL, NULL))
				{
					aLong[0] = index;
					SafeArrayGetElement(pSA, aLong, &var);

					/* Switch on the the type of the field to determine how it will be output. If the type 
					of the value that was received is VT_EMPTY, that means that no value was present in the 
					structure that was received from ACS -- in other words, the pointer was null. That situation 
					needs to be handled differently than if a value had been filled in. */
					switch (type)
					{
					case VT_BSTR:
						trace(DCM_DEBUG) << "  CH* " << fieldName << " = ";

						if (var.vt != VT_EMPTY)
						{
							_bstr_t value = var;
							trace(DCM_DEBUG) << "\"" << (value.length() ? static_cast<LPCTSTR>(value) : _T("<null>")) << "\"";
						}

						break;

					case VT_I2:
						trace(DCM_DEBUG) << "  IN* " << fieldName << " = ";

						if (var.vt != VT_EMPTY)
						{
							trace(DCM_DEBUG) << std::dec << var.iVal;
						}

						break;

					case VT_I4:
						trace(DCM_DEBUG) << "  LG* " << fieldName << " = ";

						if (var.vt != VT_EMPTY)
						{
							trace(DCM_DEBUG) << std::dec << var.lVal;
						}

						break;

					case VT_UI2:
						trace(DCM_DEBUG) << "  UI* " << fieldName << " = ";

						if (var.vt != VT_EMPTY)
						{
							trace(DCM_DEBUG) << std::dec << var.uiVal;
						}

						break;

					default:
						trace(DCM_DEBUG) << "  ??* " << fieldName << " = ";

						_bstr_t value = var;
						trace(DCM_DEBUG) << "\"" << (value.length() ? static_cast<LPCTSTR>(value) : _T("<null>")) << "\"";

						break;
					}

					trace(DCM_DEBUG) << NCR::Util::endl;

					VariantClear(&var);
					++index;
				}

				DkFreeContextFieldIterator(contfielditer);
			}
		}

		/* Do we care about this context? */
		if (m_Contexts.IsContextApplicable(contextName))
		{
			if (!m_appModel.IsIgnoreContext(contextName))
			{
				trace(DCM_INFO) 
					<< _T("Saving ACS Context: '") << (contextName ? (LPCTSTR)contextName : _T("<null>"))
					<< _T("'") << NCR::Util::endl;

				/* Save the context info and set data arrival timestamp */
				if( m_Contexts.SaveContext(contextName, contextData) )
				{
					/* Update controls with context field information */
					trace(DCM_INFO, _T("Updating Context Bound ControlFields..."));
					m_Controls.UpdateContextBoundControlFields(contextName, contextData);

					/* Update properties bound to context fields */
					trace(DCM_INFO, _T("Updating Context Bound Properties..."));
					m_Properties.UpdateContextBoundProperties(contextName, contextData);

					/* Now that we've updated our state information 
					see if you can find a state match. 
					NOTE: Running state detection before context events will allow
					us to reference the current context in the detection in 
					the case where ExecuteOnContextEvent might reset the context. */
					trace(DCM_INFO, _T("Running State match detection..."));
					State const& state = m_appModel.StateDetect(contextName);

					if (state != State::nullState)
					{
						Utils.Properties().Set(_T("__POSState"), state.Name());
					}

					/* Perform any Context event actions here */
					trace(DCM_INFO) 
						<< _T("Performing any defined Context '") << contextName
						<< _T("' Events...") << NCR::Util::endl;
					m_appModel.ExecuteOnContextEvent(contextName);


					if (state != State::nullState)
					{
						trace(DCM_INFO) 
							<< _T("State match found: '") << state.Name()
							<< _T("'.") << NCR::Util::endl;

						/* Perform the last screen's OnStateExit calls */
						trace(DCM_INFO) 
							<< _T("Performing OnStateExit for previous state '") << m_strCurrentState.GetString()
							<< _T("'.") << NCR::Util::endl;
						m_appModel.ExecuteOnStateExit();

						/* Perform the new state's OnStateEnter calls */
						trace(DCM_INFO) 
							<< _T("Performing OnStateEnter for new state '") << state.Name()
							<< _T("'.") << NCR::Util::endl;
						m_appModel.ExecuteOnStateEnter();

						FireStateChange(
							_bstr_t(state.name.c_str()), 
							state.groupBstr,
							state.ignoreDuplicateBstr,
							state.triggerDelayBstr
							);
					}
					else
					{
						trace(DCM_INFO, _T("State detection has not found a match."));
					}
				}
				else
				{
					trace(DCM_ERROR)
						<< _T("Failed to set the '") << contextName
						<< _T("' context information.  Will not be processing state detection.") << NCR::Util::endl;
				}
			}
			else
			{
				trace(DCM_INFO) << _T("Currently ignoring context '") << contextName
					<< _T("'.") << NCR::Util::endl;
			}
		}
		else
		{
			trace(DCM_INFO) 
				<< _T("Context '") << contextName
				<< _T("' is not applicable.") << NCR::Util::endl;
		}
	}
	else
	{
		trace(DCM_ERROR, _T("ERROR: Context name is empty"));
	}
}
