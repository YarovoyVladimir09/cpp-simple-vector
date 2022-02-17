#pragma once

#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <iterator>
#include <utility>

#include "array_ptr.h"
class ReserveProxyObj {
public:
    ReserveProxyObj(size_t capacity_to_reserve) :capacity_(capacity_to_reserve) {
    };
    size_t capacity_;
};
ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) :point_(size), size_(size), capacity_(size) {
        // Напишите тело конструктора самостоятельно
        Type it{};
        Fill((point_.Get()), (point_.Get() + size), it);
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) :point_(size), size_(size), capacity_(size) {
        // Напишите тело конструктора самостоятельно
        Fill((point_.Get()), (point_.Get() + size), value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) :point_(init.size()), size_(init.size()), capacity_(init.size()) {
        int i = 0;
        for (auto step : init) {
            point_[i] = step;
            ++i;
        }
    }
    SimpleVector(ReserveProxyObj obj):point_(obj.capacity_), capacity_(obj.capacity_) {
        Type it{};
        Fill((point_.Get()), (point_.Get() + capacity_), it);
    }

    SimpleVector(const SimpleVector& other) {
        SimpleVector tmp(other.size_);
        std::copy(other.begin(), other.end(), tmp.begin());
        swap(tmp);

    }

    SimpleVector( SimpleVector&& other):point_(other.size_),size_(other.size_),capacity_(other.size_) {
        SimpleVector tmp(other.size_);
        std::copy(std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()), tmp.begin());
        other.size_ = 0;
        swap(tmp);
    }
    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }
    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }
    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            capacity_ = new_capacity;
            ArrayPtr<Type> buffer(capacity_);
            Type it{};
            Fill((buffer.Get()), (buffer.Get() + capacity_), it);
            std::copy(std::make_move_iterator(point_.Get()), std::make_move_iterator(point_.Get() + size_), buffer.Get());
            buffer.swap(point_);
        }
    }
    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return point_[index];
    }
    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return point_[index];
    }
    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("");
        }
        return point_[index];
    }
    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("");
        }
        return point_[index];
    }
    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }
    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        // Напишите тело самостоятельно

        if (new_size > capacity_) {

            capacity_ = std::max(new_size, capacity_ * 2);

            ArrayPtr<Type> buffer(capacity_);
            Type it{};

            Fill((buffer.Get()), (buffer.Get() + capacity_), it);

            std::copy(std::make_move_iterator(point_.Get()), std::make_move_iterator(point_.Get() + size_), buffer.Get());

            buffer.swap(point_);

        }
        else if (new_size > size_)
        {
            ArrayPtr<Type> buffer(new_size);
            Type it{};

            Fill((buffer.Get()), (buffer.Get() + new_size), it);

           std::copy(std::make_move_iterator(point_.Get()), std::make_move_iterator(point_.Get() + size_), buffer.Get());

            buffer.swap(point_);
        }
        else if (new_size <= size_)
        {
            ArrayPtr<Type> buffer(new_size);
            Type it{};

            Fill((buffer.Get()), (buffer.Get() + new_size), it);

           std::copy(std::make_move_iterator(point_.Get()), std::make_move_iterator(point_.Get() + new_size), buffer.Get());

            buffer.swap(point_);
        }
        size_ = new_size;
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        if (this != &rhs) {
            auto rhs_copy(rhs);
            swap(rhs_copy);
        }
        return *this;
    }
    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack( Type&& item) {
        Insert(end(), std::move(item));
    }

    void PushBack(const Type& item) {
        Insert(end(), item);
        // Напишите тело самостоятельно
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, Type&& value) {
        // Напишите тело самостоятельно
        size_t length = std::distance(begin(), Iterator(pos));
        size_t length_end = std::distance(begin(), end());
        if (size_ == capacity_) {
            if (size_ == 0) {
                capacity_ = length + 1;
                ArrayPtr<Type> buff(capacity_);
                Type it{};
                Fill((buff.Get()), (buff.Get() + capacity_), it);
                std::exchange(buff[length], std::move(value));
                point_.swap(buff);
            }
            else if (length > length_end) {
                return {};
            }
            else {
                capacity_ = std::max(length, capacity_ * 2);
                ArrayPtr<Type> buff(capacity_);
                std::copy(std::make_move_iterator(begin()), std::make_move_iterator(Iterator(pos)), buff.Get());
                std::exchange(buff[length], std::move(value));
                std::copy_backward(std::make_move_iterator(Iterator(pos)), std::make_move_iterator(end()), buff.Get() + length_end + 1);
                point_.swap(buff);
            }
        }
        else {
            std::copy_backward(std::make_move_iterator(Iterator(pos)), std::make_move_iterator(end()), end() + 1);
            std::exchange(point_[length], std::move(value));
        }
        ++size_;
        return &point_[length];
    }

    Iterator Insert(ConstIterator pos, const Type& value) {
        size_t length = std::distance(begin(), Iterator(pos));
        size_t length_end = std::distance(begin(), end());
        if (size_ == capacity_) {
            if (size_ == 0) {
                capacity_ = length + 1;
                ArrayPtr<Type> buff(capacity_);
                Type it{};
                std::fill(buff.Get(), buff.Get() + capacity_, it);
                buff[length] = value;
                point_.swap(buff);
            }
            else if (length > length_end) {
                return {};
            }
            else {
                capacity_ = std::max(length, capacity_ * 2);
                ArrayPtr<Type> buff(capacity_);
                std::copy(begin(), Iterator(pos), buff.Get());
                buff[length] = value;
                std::copy_backward(Iterator(pos), end(), buff.Get() + length_end + 1);
                point_.swap(buff);
            }
        }
        else {

            std::copy_backward(Iterator(pos), end(), end() + 1);
            point_[length] = value;
        }
        ++size_;
        return &point_[length];
    }
    
    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if (!(size_ == 0)) {
            --size_;
        }
        // Напишите тело самостоятельно
    }
    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        if (pos == end()) {
            --size_;
            return end();
        }
        auto length = std::distance(begin(), Iterator(pos));
        auto temp = pos + 1;
        std::copy(std::make_move_iterator(Iterator(pos+1)), std::make_move_iterator(end()), Iterator(pos));
        --size_;
        return Iterator(&point_[length]);
    }
    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        other.point_.swap(point_);
        std::swap(other.capacity_, capacity_);
        std::swap(other.size_, size_);
    }
    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return  Iterator(&point_[0]);
        // Напишите тело самостоятельно
    }
    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        // Напишите тело самостоятельно
        return  Iterator(&point_[size_]);
    }
    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        // Напишите тело самостоятельно
        return ConstIterator(&point_[0]);
    }
    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        // Напишите тело самостоятельно
        return ConstIterator(&point_[size_]);
    }
    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        // Напишите тело самостоятельно
        return ConstIterator(&point_[0]);
    }
    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        // Напишите тело самостоятельно
        return ConstIterator(&point_[size_]);
    }

private:
    ArrayPtr<Type> point_;
    size_t size_ = 0;
    size_t capacity_ = 0;

    template<typename It>
    void Fill(It first, It last, Type& value) {
        auto it = first;
        while (it != last) {
            std::exchange(*(it), std::move(Type()));
            ++it;
        }
    }

    template<typename It>
    void Fill(It first, It last, const Type& value) {
        auto it = first;
        while (it != last) {
            *(it) = value;
            ++it;
        }
    }
};


template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return (lhs < rhs || lhs == rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(lhs <= rhs);
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(lhs < rhs);
}