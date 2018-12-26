#include <Windows.h>

namespace SmartPointers
{
	template <typename T>
	class SmartPointer
	{
	private:
		T* data;
		int kod = 0;
		HANDLE mutex;
		int numberOfReferences;
		const LPCWSTR mutexName = L"Mutex_Name";

		T* getPointer(T* data)
		{
			WaitForSingleObject(mutex, INFINITE);

			if (numberOfReferences == 0) {
				numberOfReferences++;
				ReleaseMutex(mutex);
				return data;
			}
			else {
				ReleaseMutex(mutex);
				return NULL;
			}
		}

	public:
		SmartPointer() : data(NULL), numberOfReferences(0) {}

		SmartPointer(T* value) {
			numberOfReferences = 0;
			mutex = NULL;
			if (value != NULL) {
				mutex = CreateMutex(NULL, FALSE, mutexName);
				data = getPointer(value);
			}
		}

		T* operator-> () {
			WaitForSingleObject(mutex, INFINITE);	
		
			return data;	
		}
		


		void release() { ReleaseMutex(mutex);}

		~SmartPointer() {
			delete data;
		}

	};
}