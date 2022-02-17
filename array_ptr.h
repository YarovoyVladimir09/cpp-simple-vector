#pragma once
#include <algorithm>
#include <cstdlib>

template <typename Type>
class ArrayPtr {
public:
    // �������������� ArrayPtr ������� ����������
    ArrayPtr() = default;

    // ������ � ���� ������ �� size ��������� ���� Type.
    // ���� size == 0, ���� raw_ptr_ ������ ���� ����� nullptr
    explicit ArrayPtr(size_t size) {
        // ���������� ����������� ��������������
        if (size <= 0) {
            raw_ptr_ = nullptr;
        }
        else {
            raw_ptr_ = new Type[size];
        }
    }

    // ������de����� �� ������ ���������, ��������� ����� ������� � ���� ���� nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept {
        // ���������� ����������� ��������������
        raw_ptr_ = raw_ptr;
    }

    // ��������� �����������
    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr() {
        // �������� ���������� ��������������
        delete[] raw_ptr_;
    }

    // ��������� ������������
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // ���������� ��������� �������� � ������, ���������� �������� ������ �������
    // ����� ������ ������ ��������� �� ������ ������ ����������
    [[nodiscard]] Type* Release() noexcept {
        Type* p = raw_ptr_;
        raw_ptr_ = nullptr;
        return p;
    }

    // ���������� ������ �� ������� ������� � �������� index
    Type& operator[](size_t index) noexcept {
        // ���������� �������� ��������������
        return *(raw_ptr_ + index);
    }

    // ���������� ����������� ������ �� ������� ������� � �������� index
    const Type& operator[](size_t index) const noexcept {
        // ���������� �������� ��������������
        return *(raw_ptr_ + index);
    }

    // ���������� true, ���� ��������� ���������, � false � ��������� ������
    explicit operator bool() const {
        // ��������. ���������� �������� ��������������
        return raw_ptr_ == nullptr ? false : true;
    }

    // ���������� �������� ������ ���������, ��������� ����� ������ �������
    Type* Get() const noexcept {
        // ��������. ���������� ����� ��������������
        return raw_ptr_;
    }

    // ������������ ��������� ��������� �� ������ � �������� other
    void swap(ArrayPtr& other) noexcept {
        // ���������� ����� ��������������
        std::swap(raw_ptr_, other.raw_ptr_);
    }

private:
    Type* raw_ptr_ = nullptr;
};