#pragma once
#include <array>

#include <cmath>

#include <reflection/reflection.hpp>
#include <reflection/variables.hpp>
#include <reflection/operator_helpers.hpp>

namespace Slate::Math
{
    namespace V
    {
        using Variable::Base;
        template <std::size_t Size, typename Type>
        class Fixed_Data : public Base<std::array<Type, Size>>
        {
        public:
            auto& data() { return this->variable(); }
            auto const& data() const { return this->variable(); }
        };
    }

    template <typename Type>
    class Operatorable
    {
    public:
        using Required_Features = Features<Operators::Addable, Operators::Subtractable>;

        template <typename T>
        auto& operator+=(T const& other) &
        {
            static_assert(Type::dim == T::dim);
            auto iter = Meta::cast<T>(other).data().begin();
            for (auto& x : Meta::cast<Type>(*this).data())
                x += *iter++;
            return Meta::cast<Type>(*this);
        }
        
        template <typename T>
        auto& operator-=(T const& other) &
        {
            static_assert(Type::dim == T::dim);
            auto iter = Meta::cast<T>(other).data().begin();
            for (auto& x : Meta::cast<Type>(*this).data())
                x -= *iter++;
            return Meta::cast<Type>(*this);
        }
        
        template <typename T>
        auto& operator*=(T const& other) &
        {
            for (auto& x : Meta::cast<Type>(*this).data())
                x *= other;
            return Meta::cast<Type>(*this);
        }
        
        template <typename T>
        auto&& operator*=(T const& other) &&
        {
            return std::move(*this *= other);
        }
        
        template <typename T>
        auto& operator/=(T const& other) &
        {
            for (auto& x : Meta::cast<Type>(*this).data())
                x /= other;
            return *this;
        }

        template <typename T>
        auto&& operator/=(T const& other) &&
        {
            return std::move(*this /= other);
        }
        
        friend auto operator-(Type left)
        {
            return left *= -1;
        }

        auto& operator[](std::size_t index)
        {
            return Meta::cast<Type>(*this).data()[index];
        }
        auto const& operator[](std::size_t index) const
        {
            return Meta::cast<Type>(*this).data()[index];
        }

        auto begin()
        {
            return Meta::cast<Type>(*this).data().begin();
        }
        auto begin() const
        {
            return Meta::cast<Type>(*this).data().begin();
        }

        auto end()
        {
            return Meta::cast<Type>(*this).data().end();
        }
        auto end() const
        {
            return Meta::cast<Type>(*this).data().end();
        }
    };

    template <typename Type>
    class Inner_Product
    {
    public:
        template <typename T>
        friend auto operator*(Type const& left, T const& right)
        {
            static_assert(Type::dim == T::dim);
            auto left_iter = left.begin(), right_iter = right.begin();
            auto r = *left_iter++ * *right_iter++;
            while (left_iter != left.end())
                r += *left_iter++ * *right_iter++;
            return r;
        }
    };

    template <typename Type>
    class Euclidean_Norm
    {
    public:
        using Required_Features = Features<Operators::Comparable>;

        auto norm()
        {
            return std::sqrt(Meta::cast<Type>(*this) * Meta::cast<Type>(*this));
        }

        template <typename T>
        friend bool operator<(Type const& left, T const& right)
        {
            return left.norm() < right.norm();
        }
    };

    template <std::size_t Dim, typename Type>
    class Vector : public Is<Vector<Dim, Type>, Variables<V::Fixed_Data<Dim, Type>>, Features<Operatorable, Inner_Product, Euclidean_Norm>>
    {
    public:
        static constexpr std::size_t dim = Dim;
    };

    template <typename Type>
    class Vector<0, Type>;
}