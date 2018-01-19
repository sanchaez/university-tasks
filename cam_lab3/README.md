# Lab 3 : Solving systems of linear equations
## Initial data
```
Input coefficient matrix 1
13        14        17        14        146
9         26        7         9         135
8         4         24        11        119
15        11        5         16        109

Roots :
x: 3, 3, 3, 1

Input matrix
100       5         30        11        146
5         100       20        10        135
5         10        100       4         119
5         9         5         90        109

Roots :
x: 1, 1, 1, 1

```
## Test results
##### First matrix (can't be solved with iterations method)
```
Input matrix
:ai1      :ai2      :ai3      :ai4      :bi
13        14        17        14        146
9         26        7         9         135
8         4         24        11        119
15        11        5         16        109


Roots (selection method):
x: 3                   3                   3                   1
e: 7.105427358e-15     3.552713679e-15     3.552713679e-15     3.552713679e-15


```
##### Second matrix (used to test iteration method)
```
Input matrix
:ai1      :ai2      :ai3      :ai4      :bi
100       5         30        11        146
5         100       20        10        135
5         10        100       4         119
5         9         5         90        109


Roots (selection method):
x:   1                   1                   1                   1
e:   7.105427358e-15     3.552713679e-15     2.842170943e-14     0

Roots (iteration): epsilon = 1e-15
x:   1                   1                   1                   1
e:   5.329070518e-15     5.329070518e-15     1.465494393e-14     1.421085472e-14

```