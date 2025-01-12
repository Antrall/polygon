import numpy as np

# функция для нахождения суммы элементов главной диагонали
def sum_diagonal(matrix: np.array) -> int | str:
    # проверяем, квадратная ли матрица
    if matrix.shape[0] != matrix.shape[1]:
        raise ValueError("Ошибка: матрица должна быть квадратной.")
    # используем функцию, возвращающую след матрицы
    return np.trace(matrix)

# функция с тестами
def tests():
    # единичная матрица 2x2
    matrix1 = np.array([[1, 0], [0, 1]])
    assert sum_diagonal(matrix1) == 2, 'Ошибка при тестировании'

    # нулевая матрица 3x3
    matrix2 = np.array([[0, 0, 0], [0, 0, 0], [0, 0, 0]])
    assert sum_diagonal(matrix2) == 0, 'Ошибка при тестировании'

    # матрица 1x1
    matrix3 = np.array([[10]])
    assert sum_diagonal(matrix3) == 10, 'Ошибка при тестировани'

    # случайная матрица 3x3
    matrix4 = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
    assert sum_diagonal(matrix4) == 15, 'Ошибка при тестировании'

    # матрица, для которой должна выводиться ошибка
    matrix5 = np.array([[10, 14]])
    try:
        result = sum_diagonal(matrix5)
        print(result)
    except ValueError as e:
        print(e)

if __name__ == "__main__":
    tests()