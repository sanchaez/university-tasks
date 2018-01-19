# Lab 2 : Solving one variable equations 
## Initial data
```
Base equation:  f(x) = 0; f(x) = log(x) - x + 5; 
Derivative:     f'(x) = 1/x - 1;
Roots :         0.0067838, 6.93685
```
## Test results
##### First root: [a,b] = [0.0001, 0.01]
```
Test 1: Iterative

:precision     :x
0.01           0.00583869815887097
1e-05          0.00677714367926403
1e-08          0.0067838046581917
1e-11          0.00678381134537336
1e-14          0.00678381135209032

Test 2: Hordes

:precision     :x
0.01           0.00917092684335846
1e-05          0.0067895314304509
1e-08          0.0067838081546676
1e-11          0.0067838113538845
1e-14          0.00678381135210144

Test 3: Iterations comparison

:precision          :iterative_count    :hordes_count
0.01                0                   0
1e-05               331                 4
1e-08               799                 9
1e-11               1267                14
1e-14               1736                18



```
##### Second root: [a,b] = [5, 7]
```
Test 1: Iterative

:precision     :x
0.01           6.93362372420261
1e-05          6.93684850301289
1e-08          6.93684741255941
1e-11          6.9368474072184
1e-14          6.93684740722021

Test 2: Hordes

:precision     :x
0.01           6.93496970458152
1e-05          6.93684597700509
1e-08          6.93684740613107
1e-11          6.93684740721939
1e-14          6.93684740722022

Test 3: Iterations comparison

:precision          :iterative_count    :hordes_count
0.01                2                   0
1e-05               5                   1
1e-08               7                   2
1e-11               10                  3
1e-14               12                  4
```