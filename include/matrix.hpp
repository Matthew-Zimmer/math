#pragma once
#include "vector.hpp"

namespace Slate::Math
{
    template <typename Type>
    class Matrix_Product
    {
    public:
        template <typename T>
        friend auto operator*(Type const& left, T const& right)
        {
            static_assert(Type::columns == T::rows);
            Type r;
            for (std::size_t i = 0; i < Type::rows; i++)
                for (std::size_t j = 0; j < T::columns; j++)
                    for (std::size_t k = 0; k < Type::columns; k++)
                        r[i][j] += left[i][k] * right[k][j];
            return r;
        }
    };

    template <std::size_t Row, std::size_t Column, typename Type>
    class Matrix : public Is<Matrix<Row, Column, Type>, Variables<V::Fixed_Data<Column, Vector<Row, Type>>>, Features<Operatorable, Matrix_Product>>
    {
    public:
        static constexpr std::size_t columns = Column;
        static constexpr std::size_t rows = Row;
        static constexpr std::size_t dim = Column;
    };

    template <std::size_t Row, typename Type>
    class Matrix<Row, 0, Type>;

    template <std::size_t Column, typename Type>
    class Matrix<0, Column, Type>;

    template <typename Type>
    class Matrix<0, 0, Type>;
}