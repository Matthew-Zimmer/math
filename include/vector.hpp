#pragma once
#include <algorithm>
#include <array>
#include <numeric>

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
            return std::inner_product(left.begin(), left.end(), right.begin(), static_cast<typename Type::Element_Type>(0));
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

    namespace Detail
    {
        template <typename Type, std::size_t N, std::size_t M>
        auto Join(std::array<Type, N>&& u, std::array<Type, M>&& v)
        {
            std::array<Type, N + M> r;
            std::move(u.begin(), u.end(), r.begin());
            std::move(v.begin(), v.end(), r.begin() + N);
            return r;
        }
    }

    template <std::size_t Dim, typename Type>
    class Vector : public Is<Vector<Dim, Type>, 
        Variables<
            V::Fixed_Data<Dim, Type>>, 
        Features<
            Operatorable, 
            Inner_Product, 
            Euclidean_Norm>>
    {
    public:
        static constexpr std::size_t dim = Dim;
        using Element_Type = Type;
        template <typename ... Types, typename = std::enable_if_t<sizeof...(Types) == Dim>>
        Vector(Types&& ... args) : Vector<Dim, Type>::Inherit{ V::Fixed_Data<Dim, Type>{ std::array{ static_cast<Type>(std::forward<Types>(args))... } } }
        {}
        template <typename T, typename ... Types, typename = std::enable_if_t<sizeof...(Types) + 1 < Dim>>
        Vector(T&& arg, Types&& ... args) : Vector<Dim, Type>::Inherit{ V::Fixed_Data<Dim, Type>{ Detail::Join(std::array{ static_cast<Type>(std::forward<T>(arg)), static_cast<Type>(std::forward<Types>(args))... }, std::array<Type, Dim - sizeof...(Types) - 1>{}) } }
        {}
        Vector() = default;
    };

    template <typename Type>
    class Vector<0, Type>;
}