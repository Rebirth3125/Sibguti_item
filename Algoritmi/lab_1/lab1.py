import math


class Fraction:
  """
    Класс, реализующий дроби
    """
  __slots__ = ('_numerator', '_denominator')

  def __init__(self, numerator=0, denominator=1):
    if type(numerator) is not int or type(denominator) is not int:
      raise TypeError(
        'Fraction(%s, %s) - the numerator and denominator values must be integers'
        % (numerator, denominator))
    if denominator == 0:
      raise ZeroDivisionError('Fraction(%s, 0)' % numerator)
    g = math.gcd(numerator, denominator)
    if denominator < 0:
      g = -g
    numerator //= g
    denominator //= g
    self._numerator = numerator
    self._denominator = denominator

  def __add__(self, other):
    """Сумма 2-х дробей"""
    if isinstance(other, Fraction):
      return Fraction(
        self._numerator * other._denominator +
        other._numerator * self._denominator,
        self._denominator * other._denominator)
    return NotImplemented

  def __sub__(self, other):
    """Разность 2-х дробей"""
    if isinstance(other, Fraction):
      return Fraction(
        self._numerator * other._denominator -
        other._numerator * self._denominator,
        self._denominator * other._denominator)
    return NotImplemented

  def __mul__(self, other):
    """Произведение 2-х дробей"""
    if isinstance(other, Fraction):
      return Fraction(self._numerator * other._numerator,
                      self._denominator * other._denominator)
    return NotImplemented

  def __truediv__(self, other):
    """Частное 2-х дробей"""
    if isinstance(other, Fraction):
      return Fraction(self._numerator * other._denominator,
                      self._denominator * other._numerator)
    return NotImplemented

  def __lt__(self, other):
    """x < y"""
    if isinstance(other, Fraction):
      return self._numerator * other._denominator < other._numerator * self._denominator
    return NotImplemented

  def __le__(self, other):
    """x <= y"""
    if isinstance(other, Fraction):
      return self._numerator * other._denominator <= other._numerator * self._denominator
    return NotImplemented

  def __eq__(self, other):
    """x == y"""
    if isinstance(other, Fraction):
      return self._numerator * other._denominator == other._numerator * self._denominator
    return NotImplemented

  def __ne__(self, other):
    """x != y"""
    if isinstance(other, Fraction):
      return self._numerator * other._denominator != other._numerator * self._denominator
    return NotImplemented

  def __gt__(self, other):
    """x > y"""
    if isinstance(other, Fraction):
      return self._numerator * other._denominator > other._numerator * self._denominator
    return NotImplemented

  def __ge__(self, other):
    """x >= y"""
    if isinstance(other, Fraction):
      return self._numerator * other._denominator >= other._numerator * self._denominator
    return NotImplemented

  def __repr__(self):
    if self._denominator == 1:
      return 'Fraction(%s)' % self._numerator
    else:
      return 'Fraction(%s, %s)' % (self._numerator, self._denominator)

  def __str__(self):
    if self._denominator == 1:
      return str(self._numerator)
    else:
      return '%s/%s' % (self._numerator, self._denominator)

  def get_abs(self):
    return Fraction(abs(self._numerator), abs(self._denominator))


def print_matrix(matrix):
  for i in matrix:
    for j in i:
      print('%15s' % j, end='')
    print()
  print()


def jordan_gauss_method(matrix):
  print("<Начальная матрица>")
  print_matrix(matrix)
  step = 0
  for c in range(len(matrix)):
    if c+step >= len(matrix[c]):
      break
    index = c
    for i in range(c + 1, len(matrix)):

      if matrix[index][c].get_abs() < matrix[i][c].get_abs():
        index = i
    if index != c:
      matrix[index], matrix[c] = matrix[c], matrix[index]
      print("Change stroke")
      print_matrix(matrix)

    if matrix[c][c] == Fraction(0):

      while c + step + 2 < len(matrix[c]):
        if matrix[c][c + step] == Fraction(0):
          step2 = 0
          while c + step2 < len(matrix):
            if matrix[c + step2][c + step].get_abs() != Fraction(0):
              print("Change stroke")
              matrix[c + step2], matrix[c] = matrix[c], matrix[c + step2]
              step = step - 1
              break
            else:
              step2 = step2 + 1
          step += 1
        else:
          break

    if matrix[c][c + step] != Fraction(1) and matrix[c][c +
                                                        step] != Fraction(0):
      matrix[c] = [i / matrix[c][c + step] for i in matrix[c]]
      print_matrix(matrix)
    for i in range(len(matrix)):

      if matrix[i][c + step] == Fraction(0):

        continue
      if i == c:
        continue

      coeff = matrix[i][c + step] * Fraction(-1)

      for j in range(len(matrix[0])):
        matrix[i][j] = matrix[i][j] + matrix[c][j] * coeff
    print_matrix(matrix)

  no_null_str = 0
  for i in matrix:
    flag = True
    for j in i[:-1]:
      if j != Fraction(0):
        flag = False
        break
    if flag and i[-1] != Fraction(0):
      no_null_str = 0
      break
    elif flag and i[-1] == Fraction(0):
      continue
    no_null_str += 1

  print("Ответ:")
  if not no_null_str:
    print("нет решения")
  elif no_null_str == len(matrix) and no_null_str == len(matrix[0]) - 1:
    tmp = list()
    for i in range(len(matrix)):
      tmp.append("x" + str(i + 1) + " = " + str(matrix[i][-1]))
    print(*tmp, sep='\n')
  else:
    tmp = list()
    for i in range(len(matrix)):
      str_tmp = ""
      if matrix[i][i] == Fraction(1):
        str_tmp += "x" + str(i + 1) + " = " + str(matrix[i][-1])
        for j in range(len(matrix[0]) - 1):
          if j == i:
            continue
          if matrix[i][j] == Fraction(0):
            continue
          str_tmp += " + "
          str_tmp += "(" + str(
            matrix[i][j] * Fraction(-1)) + ")" + "x" + str(j + 1)
      else:
        flag = True
        for j in matrix[i][:-1]:
          if j != Fraction(0):
            flag = False
            break
        if flag and matrix[i][-1] == Fraction(0):
          continue
        for j in range(len(matrix[0]) - 1):
          if matrix[i][j] == Fraction(0):
            continue
          str_tmp += "x" + str(j + 1)
          break
        str_tmp += " = " + str(matrix[i][-1]) + " - "
        kostil = 0
        for j in range(len(matrix[0]) - 1):
          if matrix[i][j] == Fraction(0):
            continue
          if kostil == 0:
            kostil += 1
            continue
          str_tmp += "(" + str(
            matrix[i][j + 1] * Fraction(-1)) + ")" + "x" + str(j + 1) + " + "

      if str_tmp != "":
        tmp.append(str_tmp)
    print(*tmp, sep='\n')


def main():
  matrix = []
  f = open('input.txt', 'r')
  for line in f:
    a = list(map(int, line.strip().split(' ')))
    matrix.append(list(map(Fraction, a)))
  jordan_gauss_method(matrix)
  return


if __name__ == '__main__':
  main()