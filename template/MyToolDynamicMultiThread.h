#ifndef MYTOOLDynamicMultiThread_H
#define MYTOOLDynamicMultiThread_H

#include <iostream>
#include <string>

#include <DataModel.h>
#include "Tool.h"

/**
 * \struct MyToolDynamicMultiThread_args
 *
 * This is a struct to place data you want your thread to acess or exchange with it. The idea is the
data inside is only used by the thread, and so will be thread safe
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
*/

struct MyToolDynamicMultiThread_args : Thread_args {

		MyToolDynamicMultiThread_args();
		~MyToolDynamicMultiThread_args();
		bool busy;
		std::string message;
};

/**
 * \class MyToolDynamicMultiThread
 *
 * This is a template for a Tool that dynamically more or less threads, such that there is always 1 available
 * thread. This can therefore be used to scale to your workload, however be careful when using more than one
 * of these tools and to apply upper limits if necessary both locally within this tool and globally so that
 * more threads than is practical are created causing massive inefficiency. Please fill out the description
 * and author information.
 *
 * $Author: B.Richards $
 * $Date: 2019/05/28 10:44:00 $
 */

class MyToolDynamicMultiThread : public Tool {

	public:

		MyToolDynamicMultiThread();  ///< Simple constructor
		bool Initialise(std::string configfile,
		                DataModel& data);  ///< Initialise Function for setting up Tool resources. @param
		                                   ///< configfile The path and name of the dynamic configuration file
		                                   ///< to read in. @param data A reference to the transient data
		                                   ///< class used to pass information between Tools.
		bool Execute();                    ///< Executre function used to perform Tool purpose.
		bool Finalise();                   ///< Finalise function used to clean up resources.

	private:

		void CreateThread();  ///< Function to Create Thread
		void DeleteThread(
		    int pos);  ///< Function to delete thread @param pos is the position in the args vector below

		static void Thread(
		    Thread_args* arg);  ///< Function to be run by the thread in a loop. Make sure not to block in it
		Utilities* m_util;      ///< Pointer to utilities class to help with threading
		std::vector<MyToolDynamicMultiThread_args*>
		    args;  ///< Vector of thread args (also holds pointers to the threads)

		int m_freethreads;          ///< Keeps track of free threads
		unsigned long m_threadnum;  ///< Counter for unique naming of threads
};

#endif
