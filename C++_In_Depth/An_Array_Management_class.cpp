/*A very basic array management class, just take care of extra allocation and deallocation.
All feel of arrays maintained only extra allocation and deallocation abstracted away. 
Operation like array element access and update remained same. */

// Version: 1.0

// Mufti Md

#include <iostream>

using namespace std;

template <class T>
class ArrayManagement
{
private:
	int nCapacity;
	int nSize;
	T* storage;

public:
	ArrayManagement(T* arrData, int nSizeParam)
	{
		nSize = nSizeParam;
		nCapacity = nSizeParam;

		storage = arrData;
	}

	T* AddData(T data)
	{
		T* newStorage;
		if (nCapacity == nSize)
		{
			newStorage = new T[(nCapacity * 3) / 2 + 1]; // Just an allocation ratio 
			memcpy(newStorage, storage, nSize*sizeof(T));
			nCapacity = ((nCapacity * 3) / 2 + 1);

			delete[]storage;
			newStorage[nSize++] = data;

			storage = newStorage;
		}
		else
		{
			storage[nSize++] = data;
		}

		return storage;
	}

	int getLength()
	{
		return nSize;
	}

	~ArrayManagement()
	{
		if (storage != NULL)
		{
			delete[]storage;
			storage = NULL;
		}
	}

};

int main()
{
	int* arr = new int[2];
	arr[0] = 3;
	arr[1] = 5;

	ArrayManagement<int> arrManage(arr, 2);
	arr = arrManage.AddData(50);
	arr = arrManage.AddData(60);
	arr = arrManage.AddData(70);
	arr = arrManage.AddData(80);
	arr = arrManage.AddData(90);
	arr = arrManage.AddData(100);
	int len = arrManage.getLength();

	for (int i = 0; i < len; ++i) {
		std::cout << arr[i] << std::endl;
	}

	return 0;
}