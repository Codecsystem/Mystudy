1. 多边形的面积 $A$
$$A = \sum_{i=1}^N A_i = \frac{1}{2} \sum_{i=1}^N (x_i y_{i+1} - x_{i+1} y_i)$$

2. 多边形重心
$$C_x = \frac{\sum_{i=1}^{N} (x_i y_{i+1} - x_{i+1} y_i)(x_i + x_{i+1})}{3 \sum_{i=1}^{N} (x_i y_{i+1} - x_{i+1} y_i)}$$
$$C_y = \frac{\sum_{i=1}^{N} (x_i y_{i+1} - x_{i+1} y_i)(y_i + y_{i+1})}{3 \sum_{i=1}^{N} (x_i y_{i+1} - x_{i+1} y_i)}$$（注：式中当 $i=N$ 时，$x_{N+1} = x_1$，$y_{N+1} = y_1$）