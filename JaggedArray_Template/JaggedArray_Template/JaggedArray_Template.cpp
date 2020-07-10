// JaggedArray_Template.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <unordered_map>

template <class T>
class DynamicJaggedArray
{
private: 
    DynamicJaggedArray() {}

public:
    DynamicJaggedArray(const std::unordered_map<size_t, size_t> &data)
    {
        mapRowCol = data;
        _mJaggedArr.reserve(mapRowCol.size() - 1); // This is no. of rows for arrays. No additional row won't get added
    }

    void insertRowsData(const int &rowIndex, const std::vector<T> &colsData)
    {
        if (colsData.size() > mapRowCol.find(rowIndex)->second)
            throw std::out_of_range("column index out of range in " __FUNCTION__);

        if (_mJaggedArr.size() < _mJaggedArr.capacity())
            _mJaggedArr.push_back(colsData);
        else
            throw std::out_of_range("Max number of rows capacity reached for array");
    }

private:
    std::unordered_map<size_t, size_t> mapRowCol;
    std::vector<std::vector<T>> _mJaggedArr;
};

void createIntJaggedArray()
{
    std::vector<size_t> v1{ 10, 20, 30, 40, 50 };
    std::vector<size_t> v2{ 11, 12, 13 };
    std::vector<size_t> v3{ 100, 200 };
    std::vector<size_t> v4{ 101, 201, 301, 401, 501, 601, 701 };
    std::vector<size_t> v5{ 1000, 2000 };

    std::unordered_map <size_t, size_t> mNumberOfColsPerRow({ { 0, 5 }, {1, 3}, {2, 2}, {3, 7}, {4, 2} });
    DynamicJaggedArray<size_t> data(mNumberOfColsPerRow);

    try {
        data.insertRowsData(0, v1);
        data.insertRowsData(1, v2);
        data.insertRowsData(2, v3);
        data.insertRowsData(3, v4);
        //data.insertRowsData(4, v5);
        std::cout << "Array filled with Data..." << std::endl;
    }
    catch (std::out_of_range& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void createCharJaggedArray()
{
    std::vector<char> v1{'a','b','c','d','e' };
    std::vector<char> v2{ 'f','g','h' };
    std::vector<char> v3{ 'i','j' };
    std::vector<char> v4{ 'k','l','m','n','o', 'p', 'q' };
    std::vector<char> v5{ 'r', 's' };

    std::unordered_map <size_t, size_t> mNumberOfColsPerRow({ { 0, 5 }, {1, 3}, {2, 2}, {3, 7}, {4, 2} });
    DynamicJaggedArray<char> data(mNumberOfColsPerRow);

    try {
        data.insertRowsData(0, v1);
        data.insertRowsData(1, v2);
        data.insertRowsData(2, v3);
        data.insertRowsData(3, v4);
        //data.insertRowsData(4, v5); // Uncomnment this piece of code will try to push an additional row, 
        // this action gets prevented and an exception will be thrown...
        std::cout << "Array filled with Data..." << std::endl;
    }
    catch (std::out_of_range& e)
    {
        std::cout << e.what() << std::endl;
    }
}

int main()
{
    createCharJaggedArray();
    return 0;
}


