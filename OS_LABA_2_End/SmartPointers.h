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

	public:
		SmartPointer(){}
		
		SmartPointer(SmartPointer &smrt) {
			data = smrt.data;
			numberOfReferences = smrt.numberOfReferences;
			numberOfReferences++;
		}


		SmartPointer(T* value) {
			numberOfReferences = 1;
			mutex = NULL;
			if (value != NULL) {
				mutex = CreateMutex(NULL, FALSE, mutexName);
				data = value;
			}
		}

		T* operator-> () {
			T* dataTest = NULL;
			WaitForSingleObject(mutex, INFINITE);	
			dataTest = getData();
			release();
			return dataTest;	
		}
		

		T* getData() { return data; }

		void release() { ReleaseMutex(mutex);}

		~SmartPointer() {
			WaitForSingleObject(mutex, INFINITE);
			remove();
			ReleaseMutex(mutex);
		}
		
		protected:
		void remove() {
			numberOfReferences--;
			if (numberOfReferences == 0) {
				delete data;
				numberOfReferences = NULL;
			}
		}
		

	};
}
