#ifndef MYTOOLThread_H
#define MYTOOLThread_H

#include <iostream>
#include <string>

#include "Tool.h"

/**
 * \struct MyToolThread_args_args
 *
 * This is a struct to place data you want your thread to access or exchange with it. The idea is the
 * data inside is only used by the thread, and so will be thread safe
 *
 * $Author: B.Richards $
 * $Date: 2019/05/28 10:44:00 $
 */

struct MyToolThread_args : Thread_args {

		MyToolThread_args();
		~MyToolThread_args();
};

/**
 * \class MyToolThread
 *
 * This is a template for a Tool that produces a single thread that can be assigned a function separate to the
 * main thread. Please fill out the description and author information.
 *
 * $Author: B.Richards $
 * $Date: 2019/05/28 10:44:00 $
 */

class MyToolThread : public Tool {

	public:

		MyToolThread();  ///< Simple constructor
		bool Initialise(std::string configfile,
		                DataModel& data);  ///< Initialise Function for setting up Tool resources. @param
		                                   ///< configfile The path and name of the dynamic configuration file
		                                   ///< to read in. @param data A reference to the transient data
		                                   ///< class used to pass information between Tools.
		bool Execute();                    ///< Execute function used to perform Tool purpose.
		bool Finalise();                   ///< Finalise function used to clean up resources.

	private:

		static void Thread(
		    Thread_args* arg);  ///< Function to be run by the thread in a loop. Make sure not to block in it
		Utilities* m_util;      ///< Pointer to utilities class to help with threading
		MyToolThread_args* args;  ///< thread args (also holds pointer to the thread)
};

#endif
