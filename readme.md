# Lab 4 : Solving integrals
## Initial data
```
Function: x^3(x^2/4 + 16x/5 - 1.7x);
Range = [2, 11]
```
## Test results
##### Single step calculation
```
:eps                :hstep              :value              :delta
0.0001              0.0001228613415     122118.075          3.298222146e-05
1e-05               3.885216756e-05     122118.075          3.298016964e-06
1e-06               1.228613415e-05     122118.075          3.294262569e-07
1e-07               3.885216756e-06     122118.075          3.188324627e-08
1e-08               1.228613415e-06     122118.075          8.367351256e-09
```

##### Runge step calculation
```
:eps                :nstep              :delta
0.0001              51200               6.751196634e-05
1e-05               161792              6.761183613e-06
1e-06               512000              6.733316695e-07
1e-07               1618944             7.107155398e-08
1e-08               5119488             1.212174539e-08
```

We can't get delta less than 10^-8 in Runge method because of error accumulation.