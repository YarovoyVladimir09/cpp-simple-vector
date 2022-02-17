#pragma once
#include <cassert>
#include <stdexcept>
#include <iostream>

inline void Test1() {
    // ������������� ������������� �� ���������
    {
        SimpleVector<int> v;
        assert(v.GetSize() == 0u);
        assert(v.IsEmpty());
        assert(v.GetCapacity() == 0u);
    }

    // ������������� ������� ���������� �������
    {
        SimpleVector<int> v(5);
        assert(v.GetSize() == 5u);
        assert(v.GetCapacity() == 5u);
        assert(!v.IsEmpty());
        for (size_t i = 0; i < v.GetSize(); ++i) {
            assert(v[i] == 0);
        }
    }

    // ������������� �������, ������������ �������� ���������
    {
        SimpleVector<int> v(3, 42);
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() == 3);
        for (size_t i = 0; i < v.GetSize(); ++i) {
            assert(v[i] == 42);
        }
    }

    // ������������� ������� ��� ������ initializer_list
    {
        SimpleVector<int> v{ 1, 2, 3 };
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() == 3);
        assert(v[2] == 3);
    }

    // ������ � ��������� ��� ������ At
    {
        SimpleVector<int> v(3);
        assert(&v.At(2) == &v[2]);
        try {
            v.At(3);
            assert(false);  // ��������� ������������ ����������
        }
        catch (const std::out_of_range&) {
        }
        catch (...) {
            assert(false);  // �� ��������� ����������, �������� �� out_of_range
        }
    }

    // ������� �������
    {
        SimpleVector<int> v(10);
        const size_t old_capacity = v.GetCapacity();
        v.Clear();
        assert(v.GetSize() == 0);
        assert(v.GetCapacity() == old_capacity);
    }

    // ��������� �������
    {
        SimpleVector<int> v(3);
        v[2] = 17;
        v.Resize(7);
        assert(v.GetSize() == 7);
        assert(v.GetCapacity() >= v.GetSize());
        assert(v[2] == 17);
        assert(v[3] == 0);
    }
    {
        SimpleVector<int> v(3);
        v[0] = 42;
        v[1] = 55;
        const size_t old_capacity = v.GetCapacity();
        v.Resize(2);
        assert(v.GetSize() == 2);
        assert(v.GetCapacity() == old_capacity);
        assert(v[0] == 42);
        assert(v[1] == 55);
    }
    {
        const size_t old_size = 3;
        SimpleVector<int> v(3);
        v.Resize(old_size + 5);
        v[3] = 42;
        v.Resize(old_size);
        v.Resize(old_size + 2);
        assert(v[3] == 0);
    }

    // ������������ �� SimpleVector
    {
        // ������ ������
        {
            SimpleVector<int> v;
            assert(v.begin() == nullptr);
            assert(v.end() == nullptr);
        }

        // �������� ������
        {
            SimpleVector<int> v(10, 42);
            assert(v.begin());
            assert(*v.begin() == 42);
            assert(v.end() == v.begin() + v.GetSize());
        }
    }
}

inline void Test2() {
    // PushBack
    {
        SimpleVector<int> v(1);
        v.PushBack(42);
        assert(v.GetSize() == 2);
        assert(v.GetCapacity() >= v.GetSize());
        assert(v[0] == 0);
        assert(v[1] == 42);

        for (auto step : v) {
            std::cout << step << " ";
        }
        std::cout << std::endl;
    }



    // ���� ������� �����, PushBack �� ����������� Capacity
    {
        SimpleVector<int> v(2);
        v.Resize(1);
        const size_t old_capacity = v.GetCapacity();
        v.PushBack(123);
        assert(v.GetSize() == 2);
        assert(v.GetCapacity() == old_capacity);
    }

    // PopBack
    {
        SimpleVector<int> v{ 0, 1, 2, 3 };
        const size_t old_capacity = v.GetCapacity();
        const auto old_begin = v.begin();
        v.PopBack();
        assert(v.GetCapacity() == old_capacity);
        assert(v.begin() == old_begin);
        assert((v == SimpleVector<int>{0, 1, 2}));
    }

    // ����������� �����������
    {
        SimpleVector<int> numbers{ 1, 2 };
        auto numbers_copy(numbers);
        assert(&numbers_copy[0] != &numbers[0]);
        assert(numbers_copy.GetSize() == numbers.GetSize());
        for (size_t i = 0; i < numbers.GetSize(); ++i) {
            assert(numbers_copy[i] == numbers[i]);
            assert(&numbers_copy[i] != &numbers[i]);
        }
        for (auto step : numbers) {
            std::cout << step << " ";
        }
        std::cout << std::endl;
        for (auto step : numbers_copy) {
            std::cout << step << " ";
        }
        std::cout << std::endl;
    }

    // ���������
    {
        assert((SimpleVector{ 1, 2, 3 } == SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 3 } != SimpleVector{ 1, 2, 2 }));

        assert((SimpleVector{ 1, 2, 3 } < SimpleVector{ 1, 2, 3, 1 }));
        assert((SimpleVector{ 1, 2, 3 } > SimpleVector{ 1, 2, 2, 1 }));

        assert((SimpleVector{ 1, 2, 3 } >= SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 4 } >= SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 3 } <= SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 3 } <= SimpleVector{ 1, 2, 4 }));
    }

    // ����� �������� ��������
    {
        SimpleVector<int> v1{ 42, 666 };
        SimpleVector<int> v2;
        v2.PushBack(0);
        v2.PushBack(1);
        v2.PushBack(2);
        const int* const begin1 = &v1[0];
        const int* const begin2 = &v2[0];

        const size_t capacity1 = v1.GetCapacity();
        const size_t capacity2 = v2.GetCapacity();

        const size_t size1 = v1.GetSize();
        const size_t size2 = v2.GetSize();

        static_assert(noexcept(v1.swap(v2)));
        for (auto step : v1) {
            std::cout << step << " ";
        }
        std::cout << std::endl;
        for (auto step : v2) {
            std::cout << step << " ";
        }
        std::cout << std::endl;
        v1.swap(v2);
        assert(&v2[0] == begin1);
        assert(&v1[0] == begin2);
        assert(v1.GetSize() == size2);
        assert(v2.GetSize() == size1);
        assert(v1.GetCapacity() == capacity2);
        assert(v2.GetCapacity() == capacity1);
        for (auto step : v1) {
            std::cout << step << " ";
        }
        std::cout << std::endl;
        for (auto step : v2) {
            std::cout << step << " ";
        }
        std::cout << std::endl;
    }

    // ������������
    {
        SimpleVector<int> src_vector{ 1, 2, 3, 4 };
        SimpleVector<int> dst_vector{ 1, 2, 3, 4, 5, 6 };
        dst_vector = src_vector;
        assert(dst_vector == src_vector);
    }

    // ������� ���������
    {
        SimpleVector<int> v{ 1, 2, 3, 4 };
        std::cout<<*(v.Insert(v.begin() + 2, 42))<<" ";
        for (auto step : v) {
            std::cout << step << " ";
        }
        assert((v == SimpleVector<int>{1, 2, 42, 3, 4}));
    }
std::cout << std::endl;
    {
        SimpleVector<int> v(6);
        std::cout << *(v.Insert(v.begin() + 2, 42)) << " ";
        for (auto step : v) {
            std::cout << step << " ";
        }

    }

    // �������� ���������
    {
        SimpleVector<int> v{ 1, 2, 3, 4 };
       std::cout<<*( v.Erase(v.cbegin() + 2))<<" ";
        assert((v == SimpleVector<int>{1, 2, 4}));
    }
}